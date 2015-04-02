#ifndef ARTICLE_H
#define ARTICLE_H

#include <iostream>
#include <string>

/* The class represents an article (duh) */
class Article {
public:
    Article() = default;
    /* Constructs an article with the given title, author and text. */
    Article(std::string tit, std::string auth, std::string txt) : title(tit), author(auth), text(txt) {}
    /* Prints the title and author on one row, followed by the text on the next row. */
    friend std::ostream &operator<<(std::ostream &os, const Article &article);
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
private:
    std::string title;
    std::string author;
    std::string text;
};

#if 0
std::ostream &operator<<(std::ostream &os, const Article &article)
{
    return os << article.title << '\t' << "From: " << article.author << '\n' << article.text;
}
#endif


#endif /* ARTICLE_H */
