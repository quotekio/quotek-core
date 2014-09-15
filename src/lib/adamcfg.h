#ifndef ADAMCFG_H
#define ADAMCFG_H

#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include "utils.h"
#include "assoc.h"
#include "indice.h"
#include "constants.h"


using namespace std;

typedef struct mm_params {
  float mm_capital;
  int mm_max_openpos;
  int mm_max_openpos_per_epic;
  float mm_max_loss_percentage_per_trade;
  float mm_critical_loss_percentage;
  float mm_max_var;
  int mm_reverse_pos_lock;
  int mm_reverse_pos_force_close;

} mm_params;

typedef struct aep_params {
  int enable;
  string listen_addr;
  int listen_port;

} aep_params;


typedef struct genetics_params {

  int genetics_population;
  int genetics_children;
  int genetics_survivors;
  int genetics_mutable_genes;
  int genetics_newcomers;
  float genetics_converge_thold;
  int genetics_max_generations;
  int genetics_recompute_winners;

} genetics_params;


class adamCfg {

  private:

    int ticks;
    string broker;
    string backend;
    string backend_params;
    int inmem_history;
    AssocArray<indice*> indices_list;
    string strat;
    vector<string> modules_list;
    mm_params mmp;
    genetics_params gp;
    aep_params aepp;
    int mode;
    int backtest_speed;
    string backtest_result_path;
    string backtest_dump_path;

  public:

    static const string path;
    adamCfg();
    int read();

    /* getters */

    string getBroker();
    string getBackend();
    string getBackendParams();
    AssocArray<indice*> getIndicesList();
    vector<string> getModulesList();
    mm_params* getMMP();
    genetics_params* getGP();
    aep_params* getAEPP();
    string getStrat();
    
    int getMode();
    int getBSpeed();
    string getBDump();
    string getBTResultFile();
    void chconf(char*);
    int getInMemHistory();

    /* Setters */

    void setMode(int);
    void setStrat(string);

    void setAEPPort(int);

    void setBDump(string);
    void setBSpeed(int);
    void setBTResultFile(string);


  private:

    string npath;

};

#endif

