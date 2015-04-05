#include "filesystemdatabase.h"
#include "article.h"
#include <algorithm>
#include <utility>

FileSystemDatabase::FileSystemDatabase(string file){


}

void FileSystemDatabase::addNewsgroup(Newsgroup ng)
{
	auto it = std::lower_bound(newsgroups.begin(), newsgroups.end(), ng);
	newsgroups.insert(it, ng);
}

int FileSystemDatabase::removeNewsgroup(int id)
{
	auto it = std::find_if(newsgroups.begin(), newsgroups.end(), [id] (const Newsgroup &ng) { return ng.getID() == id; });
	if (it == newsgroups.end())
		return -1;
	newsgroups.erase(it);
	return id;
}

const std::vector<Newsgroup> *FileSystemDatabase::getNewsgroups() const
{
    return &newsgroups;
}

/* Returns -1 if name is not found. */
int FileSystemDatabase::findNewsgroup(const std::string &name)
{
	auto it = std::find_if(newsgroups.begin(), newsgroups.end(), [&name] (const Newsgroup &ng) { return ng.getTitle() == name; });
	if (it == newsgroups.end())
		return -1;
	return it->getID();
}

std::vector<Newsgroup> FileSystemDatabase::listNewsgroups()
{
	return newsgroups;
}

int FileSystemDatabase::addArticle(int ngID, const std::string &title, const std::string &author, const std::string &text)
{
	auto it = std::find_if(newsgroups.begin(), newsgroups.end(), [ngID] (const Newsgroup &ng) { return ng.getID() == ngID; });
	if (it == newsgroups.end())
		return -1;
	it->addArticle(title, author, text);
	return ngID;
}

int FileSystemDatabase::listArticles(int ngID, std::vector<std::pair<int ,Article>> &vec)
{
    auto it = std::find_if(newsgroups.begin(), newsgroups.end(), [ngID] (const Newsgroup &ng) { return ng.getID() == ngID; });
    if (it == newsgroups.end())
        return -1;

    const std::map<int, Article> *articleMap = it->getArticles();
    for (const std::pair<int, Article> &p : *articleMap)
        vec.push_back(p);
    return ngID;
}

int FileSystemDatabase::deleteArticle(int ngID, int artID)
{
	auto it = std::find_if(newsgroups.begin(), newsgroups.end(), [ngID] (const Newsgroup &ng) { return ng.getID() == ngID; });
	if (it == newsgroups.end())
		return -1;
	int ret = it->removeArticle(artID);
	if (ret == -1)
		return -2;
	return ngID;
}

Article FileSystemDatabase::getArticle(int ngID, int artID, int &check)
{
	auto it = std::find_if(newsgroups.begin(), newsgroups.end(), [ngID] (const Newsgroup &ng) { return ng.getID() == ngID; });
	if (it == newsgroups.end()) {
		check = -1;
		return Article();
	}
	Article ret = it->getArticle(artID, check);
	if (check == -2) {
		return Article();
	}
	check = ngID;
	return ret;
}
