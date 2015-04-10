
#include "connection.h"
#include "messagehandler.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include "utility.h"
#include "article.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <utility>

using namespace std;

void printStartMessage(ostream &os)
{
	os << "Commands:\n\t\"list\": Lists all newsgroups.\n\t\"list [newsgroup]\": Lists articles in newgroup.\n"
	"\t\"create [name]\": Creates a newsgroup with the given name.\n"
	"\t\"delete [number]\": Deletes the newsgroup with the given ID number.\n"
	"\t\"createa [number] [newline] [title] [newline] [author] [newline] [text] $\": Creates and article in the newsgroup with the given ID.\n"
	"\t\"deletea [number] [number]\": Deletes the article with the given ID (second number) in the given newsgroup (first number).\n"
	"\t\"get [number] [number]\": Gets the article with the given ID (second number) in the given newsgroup (first number).\n" 
	"\t\"quit\" : exits the client.\n"<< endl;
}

void listNewsgroups(MessageHandler &mess)
{
	mess.sendCode(Protocol::COM_LIST_NG);
	mess.sendCode(Protocol::COM_END);

	int response = mess.receiveCode();
	if (response != Protocol::ANS_LIST_NG) {
		util::error("Error: Invalid response to list newsgroups.");
	}

	int num = mess.receiveIntParameter();
	if (num < 0) {
		util::error("Error: Negative amount of newsgroups.");
	}
	if(num==0){
		cout<<"There are no newsgroups, make one!"<<endl;
	}else{
		for (int i = 0; i != num; ++i) {
			int id = mess.receiveIntParameter();
			string ngName = mess.receiveStringParameter();
			cout << id << '\t' << ngName << endl;
		}
	}

	mess.receiveAnsEnd();
}

void printArticleList(MessageHandler &mess)
{
	int num = mess.receiveIntParameter();
	if (num < 0)
		util::error("Error: Negative amount of articles in newsgroup.");
	if(num == 0){
		cout<<"No articles in the newsgroup"<<endl;
	}else{
		for (int i = 0; i != num; ++i) {
			int id = mess.receiveIntParameter();
			string artName = mess.receiveStringParameter();
			cout << id << '\t' << artName << endl;
		}
	}
}

void listArticlesInNewsgroup(MessageHandler &mess, int id)
{
	mess.sendCode(Protocol::COM_LIST_ART);
	mess.sendIntParameter(id);
	mess.sendCode(Protocol::COM_END);

	int response = mess.receiveCode();
	if (response != Protocol::ANS_LIST_ART)
		util::error("Error: Invalid response to list articles.");

	response = mess.receiveCode();
	switch (response) {
	case Protocol::ANS_ACK:
		printArticleList(mess);
		break;
	case Protocol::ANS_NAK:
		response = mess.receiveCode();
		if (response != Protocol::ERR_NG_DOES_NOT_EXIST)
			util::error("Error: Invalid response received after ANS_NAK to list articles.");
		cout << "Newsgroup does not exist." << endl;
		break;
	}

	mess.receiveAnsEnd();
}

void createNewsgroup(MessageHandler &mess, const string &s)
{
	mess.sendCode(Protocol::COM_CREATE_NG);
	mess.sendStringParameter(s);
	mess.sendCode(Protocol::COM_END);

	int response = mess.receiveCode();
	if (response != Protocol::ANS_CREATE_NG)
		util::error("Error: Expected ANS_CREATE_NG but received something else.");

	response = mess.receiveCode();
	if (response == Protocol::ANS_NAK) {
		response = mess.receiveCode();
		if (response != Protocol::ERR_NG_ALREADY_EXISTS)
			util::error("Error: Invalid response received after ANS_NAK to create newsgroup.");
		cout<<"Newsgroup already exists"<<endl;
	}

	mess.receiveAnsEnd();
}

void createArticle(MessageHandler &mess, istream &is, int ngID)
{
	string title, author, text;

	if (getline(is, title) && getline(is, author)) {
        char ch;
		while ((ch = is.get()) != '$')
			text += ch;
	} else {
		util::error("Error: Invalid format on create article command.");
	}

	if (ngID <= 0)
		util::error("Error: newsgroup ID supplied is not greater than zero.");

	mess.sendCode(Protocol::COM_CREATE_ART);
	mess.sendIntParameter(ngID);
	mess.sendStringParameter(title);
	mess.sendStringParameter(author);
	mess.sendStringParameter(text);
	mess.sendCode(Protocol::COM_END);

	int response = mess.receiveCode();
	if (response != Protocol::ANS_CREATE_ART)
		util::error("Error: Expected response ANS_CREATE_ART but received something else.");

	response = mess.receiveCode();
    switch (response) {
    case Protocol::ANS_ACK:
        break;
    case Protocol::ANS_NAK:
        response = mess.receiveCode();
        if (response != Protocol::ERR_NG_DOES_NOT_EXIST)
            util::error("Error: Invalid response after ANS_NAK when creating article.");
        cout << "Newsgroup " << ngID << " does not exist." << endl;
        break;
    default:
        util::error("Invalid response after code ANS_CREATE_ART.");
    }

    mess.receiveAnsEnd();
}

