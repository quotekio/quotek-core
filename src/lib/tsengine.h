#ifndef TSE_H
#define TSE_H

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string>
#include "constants.h"
#include "adamcfg.h"
#include "brokers/broker.h"
#include "queue_c.h"

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "queue.h"
#include "assoc.h"
#include "utils.h"
#include "strategy.h"
#include "narrays.h"
#include "store.h"
#include "moneymanager.h"
#include "igmlogger.h"
#include "genetics.h"
#include "indice.h"
#include <iostream>
#include <fstream>

#define REMPOS_STOP 0x01
#define REMPOS_VSTOP 0x02
#define REMPOS_LIMIT 0x03



 void* poll(void*);
 void* poll_backtest(void*);

 void* evaluate(void*);
 void* execute(void*);
 void* moneyman(void*);
 void* modulethread_wrapper(void*);

using namespace rapidjson;

typedef struct eval_module_io {
      char mname[50];
      Queue_c* input;
      Queue_c* output;
    } eval_module_io;


typedef struct evmio_array {
  eval_module_io* evmio;
  size_t size;
} evmio_array;

typedef struct tradelife_io {
  char* ans;
  char* log_s;
  store* s;
  store* genes;
} tradelife_io;


typedef struct evaluate_io {
  const char* indice_name;
  farray* values;
  iarray* tstamps;
  char* ans;
  char* log_s;
  store* s;
  store* genes;
  struct evmio_array* evmio_a;
} evaluate_io;


class tsEngine{

  public:
    tsEngine(adamCfg*,
             broker*, 
             AssocArray<indice*>,
             strategy*,
             moneyManager*,
             genetics*,
             vector<string>);

    broker* getBroker();
    Queue <std::string>* getOrdersQueue();
    AssocArray<indice*> getIndicesList();
    iarray* getTimeStamps();
    strategy* getStrategy();
    moneyManager* getMoneyManager();
    genetics* getGE();

    farray* getValues(string);
    AssocArray<farray*>* getAllValues();
    int pushValues(string mepic,float v);
    int dumpAll();
    int loadDump();
    int loadDump(string);

    adamCfg* getAdamConfig();

    AssocArray<void*>* getEvalPointers();
    igmLogger* getLogger();
    evmio_array* getEVMIOArray();

    store* getStore();
    store* getGeneticsStore();
    void setGeneticsStore(store*);

    int getMode();
    float getSpeed();
    int getBacktestPos();
    int getBacktestProgress();

    void setBacktestPos(int);
    void setBacktestProgress(int);
    int eval_running(indice*,time_t);

  private:

    adamCfg* cfg;
    float tse_speed;
    int tse_mode;
    string tse_dump;
    int tse_backtest_pos;
    int tse_backtest_progress;

    evmio_array evmio_a;

    broker* tse_broker;
    strategy* tse_strat;
    moneyManager* tse_mm;
    genetics* tse_ge;

    AssocArray<indice*> indices_list;    

    Queue <std::string> orders_queue;
    iarray timestamps;
    AssocArray<farray*> values;
    AssocArray<void*> eval_ptrs;
    igmLogger* logger;
    vector<string> modules_list;
    store tse_store;
    store *tse_genes;

    pthread_t poller;
    pthread_t evaluator;
    pthread_t executor;
    pthread_t mmth;
    pthread_t bsync;
    pthread_t bfclose;

    //struct defined to pass data to modulethread_wrapper
    typedef struct module_initializer {
      tsEngine* engine;
      string name;
      Queue_c input;
      Queue_c output;
    } module_initializer;
 
    typedef struct eval_thread {
      tsEngine* engine;
      pthread_t th;
      void* eval_ptr;
      string eval_name;
    } eval_thread;


    vector<pthread_t> modules_threads_list;
    vector<module_initializer> modules_init_list;
    vector<eval_thread> eval_threads;

};

//struct defined to pass data to modulethread_wrapper
typedef struct module_initializer {
  tsEngine* engine;
  string name;
  Queue_c input;
  Queue_c output;
} module_initializer;

typedef struct eval_thread {

  tsEngine* engine;
  pthread_t th;
  void* eval_ptr;
  string eval_name;

} eval_thread;

typedef struct module_io {
  int mode;
  iarray** tstamps;
  AssocArray<farray*>* values;
  float* cur_pnl;
  float* cumulative_pnl;
  Queue_c* input;
  Queue_c* output; 

} module_io;


#endif