#include <stdlib.h>
#include <stdio.h> /* defines FILENAME_MAX */
#include <iostream>
#include "filesystemdatabase.h"
#include <unistd.h>

using namespace std;

int main(){
  char cwd[FILENAME_MAX];
  if (!getcwd(cwd, sizeof(cwd))) return errno;
  cout<<"cwd: "<<cwd<<endl;


  FileSystemDatabase db("test/db");

  std::vector<std::pair<int ,Article>> artcles;
  db.listArticles(1, artcles);
}
