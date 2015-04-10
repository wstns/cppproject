#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include "messagehandler.h"

#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <vector>

using namespace std;

/*
 * Read an integer from a client.
 */
int readNumber(const shared_ptr<Connection>& conn) {
	unsigned char byte1 = conn->read();
	unsigned char byte2 = conn->read();
	unsigned char byte3 = conn->read();
	unsigned char byte4 = conn->read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

/*
 * Send a string to a client.
 */
void writeString(const shared_ptr<Connection>& conn, const string& s) {
	for (char c : s) {
		conn->write(c);
	}
	conn->write('$');
}

void listNewsgroups(MessageHandler &mess, Database &database)
{
    mess.receiveComEnd();
	vector<Newsgroup> newsgroups = database.listNewsgroups();
	mess.sendCode(Protocol::ANS_LIST_NG);
	mess.sendIntParameter(newsgroups.size());

	for (const Newsgroup &ng : newsgroups) {
		mess.sendIntParameter(ng.getID());
		mess.sendStringParameter(ng.getTitle());
	}

	mess.sendCode(Protocol::ANS_END);
}

void createNewsgroup(MessageHandler &mess, Database &database)
{
	string name = mess.receiveStringParameter();
	mess.receiveComEnd();
	mess.sendCode(Protocol::ANS_CREATE_NG);

	if (database.findNewsgroup(name) == -1) {	// Not found.
		database.addNewsgroup(name);
		mess.sendCode(Protocol::ANS_ACK);
	} else {
		mess.sendCode(Protocol::ANS_NAK);
		mess.sendCode(Protocol::ERR_NG_ALREADY_EXISTS);
	}

	mess.sendCode(Protocol::ANS_END);
}

void deleteNewsgroup(MessageHandler &mess, Database &database)
{
    int ngID = mess.receiveIntParameter();
    mess.receiveComEnd();

    mess.sendCode(Protocol::ANS_DELETE_NG);
    int removed = database.removeNewsgroup(ngID);
    if (removed == -1) {
        mess.sendCode(Protocol::ANS_NAK);
        mess.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    } else {
        mess.sendCode(Protocol::ANS_ACK);
    }
    mess.sendCode(Protocol::ANS_END);
}

void listArticles(MessageHandler &mess, Database &database)
{
    int ngID = mess.receiveIntParameter();
    mess.receiveComEnd();

    mess.sendCode(Protocol::ANS_LIST_ART);

    vector<pair<int, Article>> vec;
    int id = database.listArticles(ngID, vec);
    if (id == -1) { // Newsgroup not found.
        mess.sendCode(Protocol::ANS_NAK);
        mess.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    } else {
        mess.sendCode(Protocol::ANS_ACK);
        mess.sendIntParameter(vec.size());
        for (const pair<int, Article> &p : vec) {
            mess.sendIntParameter(p.first);
            mess.sendStringParameter(p.second.getTitle());
        }
    }

    mess.sendCode(Protocol::ANS_END);
}

void createArticle(MessageHandler &mess, Database &database)
{
	int ngID;
	string title, author, text;

    ngID = mess.receiveIntParameter();
    title = mess.receiveStringParameter();
    author = mess.receiveStringParameter();
    text = mess.receiveStringParameter();
    mess.receiveComEnd();

    ngID = database.addArticle(ngID, title, author, text);
    mess.sendCode(Protocol::ANS_CREATE_ART);

    if (ngID == -1) { // Failed to find the newsgroup with ngID as ID.
        mess.sendCode(Protocol::ANS_NAK);
        mess.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
    } else {
        mess.sendCode(Protocol::ANS_ACK);
    }
    mess.sendCode(Protocol::ANS_END);
}

void deleteArticle(MessageHandler &mess, Database &database)
{
	int ngID = mess.receiveIntParameter();
	int artID = mess.receiveIntParameter();
	mess.receiveComEnd();

	mess.sendCode(Protocol::ANS_DELETE_ART);
	int ret = database.deleteArticle(ngID, artID);
	switch (ret) {
	case -1:
		mess.sendCode(Protocol::ANS_NAK);
		mess.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
		break;
	case -2:
		mess.sendCode(Protocol::ANS_NAK);
		mess.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
		break;
	default:
		mess.sendCode(Protocol::ANS_ACK);
		break;
	}

	mess.sendCode(Protocol::ANS_END);
}

void getArticle(MessageHandler &mess, Database &database)
{
	int ngID = mess.receiveIntParameter();
	int artID = mess.receiveIntParameter();
	mess.receiveComEnd();

	mess.sendCode(Protocol::ANS_GET_ART);
	int check = 0;
	Article art = database.getArticle(ngID, artID, check);
	switch (check) {
	case -1:
		mess.sendCode(Protocol::ANS_NAK);
		mess.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
		break;
	case -2:
		mess.sendCode(Protocol::ANS_NAK);
		mess.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
		break;
	default:
		mess.sendCode(Protocol::ANS_ACK);
		mess.sendStringParameter(art.getTitle());
		mess.sendStringParameter(art.getAuthor());
		mess.sendStringParameter(art.getText());
		break;
	}

	mess.sendCode(Protocol::ANS_END);
}

void handleIt(MessageHandler &mess, Database &database)
{
	int code = mess.receiveCode();

	switch (code) {
	case Protocol::COM_LIST_NG:
		listNewsgroups(mess, database);
		break;
	case Protocol::COM_CREATE_NG:
		createNewsgroup(mess, database);
		break;
	case Protocol::COM_DELETE_NG:
		deleteNewsgroup(mess, database);
		break;
	case Protocol::COM_LIST_ART:
		listArticles(mess, database);
		break;
	case Protocol::COM_CREATE_ART:
		createArticle(mess, database);
		break;
	case Protocol::COM_DELETE_ART:
		deleteArticle(mess, database);
		break;
	case Protocol::COM_GET_ART:
		getArticle(mess, database);
		break;
	default:
		cerr << "Invalid code command." << endl;
		//exit(EXIT_FAILURE);
		break;
	}
}

#include "newsgroup.h"

int main(int argc, char* argv[]){
	if (argc != 2) {
		cerr << "Usage: myserver port-number" << endl;
		exit(1);
	}

	int port = -1;
	try {
		port = stoi(argv[1]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}

	Server server(port, FILE_SYSTEM_DATABASE);
	if (!server.isReady()) {
		cerr << "Server initialization error." << endl;
		exit(1);
	}

	shared_ptr<Database> database = server.getDatabase();
	while (true) {
		auto conn = server.waitForActivity();
		if (conn != nullptr) {
			try {
				MessageHandler &mess = conn->getMessageHandler();
				/* This is a mess, he has to handle it. */
				handleIt(mess, *database);
			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				cout << "Client closed connection" << endl;
			}
		} else {
			conn = make_shared<Connection>();
			server.registerConnection(conn);
			cout << "New client connects" << endl;
		}
	}
}
