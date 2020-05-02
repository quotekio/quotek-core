#ifndef QATECFG_H
#define QATECFG_H

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include "utils.h"
#include "assoc.h"
#include "indice.h"
#include "constants.h"
#include <n3rv/n3rvlogger.hpp>



using namespace std;

typedef struct mm_params {
  float mm_capital;
  float safe_rate;
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

typedef struct ticks_t {

  int getval;
  int eval;

} ticks_t;



class qateCfg {

  private:

    ticks_t ticks;

    string broker;
    string broker_params;
    string broker_mode;
    string backend;
    string backend_params;
    int inmem_history;
    AssocArray<indice*> indices_list;
    std::vector<std::string> active_strats;
    string strats_path;
    vector<string> modules_list;
    mm_params mmp;
    genetics_params gp;
    aep_params aepp;
    int mode;
    int backtest_speed;
    int backtest_from;
    int backtest_to;
    
    string backtest_result_path;

    bool bt_exit;

    string redis_host;
    int redis_port;



  public:

    static const string path;
    qateCfg();
    int read();

    /* getters */

  
    string getBroker();
    string getBrokerParams();
    string getBackend();
    string getBackendParams();
    AssocArray<indice*> getIndicesList();
    vector<string> getModulesList();
    mm_params* getMMP();
    genetics_params* getGP();
    aep_params* getAEPP();
    std::vector<std::string> getActiveStrats();
    string getStratsPath();

    int getBFrom();
    int getBTo();
    int getMode();
    string getBrokerMode();
    int getBSpeed();
    ticks_t getTicks();
    string getBDump();
    string getBTResultFile();
    void chconf(char*);
    int getInMemHistory();

    bool getBTExit();

    string getRedisHost();
    int getRedisPort();

    /* Setters */

    void setMode(int);
    void setActiveStrats(std::vector<std::string>);

    void setAEPPort(int);

    void setBDump(string);
    void setBSpeed(int);
    void setBTResultFile(string);
    void setBFrom(int);
    void setBTo(int);

    void setStratsPath(std::string);

    void setBTExit(bool tv); 

    n3rv::logger* ll;


  private:

    string npath;

};

#endif

