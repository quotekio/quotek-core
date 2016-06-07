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
#include "btf.hpp"
#include <quotek/utils.hpp> 

#include <regex>


using namespace std;


/**
 * Class strategy handler takes care of source decoration, compiling, etc..
 */

class strategyHandler {

  public:

    strategyHandler(string stpath, string name);
    int prepareCompile();

    std::string import_module(std::string module_name);
    int preprocess();
    int preprocess_cpp();
    int preprocess_python();
    int compile(int);
    int compile_python(int);
    int compile_cpp(int);
    int dlibOpen(int);
    void* getExportFct();
    void* getHandle();
    std::string getAssetMatch();
    std::string getName();

    std::vector< std::vector<std::string> > getBatchDirectives();
    std::vector< std::vector<std::string> > getGeneDirectives();
    btf getBTFilter();


    
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
    string language;

    std::vector< std::vector<std::string> > batch_directives;
    std::vector< std::vector<std::string> > gene_directives;

    btf btfilt;

};

#endif
