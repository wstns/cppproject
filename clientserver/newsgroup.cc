#include "newsgroup.h"
#include <iostream>
#include <stdexcept>

util::id_type Newsgroup::nextNewsgroupID = 1;

void Newsgroup::addArticle(Article article)
{
    articles[nextArticleID++] = article;
}

int Newsgroup::removeArticle(int id)
{
	int n = articles.erase(id);
	return (n == 0) ? -1 : id;
}

void Newsgroup::addArticle(const std::string &title, const std::string &author, const std::string &text)
{
    addArticle(Article(title, author, text));
}

Article Newsgroup::getArticle(int artID, int &check) const
{
	try {
		return articles.at(artID);
	} catch (std::exception &) {	// articles.at(artID) will throw an exception if the article wasn't found.
		check = -2;
		return Article();
	}
}
