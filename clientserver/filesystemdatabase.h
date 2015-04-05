#ifndef FILE_SYSTEM_DATABASE_H
#define FILE_SYSTEM_DATABASE_H

#include "newsgroup.h"
#include <vector>
#include "database.h"

class FileSystemDatabase : public Database {
public:
	FileDatabase(string file);
    void addNewsgroup(Newsgroup ng) override;
    int removeNewsgroup(int id) override;
	/* Returns -1 if name is not found. */
	int findNewsgroup(const std::string &name) override;
	std::vector<Newsgroup> listNewsgroups() override;
	void addNewsgroup(const std::string &name) override { addNewsgroup(Newsgroup(name)); }
	int addArticle(int ngID, const std::string &title, const std::string &author, const std::string &text) override;
	int listArticles(int ngID, std::vector<std::pair<int ,Article>> &vec) override;
	int deleteArticle(int ngID, int artID) override;
	Article getArticle(int ngID, int artID, int &check) override;

    const std::vector<Newsgroup> *getNewsgroups() const;
private:
	string datafile
    std::vector<Newsgroup> newsgroups;
};

#endif /* FILE_SYSTEM_DATABASE_H */
