#include "inmemorydatabase.h"
#include "article.h"
#include <algorithm>
#include <utility>

int InMemoryDatabase::addNewsgroup(Newsgroup ng)
{
	if (findNewsgroup(ng.getTitle()) != -1) { // A newsgroup with that name already exists.
		return -1;
	}
	auto it = std::lower_bound(newsgroups.begin(), newsgroups.end(), ng);
	newsgroups.insert(it, ng);
	return ng.getID();
}

int InMemoryDatabase::removeNewsgroup(int id)
{
	auto it = std::find_if(newsgroups.begin(), newsgroups.end(), [id] (const Newsgroup &ng) { return ng.getID() == id; });
	if (it == newsgroups.end())
		return -1;
	newsgroups.erase(it);
	return id;
}

const std::vector<Newsgroup> *InMemoryDatabase::getNewsgroups() const
{
    return &newsgroups;
}

/* Returns -1 if name is not found. */
int InMemoryDatabase::findNewsgroup(const std::string &name)
{
	auto it = std::find_if(newsgroups.begin(), newsgroups.end(), [&name] (const Newsgroup &ng) { return ng.getTitle() == name; });
	if (it == newsgroups.end())
		return -1;
	return it->getID();
}

std::vector<Newsgroup> InMemoryDatabase::listNewsgroups()
{
	return newsgroups;
}

int InMemoryDatabase::addArticle(int ngID, const std::string &title, const std::string &author, const std::string &text)
{
	auto it = std::find_if(newsgroups.begin(), newsgroups.end(), [ngID] (const Newsgroup &ng) { return ng.getID() == ngID; });
	if (it == newsgroups.end())
		return -1;
	it->addArticle(title, author, text);
	return ngID;
}

int InMemoryDatabase::listArticles(int ngID, std::vector<std::pair<int ,Article>> &vec)
{
    auto it = std::find_if(newsgroups.begin(), newsgroups.end(), [ngID] (const Newsgroup &ng) { return ng.getID() == ngID; });
    if (it == newsgroups.end())
        return -1;

    const std::map<int, Article> *articleMap = it->getArticles();
    for (const std::pair<int, Article> &p : *articleMap)
        vec.push_back(p);
    return ngID;
}

int InMemoryDatabase::deleteArticle(int ngID, int artID)
{
	auto it = std::find_if(newsgroups.begin(), newsgroups.end(), [ngID] (const Newsgroup &ng) { return ng.getID() == ngID; });
	if (it == newsgroups.end())
		return -1;
	int ret = it->removeArticle(artID);
	if (ret == -1)
		return -2;
	return ngID;
}

Article InMemoryDatabase::getArticle(int ngID, int artID, int &check)
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
