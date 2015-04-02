#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <map>
#include <string>
#include "utility.h"
#include "article.h"

/* Newsgroups contain a bunch of articles */
class Newsgroup {
public:
    Newsgroup(std::string tit, util::id_type ident) : title(tit), id(ident) {}
    Newsgroup(std::string tit) : Newsgroup(tit, nextNewsgroupID++) {}
    /* Newsgroups are compared by their ID's. */
    bool operator<(const Newsgroup &rhs) const { return id < rhs.id; }
    bool operator==(const Newsgroup &rhs) const { return id == rhs.id; }
    /* Gets the article with the given ID. What should happen if it can't be found? */
    const Article *getArticle(util::id_type id) const { return &articles.at(id); }
    /* Adds the article to the group. */
    void addArticle(Article article);
    void addArticle(const std::string &title, const std::string &author, const std::string &text);
    /* Removes the article with the given ID. */
    void removeArticle(util::id_type id);
    std::string getTitle() const { return title; }
    const std::map<util::id_type, Article> *getArticles() const { return &articles; }
	int getID() const { return id; }
private:
    std::string title;
    std::map<int, Article> articles;
    /* The unique ID belonging to the newsgroup. */
    int id;
    /* nextArticleID is the ID to be given to the next article added to this newsgroup. */
    int nextArticleID = 1;
    /* nextNewsgroupID is the ID the will be given to the next Newsgroup. */
    static util::id_type nextNewsgroupID;
};

#endif /* NEWSGROUP_H */
