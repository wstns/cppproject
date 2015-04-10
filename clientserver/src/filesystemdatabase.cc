#include "filesystemdatabase.h"
#include "article.h"
#include "newsgroup.h"
#include <algorithm>
#include <utility>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>

using namespace std;

FileSystemDatabase::FileSystemDatabase(string root_dir_path) {
	root_dir = root_dir_path;
	struct stat statbuf;
	if (stat(root_dir.c_str(), &statbuf) != -1) { 	//checking status on root_dir
   		if (S_ISDIR(statbuf.st_mode)) {				//checking if it was a directory
   			cout<<"Database folder already exists."<<endl;
			ifstream ng_meta_file;
			int nextID;
			ng_meta_file.open(root_dir+"/meta");
			ng_meta_file >> nextID;
			Newsgroup::setNextNewsgroupID(nextID);
		}
	}else {
		cout<<"Skapar databasmap"<<endl;
   		string cmd = "mkdir " + root_dir_path;
		system(cmd.c_str());	
		cmd = "touch meta";
		string ng_meta_path = root_dir_path+"/meta";
		ofstream out_ng_meta_file(ng_meta_path);
		out_ng_meta_file <<"1";
		out_ng_meta_file.close(); 
	}
}

int FileSystemDatabase::addNewsgroup(Newsgroup ng){
	if (findNewsgroup(ng.getTitle()) != -1) {
		return -1;
	}	

	string ng_path = root_dir+"/"+to_string(ng.getID());
	string cmd = "mkdir " + ng_path;
	system(cmd.c_str());
	//increment next newsgroup id in metafile.
	string ng_meta_path = root_dir+"/meta";
	ifstream ng_meta_file(ng_meta_path);
	string nextID;
	ng_meta_file >> nextID;
	ng_meta_file.close();
	ofstream out_ng_meta_file(ng_meta_path);
	out_ng_meta_file << stoi(nextID)+1;
	out_ng_meta_file.close();

	//creating meta_file for ng

	ofstream meta_file(ng_path+"/meta");
	meta_file<<"1"<<endl<<ng.getTitle()<<endl;
	meta_file.close();

	return ng.getID();
}

int FileSystemDatabase::removeNewsgroup(int id){
	int t = findNewsgroup(id);
	if(t != -1){
		string ng_path = root_dir+"/"+to_string(id);
		string cmd = "rm -rf " + ng_path;
		system(cmd.c_str());
	}
	return t;
}

vector<Newsgroup> FileSystemDatabase::listNewsgroups(){
	vector<Newsgroup> ngs;
	vector<int> ngIDs = readDirectory(root_dir);
	ifstream meta_file;
	string trash;
	string title;
	for(auto ngID : ngIDs){
		meta_file.open(root_dir+"/"+to_string(ngID)+"/meta");
		getline(meta_file,trash);
		getline(meta_file,title);
		ngs.push_back(Newsgroup(title, ngID));
		meta_file.close();
		title = "";
	}
	return ngs;
}

/* Returns -1 if name is not found. */
int FileSystemDatabase::findNewsgroup(const std::string &name){
	vector<Newsgroup> ngs = listNewsgroups();
	auto found = find_if(ngs.begin(), ngs.end(), [&](const Newsgroup &n){ return n.getTitle() == name;});
	if(found != ngs.end()){
		return found->getID();
	}
	return -1;
}

int FileSystemDatabase::findNewsgroup(int id){
	vector<Newsgroup> ngs = listNewsgroups();
	auto found = find_if(ngs.begin(), ngs.end(), [id](const Newsgroup &n){ return n.getID() == id;});
	if(found != ngs.end()){
		return id;
	}
	return -1;
}

int FileSystemDatabase::getAndIncrementNextArticleId(int ngID){
	int nextID;
	string ng_path = root_dir+"/"+to_string(ngID);
	string meta_path = ng_path+"/meta";

	ifstream meta_file(meta_path);
	string id;
	getline(meta_file,id);
	nextID = stoi(id);
	string title;
	getline(meta_file,title);
	meta_file.close();

	ofstream out_meta_file(meta_path);
	out_meta_file << nextID+1 << endl << title << endl;
	out_meta_file.close();
	return nextID;
}

int FileSystemDatabase::addArticle(int ngID, const std::string &title, const std::string &author, const std::string &text){
	if(findNewsgroup(ngID)!= -1){
		string ng_path = root_dir + "/" + to_string(ngID);
		int aID = getAndIncrementNextArticleId(ngID);
		string a_path = ng_path + "/"+to_string(aID);
		ofstream a_file(a_path);
		a_file<<title<<endl<<author<<endl<<text<<endl;
		a_file.close();
		return ngID;
	}
	return -1;
}

int FileSystemDatabase::listArticles(int ngID, std::vector<std::pair<int ,Article>> &vec){
	if(findNewsgroup(ngID)!=-1){
		string ng_path = root_dir + "/" + to_string(ngID);
		vector<int> aids = readDirectory(root_dir+"/"+to_string(ngID));
		int check;
		for(auto aID : aids)
			vec.push_back(make_pair(aID, getArticle(ngID, aID, check)));
		return ngID;
	}
	return -1;
}

vector<int> FileSystemDatabase::readDirectory(string dir_path){
	vector<int> dir;
	struct dirent *cur;
	string filename;
	DIR *root = opendir(dir_path.c_str());
	while((cur = readdir(root)) != NULL){
		filename = string(cur->d_name);
		if(filename != "." && filename != ".." && filename != "meta"){
			dir.push_back(stoi(filename));
		}
	}
	return dir;
}

int FileSystemDatabase::deleteArticle(int ngID, int artID){
	int check;
	getArticle(ngID, artID, check);
	if (check == -1)
		return -1;
	if (check == -2)
		return -2;
	string ng_path = root_dir + "/" + to_string(ngID);
	string cmd = "rm " + ng_path + "/" + to_string(artID);
	system(cmd.c_str());
	//todo: check?
	return artID;
}

Article FileSystemDatabase::getArticle(int ngID, int artID, int &check){
	if (findNewsgroup(ngID) != -1) {
		string file_path = root_dir+"/"+to_string(ngID)+"/"+to_string(artID);
		ifstream a_stream;
		a_stream.open(file_path.c_str());
		if(!a_stream){
			check = -2;
			return Article();
		}
		string title, author, text;
		getline(a_stream,title);
		getline(a_stream,author);
		string line;
		while(getline(a_stream,line)){
			text += line+"\n";
		}
		text.pop_back();
		check = ngID;
		return Article(title, author, text);
	}
	check = -1;
	return Article();
}
