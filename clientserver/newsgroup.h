#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <map>
#include <string>
#include "utility.h"
#include "article.h"

/* Newsgroups contain a bunch of articles */
class Newsgroup {
public:
  Newsgroup(std::string tit, int ident) : title(tit), id(ident) {}
  Newsgroup(std::string tit) : Newsgroup(tit, nextNewsgroupID++) {}
    /* Newsgroups are compared by their ID's. */
  bool operator<(const Newsgroup &rhs) const { return id < rhs.id; }
  bool operator==(const Newsgroup &rhs) const { return id == rhs.id; }
    /* Gets the article with the given ID. What should happen if it can't be found? */
  const Article *getArticle(int id) const { return &articles.at(id); }
    /* Adds the article to the group. */
  void addArticle(Article article);
  void addArticle(const std::string &title, const std::string &author, const std::string &text);
    /* Removes the article with the given ID. Returns -1 if element was not found, else returns id. */
  int removeArticle(int id);
  std::string getTitle() const { return title; }
  const std::map<int, Article> *getArticles() const { return &articles; }
  int getAndIncrementNextArticleID();   //Denna liiger i filesystemdatabase. det e där den ska ligga tror jag.
  static void setNextNewsgroupID(int newNextID) {nextNewsgroupID = newNextID;}
	int getID() const { return id; }
	Article getArticle(int artID, int &check) const;
private:
    std::string title;
    std::map<int, Article> articles;
    /* The unique ID belonging to the newsgroup. */
    int id;
    /* nextArticleID is the ID to be given to the next article added to this newsgroup. */
    int nextArticleID = 1;
    /* nextNewsgroupID is the ID the will be given to the next Newsgroup. */
    static int nextNewsgroupID;
};

#endif /* NEWSGROUP_H */
