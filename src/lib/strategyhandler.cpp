#include "strategyhandler.h"

const string strategyHandler::cc = "clang++";
const string strategyHandler::cflags = "-g -Wall -shared -rdynamic -fPIC";

const string strategyHandler::cpath = "/tmp/adam/cenv";

strategyHandler::strategyHandler(string stpath, string n) {
  strats_path = stpath;
  name = n;
  genetics_engine = NULL;
}

strategyHandler::strategyHandler(string stpath, string n, genetics* ge) {
  strats_path = stpath;
  name = n;
  genetics_engine = ge;
}

void strategyHandler::setGE(genetics* ge) {
  genetics_engine = ge;
}


int strategyHandler::prepareCompile() {

  mkdir("/tmp/adam",S_IRWXU);
  mkdir("/tmp/adam/cenv",S_IRWXU);

  string cfiles_cp_cmd = "cp compile/headers/* " + strategyHandler::cpath;
  system(cfiles_cp_cmd.c_str());

  cfiles_cp_cmd = "cp compile/obj/* " + strategyHandler::cpath;
  system(cfiles_cp_cmd.c_str());

  decorate();

  return 0;

}
    

int strategyHandler::include(string line,vector<string>* lines) {


    string iname = trim(line.replace(0,strlen("@strat_include"),""));
    ifstream fh (std::string(strats_path + "/" + iname).c_str());
    string iline;

    while(fh.good()){
      getline(fh,iline);
      lines->push_back(iline);
    }

    fh.close();
    return 0;

}


int strategyHandler::decorate() {

  std::vector<std::string> lines;
  std::string line;

  ifstream fh (std::string(strats_path + "/" + name).c_str());
  ofstream wh (std::string(strategyHandler::cpath + "/" + name + ".c").c_str());

  wh << "#include \"strats.h\"" << endl;  

  while(fh.good()){
    getline(fh,line);

    


    lines.push_back(line);
  }
  fh.close();

  for(int i=0;i<lines.size();i++) {

    wh << lines.at(i) <<endl; 

  }

  /*
  // the class factories
  extern "C" backend* create() {
      return new influxdb;
  }

  extern "C" void destroy(backend* p) {
      delete p;
  }
  */

  wh.close();

  return 0;

}



int strategyHandler::compile(int iter) {

  ostringstream oss;
  oss << iter;

  string ccmd = strategyHandler::cc + " " + strategyHandler::cflags + " *.o " + name + ".c -o " + name +  oss.str()  + ".so" ;

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


void* strategyHandler::resolveFunction(string indice,string type) {

  string symbol = indice + "_" + type; 
  return dlsym(handle, symbol.c_str());

}

void* strategyHandler::getTLFct() {
  return dlsym(handle,"TRADE_LIFE");
}


void* strategyHandler::getHandle() {
  return handle;
}

