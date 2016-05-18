#include "qatecfg.h"

const string qateCfg::path = std::string(QATE_PREFIX) + "/etc/qate.conf";

qateCfg::qateCfg() {

  npath = "";
  mode = QATE_MODE_REAL;
  inmem_history = 0;
  ticks.getval = 1000000;
  ticks.eval = 1000000;
  broker_mode = "poll";
  strats_path = "strats";
  
  bt_exit = false;

}

string qateCfg::getBroker() {
  return broker;
}

string qateCfg::getBrokerParams() {
  return broker_params;
}

string qateCfg::getBackend() {
  return backend;
}

string qateCfg::getBackendParams() {
  return backend_params;
}

int qateCfg::getInMemHistory() {
  return inmem_history;
}

AssocArray<indice*> qateCfg::getIndicesList() {
  return indices_list;
}


vector<string> qateCfg::getModulesList() {
  return modules_list;
}


std::vector<std::string> qateCfg::getActiveStrats() {
  return active_strats;
}

string qateCfg::getStratsPath() {
  return strats_path;
}

mm_params* qateCfg::getMMP() {
  return &mmp;
}

genetics_params* qateCfg::getGP() {
  return &gp;
}

aep_params* qateCfg::getAEPP() {
  return &aepp;
}


std::string qateCfg::getBrokerMode() {
  return broker_mode;
}

int qateCfg::getMode() {
  return mode;
}


string qateCfg::getBTResultFile() {
  return backtest_result_path;
}


int qateCfg::getBFrom() {
  return backtest_from;
}

int qateCfg::getBTo() {
  return backtest_to;
}

ticks_t qateCfg::getTicks() {
  return ticks;
}

bool qateCfg::getBTExit() {
  return bt_exit;
}

void qateCfg::setMode(int md) {
  mode = md;
}

void qateCfg::setActiveStrats(std::vector<std::string> st) {
  active_strats = st;
}

void qateCfg::setBFrom(int from) {
  backtest_from = from;
}

void qateCfg::setBTo(int to) {
  backtest_to = to;  
}

void qateCfg::setAEPPort(int p) {
  aepp.listen_port = p;
}

void qateCfg::setBTResultFile(string f ) {
  backtest_result_path =  f;
}

void qateCfg::setStratsPath(std::string p) {
  strats_path = p;
}

void qateCfg::setBTExit(bool tv) {
  bt_exit = tv;
}


void qateCfg::chconf(char* np) {
  npath = std::string(np);
}

int qateCfg::read() {

  string line;

  vector<string> sline;
  string param;
  string arg;
  ifstream fh;

  if (npath == "") {
    fh.open(path.c_str());
  }

  else fh.open(npath.c_str()); 

  if (fh.is_open() ) {
    while(fh.good() ){
      getline(fh,line);

      if (line.find("=") != string::npos && line.find("#") != 0 ) {
        
        sline = split(line,'=');
        param = trim(sline.at(0));
        arg = trim(sline.at(1));

        if (param == "getval_ticks") {
          ticks.getval = atoi(arg.c_str());
          //Limits ticks to 1000000
          if (ticks.getval > 1000000) {
            printf("ERROR: getval ticks cannot be higher than one second");
            exit(1);
          }
        }
        
        else if (param == "eval_ticks") {
          ticks.eval = atoi(arg.c_str());
        }
        else if (param == "broker") broker = arg;
        else if (param == "broker_mode") broker_mode = arg;

        else if (param == "broker_params") broker_params = arg;
        else if (param == "backend") backend = arg;
        else if (param == "backend_params") backend_params = arg;

        else if (param == "inmem_history") inmem_history = atoi(arg.c_str());

        else if (param == "indice") {
          vector<string> ilist = split(arg,' ');
          if (ilist.size() == 7) {
            indices_list[ilist.at(3)] = new indice(ilist[0], 
                                                   atoi(ilist[1].c_str()),
                                                   atoi(ilist[2].c_str()),
                                                   ilist[3],
                                                   atof(ilist[4].c_str()),
                                                   ilist[5],
                                                   ilist[6]);
          }
        }

        else if (param == "modules") { 
          modules_list = split(arg,',');
          for (int i=0;i<modules_list.size();i++) {
            modules_list[i] = trim(modules_list[i]);
          }
        }
        
        else if (param == "active_strats") {
          active_strats = split(arg,',');
          for (int i=0;i<active_strats.size();i++) {
            active_strats[i] = trim(active_strats[i]);
          }
        }

        else if (param == "strats_path") strats_path = arg;
        else if (param == "mm_capital") mmp.mm_capital = atof(arg.c_str());
        else if (param == "mm_max_openpos") mmp.mm_max_openpos = atoi(arg.c_str());
        else if (param == "mm_max_openpos_per_epic") mmp.mm_max_openpos_per_epic = atoi(arg.c_str());
        else if (param == "mm_max_loss_percentage_per_trade" ) mmp.mm_max_loss_percentage_per_trade = atof(arg.c_str());
        else if (param == "mm_critical_loss_percentage" ) mmp.mm_critical_loss_percentage = atof(arg.c_str());
        else if (param == "mm_max_var") mmp.mm_max_var = atof(arg.c_str()); 
        else if (param == "mm_reverse_pos_lock") mmp.mm_reverse_pos_lock = atoi(arg.c_str());
        else if (param == "mm_reverse_pos_force_close") mmp.mm_reverse_pos_force_close = atoi(arg.c_str());

        else if (param == "genetics_population") gp.genetics_population = atoi(arg.c_str());        
        else if (param == "genetics_survivors") gp.genetics_survivors = atoi(arg.c_str());
        else if (param == "genetics_children") gp.genetics_children = atoi(arg.c_str());
        else if (param == "genetics_newcomers") gp.genetics_newcomers = atoi(arg.c_str());
        else if (param == "genetics_mutable_genes") gp.genetics_mutable_genes = atoi(arg.c_str());
        else if (param == "genetics_converge_thold") gp.genetics_converge_thold = atof(arg.c_str());
        else if (param == "genetics_max_generations") gp.genetics_max_generations = atoi(arg.c_str());
        else if (param == "genetics_recompute_winners") gp.genetics_recompute_winners = atoi(arg.c_str());

        else if (param == "aep_enable") aepp.enable = atoi(arg.c_str());
        else if (param == "aep_listen_addr") aepp.listen_addr = arg;
        else if (param == "aep_listen_port") aepp.listen_port = atoi(arg.c_str());

        
      }

    }
  }

  else {
    cout << "ERROR: Unable to open qate config file" << endl;
    exit(0);
  }
  return 0;
}
