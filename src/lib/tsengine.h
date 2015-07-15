#ifndef TSE_H
#define TSE_H

#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string>
#include "backends/backend.h"
#include "queue_c.h"

#include "adamcfg.h"
#include "assoc.h"
#include "broker.hpp"
#include "constants.h"
#include "narrays.h"
#include "moneymanager.h"
#include "igmlogger.h"
#include "genetics.h"
#include "indice.h"
#include "quant.h"

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "records.h"
#include "results.h"
#include "store.h"
#include "strategyhandler.hpp"
#include "tsemodule.hpp"
#include "tsexport.hpp"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <thread>
#include <quotek/cvector.hpp>
#include <quotek/cqueue.hpp>
#include <quotek/strategy.hpp>


using namespace rapidjson;

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
} evaluate_io;


class tsEngine{

  public:
    tsEngine();
    tsEngine(adamCfg*,
             broker*,
             backend*, 
             AssocArray<indice*>,
             strategyHandler*,
             moneyManager*,
             genetics*,
             vector<string>);

    broker* getBroker();
    backend* getBackend();
    ticks_t getTicks();
    quotek::data::cqueue <std::string>* getOrdersQueue();
    AssocArray<indice*> getIndicesList();
    strategyHandler* getStratHandler();
    moneyManager* getMoneyManager();
    genetics* getGE();

    AssocArray<records*>* getRecords();
    records* getIndiceRecords(string);
    int pushValues(string mepic,float v);
    int pushRecord(string,record*);

    adamCfg** getAdamConfig();

    AssocArray<void*>* getEvalPointers();
    igmLogger* getLogger();

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

    /** Creates a new object to expose some parts of the engine. */
    tsexport* eexport();

    //#### THREAD FUNCTIONS
    
    /**
     * Polling thread of adam.
     * This function periodically fetches the data from the configured broker
     */
    void poll();

    /**
     * Asset Evaluation threads.
     * This function is vital to adam: it is the function which is
     * called periodically (at each tick) to evaluate an asset and ultimately
     * takes a decision to take a position or not.
     */
    void evaluate(void*);

    void evaluate2(quotek::core::strategy* s);

    /**
     * This function is the orders execution threads callback.
     * Each call to the broker passes there.
     */
    void execute();

    /**
     * Money Management Thread.
     * This callback is the second most important in adam.
     * It runs the moneymanager: it ultimately decides to open a position
     * or not depending of the defined policy. It also computes the PNL 
     * of each position in real time.
     */
    void moneyman();

    void modulethread_wrapper();
    /**
     * aclock is the thread callback handling time
     * count in adam.
     */
    void aclock();
    
    /**
     * saveToBackend is the thread callback handling interactions
     * with backend and specially saving data. 
     */
    void saveToBackend();

    void broker_sync_start();


  protected:

    adamCfg* cfg;

    broker* tse_broker;
    ticks_t tse_ticks;
    int tse_inmem_history;
    backend* tse_back;
    strategyHandler* tse_strathandler;
    moneyManager* tse_mm;
    genetics* tse_ge;

    int bsp;

    AssocArray<indice*> indices_list;    

    quotek::data::cqueue<std::string> orders_queue;
    AssocArray<records*> inmem_records;

    AssocArray<void*> eval_ptrs;
    igmLogger* logger;
    vector<string> modules_list;
    store tse_store;
    store *tse_genes;

    std::thread* poller;
    std::thread* evaluator;
    std::thread* executor;
    std::thread* mmth;
    std::thread* clkth;
    std::thread* backioth;

    typedef struct eval_thread {
      tsEngine* engine;
      std::thread* th;
      void* eval_ptr;
      string eval_name;
    } eval_thread;

    vector<std::thread> modules_threads_list;
    vector<eval_thread> eval_threads;
    int uptime;
};


typedef struct eval_thread {

  tsEngine* engine;
  std::thread* th;
  void* eval_ptr;
  string eval_name;

} eval_thread;

#endif
