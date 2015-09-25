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


/**
 * Class strategy handler takes care of source decoration, compiling, etc..
 */

class strategyHandler {

  public:

    strategyHandler(string stpath, string name);
    strategyHandler(string stpath, string name, genetics* ge);
    void setGE(genetics* ge);
    int prepareCompile();
    int preprocess();
    int compile(int);
    int dlibOpen(int);
    void* getExportFct();
    void* getHandle();
    std::string getAssetMatch();

    static const string cc;
    static const string cflags;
    static const string dependencies;
    static const string cpath;

  private:

    void* handle;
    std::string name;
    std::string classname;
    std::string asset_match;
    string strats_path;
    genetics* genetics_engine;

};

#endif
