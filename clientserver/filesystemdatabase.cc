#include "filesystemdatabase.h"
#include "article.h"
#include <algorithm>
#include <utility>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>

using namespace std;

FileSystemDatabase::FileSystemDatabase(string root_dir_path) {
	root_dir = root_dir_path;
	string cmd = "mkdir " + root_dir_path;
	system(cmd.c_str());
}

void FileSystemDatabase::addNewsgroup(Newsgroup ng){
	string ng_path = root_dir+"/"+to_string(ng.getID());
	string cmd = "mkdir " + ng_path;
	system(cmd.c_str());

	ofstream meta_file(ng_path+"/meta");
	meta_file<<"0"<<endl<<ng.getTitle()<<endl;
	meta_file.close();

	for(auto a : *ng.getArticles()){
		Article article = a.second;
		addArticle(ng.getID(), article.getTitle(), article.getAuthor(), article.getText());
	}
}

int FileSystemDatabase::removeNewsgroup(int id){
	string ng = root_dir+"/"+to_string(id);
	string cmd = "rm -rf " + ng;
	system(cmd.c_str());

	return id; //todo: wät?
}

std::vector<Newsgroup> FileSystemDatabase::listNewsgroups(){
	vector<Newsgroup> ngs;
	vector<int> ngIDs = readDirectory(root_dir);
	ifstream meta_file;
	string trash;
	string title;
	for(auto ngID : ngIDs){
		meta_file.open(root_dir+"/"+to_string(ngID)+"/meta");
		meta_file >> trash;
		meta_file >> title;
		ngs.push_back(Newsgroup(title, ngID));
		meta_file.close();
		title = "";
	}
	return ngs;
}

/* Returns -1 if name is not found. */
int FileSystemDatabase::findNewsgroup(const std::string &name){
	vector<Newsgroup> ngs = listNewsgroups();
	auto found = find_if(ngs.begin(), ngs.end(), [&](Newsgroup n){return n.getTitle() == name;});
	if(found != ngs.end()){
		return found->getID();
	}
	return -1;
}

int FileSystemDatabase::getAndIncrementNextArticleId(int ngID){
	int nextID;
	string ng_path = root_dir+"/"+to_string(ngID);
	string meta_path = ng_path+"/meta";

	ifstream meta_file(meta_path);
	string id;
	meta_file >> id;
	nextID = stoi(id);
	string title;
	meta_file >> title;
	meta_file.close();

	ofstream out_meta_file(meta_path);
	out_meta_file << nextID+1 << endl << title << endl;
	out_meta_file.close();
	return nextID;
}

int FileSystemDatabase::addArticle(int ngID, const std::string &title, const std::string &author, const std::string &text){
	int aID = getAndIncrementNextArticleId(ngID);

	string a_path = root_dir + "/" + to_string(ngID) + "/"+to_string(aID);
	ofstream a_file(a_path);
	a_file<<title<<endl<<author<<endl<<text<<endl;
	a_file.close();
	return ngID;
}

int FileSystemDatabase::listArticles(int ngID, std::vector<std::pair<int ,Article>> &vec){
	vector<int> aids = readDirectory(root_dir+"/"+to_string(ngID));
	int check;
	for(auto aID : aids)
		vec.push_back(make_pair(aID, getArticle(ngID, aID, check)));
	//todo: check?
	return ngID;
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
	string cmd = "rm " + root_dir + "/" + to_string(ngID) + "/" + to_string(artID);
	system(cmd.c_str());
	//todo: check;
	return artID; //todo: wät?
}

Article FileSystemDatabase::getArticle(int ngID, int artID, int &check){
	string file_path = root_dir+"/"+to_string(ngID)+"/"+to_string(artID);
	ifstream a_stream;
	a_stream.open(file_path.c_str());
	if(!a_stream){
		check = -1;
		return Article();
	}
	string title, author, text;
	a_stream >> title;
	a_stream >> author;
	a_stream >> text;
	check = ngID;
	return Article(title, author, text);
}
