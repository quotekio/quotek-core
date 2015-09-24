#include "strategyhandler.hpp"

const string strategyHandler::cc = "clang++";
const string strategyHandler::cflags = "-std=c++11 -stdlib=libc++ -g -shared -rdynamic -fPIC -lcurl -lpthread -lquotek";
const string strategyHandler::cpath = "/tmp/adam/cenv";

strategyHandler::strategyHandler(string stpath, string n) {
  strats_path = stpath;
  name = n;
  classname = n;
  asset_match = "(.*)";
  genetics_engine = NULL;

}

strategyHandler::strategyHandler(string stpath, string n, genetics* ge) {
  strats_path = stpath;
  name = n;
  classname = n;
  asset_match = "(.*)";
  genetics_engine = ge;
}

void strategyHandler::setGE(genetics* ge) {
  genetics_engine = ge;
}

int strategyHandler::prepareCompile() {

  mkdir("/tmp/adam",S_IRWXU);
  mkdir("/tmp/adam/cenv",S_IRWXU);

  preprocess();

  return 0;

}
    
int strategyHandler::preprocess() {

  std::vector<std::string> lines;
  std::string line;

  ifstream fh (std::string(strats_path + "/" + name).c_str());
  ofstream wh (std::string(strategyHandler::cpath + "/" + name + ".cpp").c_str());

  std::regex classname_regex("^class(.*)");
  std::regex asset_match_regex("^\\/\\/\\#asset_match(.*)");
  std::regex strat_include_regex("^\\/\\/\\#strat_include(.*)");

  wh << "#include <quotek/quotek.hpp>\n";

  while(fh.good()){
    getline(fh,line);

    lines.emplace_back(line);

    //finding strategy class name.
    if ( std::regex_match(line,classname_regex) ) {

      this->classname = line.replace(line.find("class"),strlen("class"),"");
      this->classname = line.replace(line.find(": public strategy {"),
                                      strlen(": public strategy {"),
                                      "");
      trim(this->classname);
      std::cout << "Found usable class in Strategy: " << this->classname << std::endl;
    }

    //finding strategy matching regex.
    else if ( std::regex_match(line,asset_match_regex) ) {
      this->asset_match = line.replace(line.find("//#asset_match"),strlen("//#asset_match"),"");
      trim(this->asset_match);
      std::cout << "Found asset matching regex in Strategy: " << this->asset_match << std::endl;
    }

  }

  for (int i=0;i<lines.size();i++) {
    wh << lines[i] << std::endl ;
  }

  // the class factories
  wh << "extern \"C\" strategy* create(quotek::data::records& recs,\nstd::map<std::string, quotek::data::any>& store,\nstd::vector<quotek::core::position>& portfolio) {\n";
  wh << "\treturn new " << classname << "(recs,store,portfolio);\n";
  wh << "}\n";

  wh << "extern \"C\" void destroy(strategy* s) {\n";
  wh <<  "\tdelete s;\n";
  wh << "}\n";

  wh.close();

  return 0;

}

int strategyHandler::compile(int iter) {

  ostringstream oss;
  oss << iter;

  string ccmd = strategyHandler::cc + " " + strategyHandler::cflags + " " + name + ".cpp -o " + name +  oss.str()  + ".so" ;

  chdir(strategyHandler::cpath.c_str()); 
  system(ccmd.c_str());

  return 0;

}

int strategyHandler::dlibOpen(int iter) {

  ostringstream oss;
  oss << iter;

  string libname = "./" + name + oss.str() + ".so";

  handle = dlopen(libname.c_str(),RTLD_LAZY);
  if (!handle) {

    cout << dlerror() << endl;
    exit(1);

  }

  return 0;

}

void* strategyHandler::getExportFct() {
  return dlsym(handle,"create");
}

void* strategyHandler::getHandle() {
  return handle;
}

std::string strategyHandler::getAssetMatch() {
  return asset_match;
}
