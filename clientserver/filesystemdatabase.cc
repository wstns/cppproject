#include "filesystemdatabase.h"
#include "article.h"
#include <algorithm>
#include <utility>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

using namespace std;

FileSystemDatabase::FileSystemDatabase(string root_dir) {
	root_dir_path = root_dir;
	string cmd = "mkdir " + root_dir_path;
	system(cmd.c_str());
}

void FileSystemDatabase::addNewsgroup(Newsgroup ng){
	string ng_path = root_dir_path+"/"+to_string(ng.getID());
	string cmd = "mkdir " + ng_path;
	system(cmd.c_str());
	cmd = "touch " + ng_path+"/title:" + ng.getTitle();
	system(cmd.c_str());

	auto articles = ng.getArticles();
	for(auto i = articles->begin(); i != articles->end(); i++) {
		auto article = i->second;
		string a_path = ng_path + "/" + to_string(i->first);

		FILE *a_file;
		a_file = fopen(a_path.c_str(), "w");
		if(a_file){
			string contents = article.getTitle()+";"+article.getAuthor() + "\n" + article.getText();
			fputs(contents.c_str(), a_file);
			fclose(a_file);
		}
	}
}

int FileSystemDatabase::removeNewsgroup(int id){
	string ng = root_dir_path+"/"+to_string(id);
	string cmd = "rm -rf " + ng;
	system(cmd.c_str());
}

const std::vector<Newsgroup> *FileSystemDatabase::getNewsgroups() const{
	vector<Newsgroup> ngs;
	DIR *root = opendir(root_dir_path.c_str());
	struct dirent *cur;
	while(cur = readdir(root)){
		cout<<cur->d_name;
	}
}

Newsgroup readNewsgroup(int id){

}

/* Returns -1 if name is not found. */
int FileSystemDatabase::findNewsgroup(const std::string &name){
	// auto it = std::find_if(newsgroups.begin(), newsgroups.end(), [&name] (const Newsgroup &ng) { return ng.getTitle() == name; });
	// if (it == newsgroups.end())
	// 	return -1;
	// return it->getID();
}

std::vector<Newsgroup> FileSystemDatabase::listNewsgroups()
{
	// return newsgroups;
}

int FileSystemDatabase::addArticle(int ngID, const std::string &title, const std::string &author, const std::string &text)
{
	// auto it = std::find_if(newsgroups.begin(), newsgroups.end(), [ngID] (const Newsgroup &ng) { return ng.getID() == ngID; });
	// if (it == newsgroups.end())
	// 	return -1;
	// it->addArticle(title, author, text);
	// return ngID;
}

int FileSystemDatabase::listArticles(int ngID, std::vector<std::pair<int ,Article>> &vec)
{
    // auto it = std::find_if(newsgroups.begin(), newsgroups.end(), [ngID] (const Newsgroup &ng) { return ng.getID() == ngID; });
    // if (it == newsgroups.end())
    //     return -1;
		//
    // const std::map<int, Article> *articleMap = it->getArticles();
    // for (const std::pair<int, Article> &p : *articleMap)
    //     vec.push_back(p);
    // return ngID;
}

int FileSystemDatabase::deleteArticle(int ngID, int artID)
{
	// auto it = std::find_if(newsgroups.begin(), newsgroups.end(), [ngID] (const Newsgroup &ng) { return ng.getID() == ngID; });
	// if (it == newsgroups.end())
	// 	return -1;
	// int ret = it->removeArticle(artID);
	// if (ret == -1)
	// 	return -2;
	// return ngID;
}

Article FileSystemDatabase::getArticle(int ngID, int artID, int &check)
{
	// auto it = std::find_if(newsgroups.begin(), newsgroups.end(), [ngID] (const Newsgroup &ng) { return ng.getID() == ngID; });
	// if (it == newsgroups.end()) {
	// 	check = -1;
	// 	return Article();
	// }
	// Article ret = it->getArticle(artID, check);
	// if (check == -2) {
	// 	return Article();
	// }
	// check = ngID;
	// return ret;
}
