#include "inmemorydatabase.h"
#include "article.h"
#include <algorithm>
#include <utility>

int InMemoryDatabase::removeNewsgroup(util::id_type id)
{
    auto n = newsgroups.erase(Newsgroup("", id));
    return (n == 0) ? -1 : id;
}

const std::set<Newsgroup> *InMemoryDatabase::getNewsgroups() const
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
	std::vector<Newsgroup> vec;
	for (const Newsgroup &ng : newsgroups)
		vec.push_back(ng);

	return vec;
}

int InMemoryDatabase::addArticle(int ngID, const std::string &title, const std::string &author, const std::string text)
{
    auto it = newsgroups.find(Newsgroup("", ngID));
    if (it == newsgroups.end())
        return -1;

    Newsgroup ng = *it;
    ng.addArticle(Article(title, author, text));
    return ngID;
}

int InMemoryDatabase::listArticles(int ngID, std::vector<std::pair<int ,Article>> &vec)
{
    auto it = newsgroups.find(Newsgroup("", ngID));
    if (it == newsgroups.end())
        return -1;

    const std::map<int, Article> *articleMap = it->getArticles();
    for (const std::pair<int, Article> &p : *articleMap)
        vec.push_back(p);
    return ngID;
}
