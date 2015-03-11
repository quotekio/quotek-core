#ifndef TSE_H
#define TSE_H

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string>
#include "constants.h"
#include "adamcfg.h"
#include "broker.hpp"
#include "backends/backend.h"
#include "records.h"
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
#include "results.h"
#include "genetics.h"
#include "indice.h"
#include "quant.h"
#include <iostream>
#include <fstream>

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
  Queue_c* orders;
  Queue_c* logs;
  store* s;
  store* genes;
} tradelife_io;


typedef struct evaluate_io {
  const char* indice_name;
  records* recs;
  Queue_c* orders;
  Queue_c* logs;
  store* s;
  store* genes;
  int state;
  struct evmio_array* evmio_a;
} evaluate_io;


class tsEngine{

  public:
    tsEngine();
    tsEngine(adamCfg*,
             broker*,
             backend*, 
             AssocArray<indice*>,
             strategy*,
             moneyManager*,
             genetics*,
             vector<string>);

    broker* getBroker();
    backend* getBackend();
    ticks_t getTicks();
    Queue <std::string>* getOrdersQueue();
    AssocArray<indice*> getIndicesList();
    strategy* getStrategy();
    moneyManager* getMoneyManager();
    genetics* getGE();

    AssocArray<records*>* getRecords();
    records* getIndiceRecords(string);
    int pushValues(string mepic,float v);
    int pushRecord(string,record*);

    adamCfg** getAdamConfig();

    AssocArray<void*>* getEvalPointers();
    igmLogger* getLogger();
    evmio_array* getEVMIOArray();

    store* getStore();
    store* getGeneticsStore();
    void setGeneticsStore(store*);

    int loadHistory();
    
    int eval_running(indice*,time_t);
    
    int getUptime();
    void tick();

    int getBSP();
    void setBSP(int);

    void openPosition(string epic, string way, int nbc, int stop, int limit);
    void closePosition(string dealid);


    //#### STATIC,THREADED FUNCTIONS
    static void* poll(void*);
    static void* evaluate(void*);
    static void* execute(void*);
    static void* moneyman(void*);
    static void* modulethread_wrapper(void*);
    static void* aclock(void*);
    static void* saveToBackend(void*);

  protected:

    adamCfg* cfg;
    evmio_array evmio_a;

    broker* tse_broker;
    ticks_t tse_ticks;
    int tse_inmem_history;
    backend* tse_back;
    strategy* tse_strat;
    moneyManager* tse_mm;
    genetics* tse_ge;

    int bsp;

    AssocArray<indice*> indices_list;    

    Queue <std::string> orders_queue;
    AssocArray<records*> inmem_records;


    AssocArray<void*> eval_ptrs;
    igmLogger* logger;
    vector<string> modules_list;
    store tse_store;
    store *tse_genes;

    pthread_t poller;
    pthread_t evaluator;
    pthread_t executor;
    pthread_t mmth;
    pthread_t clkth;
    pthread_t backioth;

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

    int uptime;

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
  float* cur_pnl;
  float* cumulative_pnl;
  Queue_c* input;
  Queue_c* output;
  store* s;
} module_io;



#endif
