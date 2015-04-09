#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

#include "newsgroup.h"
#include "utility.h"

class Database {
public:
	virtual ~Database() = default;
	/* Returns -1 if ng already exists. */
	virtual int addNewsgroup(Newsgroup ng) = 0;
	virtual int addNewsgroup(const std::string &name) = 0;
	/* Returns the ID of the newsgroup removed, or -1 if it wasn't found. */
	virtual int removeNewsgroup(util::id_type id) = 0;
	/* Returns the ID of the newsgroup with the given name, or -1 if not found. */
	virtual int findNewsgroup(const std::string &name) = 0;
	virtual std::vector<Newsgroup> listNewsgroups() = 0;
	/* Adds the article to the given newsgroup. Returns the ID of the newsgroup the article was added to, or -1 if the newsgroup could not be found. */
	virtual int addArticle(int ngID, const std::string &title, const std::string &author, const std::string &text) = 0;
	/* Returns -1 if the newsgroup wasn't found. Articles are added to the vector parameter. */
	virtual int listArticles(int ngID, std::vector<std::pair<int ,Article>> &vec) = 0;
	/* Same old, returns -1 if newsgroup does not exist and -2 if article does not exist in newsgroup. */
	virtual int deleteArticle(int ngID, int artID) = 0;
	virtual Article getArticle(int ngID, int artID, int &check) = 0;
};

#endif /* DATABASE_H */
