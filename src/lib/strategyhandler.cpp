#include "strategyhandler.hpp"

const string strategyHandler::cc = "clang++";
const string strategyHandler::cflags = "-std=c++11 -stdlib=libc++ -g -shared -rdynamic -fPIC";
const string strategyHandler::dependencies = "-lcurl -lpthread -lquotek";
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
  std::regex ex_eval_regex ("(.*)\\/\\/#ex_eval(.*)");

  wh << "#include <quotek/quotek.hpp>\n";
  wh << "#include <unistd.h>\n";

  //functions/statements blacklisting
  std::string randfct = "BL" + randstring(32);
  wh << "#define system(a_) " << randfct << "(a_)" << std::endl;
  wh << "#define popen(a_,b_) " << randfct << "(a_,b_)" << std::endl;
  wh << "#define fork() " << randfct << "()" << std::endl;
  wh << "#define exec(a_) " << randfct << "(a_)" << std::endl;
  wh << "#define asm(a_) " << randfct << "(a_)" << std::endl;
  wh << "#define __asm__(a_) " << randfct << "(a_)" << std::endl;
  wh << "#define volatile(a_) " << randfct << "(a_)" << std::endl;
  wh << "#define __volatile__(a_) " << randfct << "(a_)" << std::endl;

  wh << "#define opendir(a_) " << randfct << "(a_)" << std::endl;
  wh << "#define fdopendir(a_) " << randfct << "(a_)" << std::endl;

  wh << "#define readdir(a_) " << randfct << "(a_)" << std::endl;
  wh << "#define readdir64(a_) " << randfct << "(a_)" << std::endl;
  wh << "#define readdir_r(a_,b_,c_) " << randfct << "(a_,b_,c_)" << std::endl;
  wh << "#define readdir64_r(a_,b_,c_) " << randfct << "(a_,b_,c_)" << std::endl;
  wh << "#define scandir(a_,b_,c_,d_) " << randfct << "(a_,b_,c_,d_)" << std::endl;
  wh << "#define stat(a_, b_) " << randfct << "(a_,b_)" << std::endl;
  wh << "#define lstat(a_, b_) " << randfct << "(a_,b_)" << std::endl;
  
  wh << "#define execve " << randfct  << std::endl;
  wh << "#define execl " << randfct  << std::endl;
  wh << "#define execlp" << randfct  << std::endl;
  wh << "#define execle " << randfct  << std::endl;
  wh << "#define execv " << randfct  << std::endl;
  wh << "#define execvp " << randfct  << std::endl;

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

    else if ( std::regex_match(line,ex_eval_regex) ) {
      stringstream ss;
      int period;
      std::string fct;
      std::string direc;     
      ss << line;
      ss >> direc;
      ss >> period;
      ss >> fct;

      stringstream ss2;
      ss2 << "new std::thread( [&] {  while(1) { sleep(" << period << ");" << fct << "; }});";

      lines.pop_back();
      lines.emplace_back(ss2.str());

    }


  }

  for (int i=0;i<lines.size();i++) {
    wh << lines[i] << std::endl ;
  }

  // the class factories
  wh << "extern \"C\" strategy* create() {\n";
  wh << "\treturn new " << classname << "();\n";
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

  string ccmd = strategyHandler::cc + 
                " " + strategyHandler::cflags + " " + 
                name + ".cpp -o " + name +  oss.str()  + ".so" + 
                " " + strategyHandler::dependencies + 
                " 2>/tmp/adam/compiler.errors.log";
  
  chdir(strategyHandler::cpath.c_str()); 
  system("> /tmp/adam/compiler.errors.log");
  system(ccmd.c_str());

  struct stat errstatus;
  stat("/tmp/adam/compiler.errors.log", &errstatus );
  
  return errstatus.st_size;
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

std::string strategyHandler::getName() {
  return name;
}