void deleteNewsgroup(MessageHandler &mess, int id)
{
    mess.sendCode(Protocol::COM_DELETE_NG);
    mess.sendIntParameter(id);
    mess.sendCode(Protocol::COM_END);

    int response = mess.receiveCode();
    if (response != Protocol::ANS_DELETE_NG)
        util::error("Error: Expected response ANS_DELETE_NG but received something else.");

    response = mess.receiveCode();
    switch (response) {
    case Protocol::ANS_ACK:
    	cout<<"Deleted"<<endl;
        break;
    case Protocol::ANS_NAK:
        if (mess.receiveCode() != Protocol::ERR_NG_DOES_NOT_EXIST)
            util::error("Expected response ERR_NG_DOES_NOT_EXIST after ANS_NAK but received something else.");
        cout << "Newsgroup with ID " << id << " does not exist." << endl;
        break;
    default:
        util::error("Error: Invalid code received after ANS_DELETE_NG.");
        break;
    }
    mess.receiveAnsEnd();
}

void deleteArticle(MessageHandler &mess, int ngID, int artID)
{
	mess.sendCode(Protocol::COM_DELETE_ART);
	mess.sendIntParameter(ngID);
	mess.sendIntParameter(artID);
	mess.sendCode(Protocol::COM_END);

	if (mess.receiveCode() != Protocol::ANS_DELETE_ART)
        	util::error("Error: Expected response ANS_DELETE_ART but received something else.");
	
	int code = mess.receiveCode();
	switch (code) {
	case Protocol::ANS_ACK:
		cout<<"Deleted"<<endl;
		break;
	case Protocol::ANS_NAK:
		code = mess.receiveCode();
		if (code == Protocol::ERR_NG_DOES_NOT_EXIST)
			cout << "Newsgroup with ID " << ngID << " does not exist." << endl;
		else if (code == Protocol::ERR_ART_DOES_NOT_EXIST)
			cout << "Article with ID " << artID << "in newsgroup with ID " << ngID << "does not exist." << endl;
		else
		        util::error("Error: Invalid code received after ANS_NAK.");
		break;
	default:
		util::error("Error: Invalid code received after ANS_DELETE_ART.");
        break;
	}

	mess.receiveAnsEnd();
}

void getArticle(MessageHandler &mess, int ngID, int artID)
{
	mess.sendCode(Protocol::COM_GET_ART);
	mess.sendIntParameter(ngID);
	mess.sendIntParameter(artID);
	mess.sendCode(Protocol::COM_END);

	int code = mess.receiveCode();
	if (code != Protocol::ANS_GET_ART)
		util::error("Error: Expected ANS_GET_ART but received something else.");

	code = mess.receiveCode();
	string s1, s2, s3;
	switch (code) {
	case Protocol::ANS_ACK:
		s1 = mess.receiveStringParameter();
		s2 = mess.receiveStringParameter();
		s3 = mess.receiveStringParameter();
		Article(s1, s2, s3).print(cout) << endl;
		break;
	case Protocol::ANS_NAK:
		code = mess.receiveCode();
		if (code == Protocol::ERR_NG_DOES_NOT_EXIST)
			cout << "Newsgroup with ID " << ngID << " does not exist." << endl;
		else if (code == Protocol::ERR_ART_DOES_NOT_EXIST)
			cout << "Article with ID " << artID << " in newsgroup with ID " << ngID << " does not exist." << endl;
		else
		        util::error("Error: Invalid code received after ANS_NAK.");
		break;
	default:
		util::error("Error: Invalid code received after ANS_GET_ART.");
		break;
	}

	mess.receiveAnsEnd();
}

void handleInput(MessageHandler &mess)
{
	string line;
	getline(cin, line);
	string s;
	istringstream iss(line);
	iss >> s;

	if (s == "list") {
		int id;
		if (iss >> s) {	// If the user supplied a number.
			try {
				id = stoi(s);
			} catch (exception &) {
				cout << "Invalid input" << endl;
				return;
			}
			if(iss >> s){
				cout<<"Too many arguments"<<endl;
			}else{
				listArticlesInNewsgroup(mess, id);
			}
		} else{	// No newsgroup was given, list the newsgroups.
			listNewsgroups(mess);
		}
	} else if (s == "create") {
		iss >> s;
		createNewsgroup(mess, s);
	} else if (s == "delete") {
        	int id;
        	iss >> id;
        	deleteNewsgroup(mess, id);
	} else if (s == "createa") {
        	int id;
        	iss >> id;
		createArticle(mess, cin, id);
	} else if (s == "deletea") {
        	int ngID, artID;
        	iss >> ngID >> artID;
        	deleteArticle(mess, ngID, artID);
	} else if (s == "get") {
		int ngID, artID;
		if(iss >> ngID >> artID) {
			if(iss >> s){
				cout<<"Too many arguments"<<endl;
			}else{
				getArticle(mess, ngID, artID);	
			}
		}
		else
			cout << "Invalid input" << endl;
	}else if (s == "quit"){	
		exit(0);
	}else
		cout<<"Not a valid command"<<endl;

}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: myclient host-name port-number" << endl;
		exit(1);
	}

	int port = -1;
	try {
		port = stoi(argv[2]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}

	Connection conn(argv[1], port);
	if (!conn.isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}

	MessageHandler &mess = conn.getMessageHandler();

	printStartMessage(cout);
	while (true) {
		handleInput(mess);
	}
}

