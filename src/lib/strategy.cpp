#include "strategy.h"

const string strategy::cc = "clang";
const string strategy::cflags = "-g -Wall -shared -rdynamic -fPIC";

//const string strategy::cc = "gcc";
//const string strategy::cflags = "-shared -rdynamic -fPIC";

const string strategy::cpath = "/tmp/adam/cenv";

strategy::strategy(string n) {
  name = n;
  genetics_engine = NULL;

}

strategy::strategy(string n, genetics* ge) {
  name = n;
  genetics_engine = ge;
}

void strategy::setGE(genetics* ge) {
  genetics_engine = ge;
}


int strategy::prepareCompile() {

  mkdir("/tmp/adam",S_IRWXU);
  mkdir("/tmp/adam/cenv",S_IRWXU);

  string cfiles_cp_cmd = "cp compile/headers/* " + strategy::cpath;
  system(cfiles_cp_cmd.c_str());

  cfiles_cp_cmd = "cp compile/obj/* " + strategy::cpath;
  system(cfiles_cp_cmd.c_str());

  decorate();

  return 0;

}
    

int strategy::include(string line,vector<string>* lines) {


    string iname = trim(line.replace(0,strlen("@strat_include"),""));
    ifstream fh (std::string("strats/" + iname + ".ts").c_str());
    string iline;

    while(fh.good()){
      getline(fh,iline);
      lines->push_back(iline);
    }

    fh.close();
    return 0;

}


int strategy::decorate() {

  std::vector<std::string> deco;
  std::vector<std::string> lines;
  std::string line;

  std::regex subeval ("subeval\\(\"(.*?)\"\\)");

  deco.push_back("_EVAL()");
  deco.push_back("_EVAL(uint32_t t, float v, evaluate_io* io)");
  deco.push_back("_SUBEVAL()");
  deco.push_back("_SUBEVAL(uint32_t t, float v, evaluate_io* io)");  
  deco.push_back("TRADE_LIFE()");
  deco.push_back("TRADE_LIFE(position* pos, tradelife_io* io)");

  

  ifstream fh (std::string("strats/" + name + ".ts").c_str());
  ofstream wh (std::string(strategy::cpath + "/" + name + ".c").c_str());

  wh << "#include \"strats.h\"" << endl;  

  while(fh.good()){
    getline(fh,line);

    if (  line.find("@strat_include") != string::npos ) {
      include(line,&lines);
      line = "";
    }
    
    if ( line.find("@gene") != string::npos ) {
      if (genetics_engine != NULL ) {
        genetics_engine->parseGene(line);
      }
      line = "";

    }


    line = std::regex_replace(line,subeval,std::string("$1_SUBEVAL(t,v,io)")) ;
    for (int i=0;i<deco.size()-1;i=i+2) {
      line = sreplace(line,deco.at(i),deco.at(i+1));
    }
    lines.push_back(line);
  }
  fh.close();

  for(int i=0;i<lines.size();i++) {

    wh << lines.at(i) <<endl; 

  }

  wh.close();

  return 0;

}



int strategy::compile(int iter) {

  ostringstream oss;
  oss << iter;

  string ccmd = strategy::cc + " " + strategy::cflags + " *.o " + name + ".c -o " + name +  oss.str()  + ".so" ;

  chdir(strategy::cpath.c_str()); 
  system(ccmd.c_str());

  return 0;

}

int strategy::dlibOpen(int iter) {

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


void* strategy::resolveFunction(string indice,string type) {

  string symbol = indice + "_" + type; 
  return dlsym(handle, symbol.c_str());

}

void* strategy::getTLFct() {
  return dlsym(handle,"TRADE_LIFE");
}


void* strategy::getHandle() {
  return handle;
}

