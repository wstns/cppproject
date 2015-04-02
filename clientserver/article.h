#ifndef ARTICLE_H
#define ARTICLE_H

#include <iostream>
#include <string>

/* The class represents an article (duh) */
class Article {
public:
    Article() = default;
    /* Constructs an article with the given title, author and text. */
    Article(const std::string &tit, const std::string &auth, const std::string &txt) : title(tit), author(auth), text(txt) {}
    /* Prints the title and author on one row, followed by the text on the next row. */
    std::ostream &print(std::ostream &os) { return os << title << '\t' << "From: " << author << '\n' << text; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
	std::string getText() const { return text; }
private:
    std::string title;
    std::string author;
    std::string text;
};


#endif /* ARTICLE_H */
