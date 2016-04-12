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

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
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
#include <quotek/quant.hpp>
#include <quotek/record.hpp>
#include <quotek/strategy.hpp>



using namespace rapidjson;

typedef struct algo {
      std::thread* th;
      void* eval_ptr;
      string eval_name;
      string strategy;
      float pnl;
} algo;

typedef struct savepos {
  int saved;
  int size;
} savepos ;


class tsEngine{

  public:

    tsEngine();
    tsEngine(adamCfg*,
             broker*,
             backend*, 
             AssocArray<indice*>,
             std::vector<strategyHandler*>,
             moneyManager*,
             genetics*,
             vector<string>);

    void init_finalize(adamCfg* conf);

    broker* getBroker();
    backend* getBackend();
    ticks_t getTicks();
    quotek::data::cqueue <std::string>* getOrdersQueue();
    AssocArray<indice*> getIndicesList();
    std::vector<strategyHandler*> getStratHandlers();
    moneyManager* getMoneyManager();
    genetics* getGE();

    AssocArray<quotek::data::records>& getRecords();
    quotek::data::records& getAssetRecords(string);

    int pushValues(string mepic,float v);
    int pushRecord(string,quotek::data::record& r);

    adamCfg** getAdamConfig();

    AssocArray<void*>* getEvalPointers();
    std::vector<algo> getAlgos();
    igmLogger* getLogger();

    std::map<std::string, quotek::data::any>& getStore();
    store* getGeneticsStore();
    void setGeneticsStore(store*);

    int loadHistory();
    
    int eval_running(indice*,time_t);
    
    int getUptime();
    void tick();

    int getBSP();
    void setBSP(int);

    void openPosition(string id, string epic, string way, int nbc, int stop, int limit);
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
    //void evaluate(void*);

    void evaluate(strategy* s);

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

    /**
     * saveToBackend2 is an alternate thread callback to store variables in backend
     */
    void saveToBackend2();


    void broker_sync_start();


  protected:

    adamCfg* cfg;

    broker* tse_broker;
    ticks_t tse_ticks;
    int tse_inmem_history;
    backend* tse_back;
    std::vector<strategyHandler*> tse_strathandlers;
    moneyManager* tse_mm;
    genetics* tse_ge;

    int bsp;

    AssocArray<indice*> indices_list;    

    quotek::data::cqueue<std::string> orders_queue;
    AssocArray<quotek::data::records> inmem_records;

    //Stores the algo data to be saved in backend.
    quotek::data::cqueue<std::string> save_queue;

    AssocArray<void*> eval_ptrs;
    igmLogger* logger;
    vector<string> modules_list;

    

    std::map<std::string, quotek::data::any> tse_store;

    std::map<std::string, savepos> save_positions;

    store *tse_genes;

    std::thread* poller;
    std::thread* evaluator;
    std::thread* executor;
    std::thread* mmth;
    std::thread* clkth;
    std::thread* backioth;
    std::thread* backsaveth;
    
    vector<std::thread> modules_threads_list;
    vector<algo> algos;

    vector<strategy*> spool;

    int uptime;
};

#endif
