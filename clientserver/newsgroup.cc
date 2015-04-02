#include "newsgroup.h"
#include <iostream>

util::id_type Newsgroup::nextNewsgroupID = 1;

void Newsgroup::addArticle(Article article)
{
    articles[nextArticleID++] = article;
}

void Newsgroup::removeArticle(util::id_type id)
{
    articles.erase(id);
}

void Newsgroup::addArticle(const std::string &title, const std::string &author, const std::string &text)
{
    addArticle(Article(title, author, text));
}
