#ifndef IN_MEMORY_DATABASE_H
#define IN_MEMORY_DATABASE_H

#include <set>
#include "database.h"
#include "newsgroup.h"

class InMemoryDatabase : public Database {
public:
	InMemoryDatabase() = default;
    void addNewsgroup(Newsgroup ng) override { newsgroups.insert(ng); }
    int removeNewsgroup(util::id_type id) override;
	/* Returns -1 if name is not found. */
	int findNewsgroup(const std::string &name) override;
	std::vector<Newsgroup> listNewsgroups() override;
	void addNewsgroup(const std::string &name) override { addNewsgroup(Newsgroup(name)); }
	int addArticle(int ngID, const std::string &title, const std::string &author, const std::string text) override;
	int listArticles(int ngID, std::vector<std::pair<int ,Article>> &vec) override;

    const std::set<Newsgroup> *getNewsgroups() const;
private:
    std::set<Newsgroup> newsgroups;
};

#endif /* IN_MEMORY_DATABASE_H */
