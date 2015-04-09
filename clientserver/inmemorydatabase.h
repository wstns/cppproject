#ifndef IN_MEMORY_DATABASE_H
#define IN_MEMORY_DATABASE_H

#include "newsgroup.h"
#include <vector>
#include "database.h"

class InMemoryDatabase : public Database {
public:
	InMemoryDatabase() = default;
    int addNewsgroup(Newsgroup ng) override;
    int removeNewsgroup(int id) override;
	/* Returns -1 if name is not found. */
	int findNewsgroup(const std::string &name) override;
	std::vector<Newsgroup> listNewsgroups() override;
	int addNewsgroup(const std::string &name) override { return addNewsgroup(Newsgroup(name)); }
	int addArticle(int ngID, const std::string &title, const std::string &author, const std::string &text) override;
	int listArticles(int ngID, std::vector<std::pair<int ,Article>> &vec) override;
	int deleteArticle(int ngID, int artID) override;
	Article getArticle(int ngID, int artID, int &check) override;

    const std::vector<Newsgroup> *getNewsgroups() const;
private:
    std::vector<Newsgroup> newsgroups;
};

#endif /* IN_MEMORY_DATABASE_H */
