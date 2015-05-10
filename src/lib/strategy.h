#ifndef STRAT_H
#define STRAT_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "utils.h"
#include "genetics.h"
#include <regex>


using namespace std;


class strategy {

  public:

    strategy(string stpath, string name);
    strategy(string stpath, string name,genetics* ge);
    void setGE(genetics* ge);
    int prepareCompile();
    int compile(int);
    int dlibOpen(int);
    void* resolveFunction(string,string);
    void* getTLFct();
    void* getHandle();
    int include(string,vector<string>*);
    int decorate();

    static const string cc;
    static const string cflags;
    static const string cpath;

  private:

    void* handle;
    string name;
    string strats_path;
    genetics* genetics_engine;

};

#endif
