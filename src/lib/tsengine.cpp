#include "tsengine.h"


void* tsEngine::modulethread_wrapper(void* arg) {

  if (!arg) {
    cout << "Fatal error: Null pointer as module argument" << endl;
    exit(1);
  }


  typedef void* (*fctptr)(module_io);
  
  module_initializer* mi = (module_initializer*) arg;

  tsEngine* t0 = mi->engine;
  moneyManager* mm = t0->getMoneyManager();

  string module_so = "lib" + mi->name + ".so";
  module_io mio;
  mio.mode = ADAM_MODE_REAL;
  mio.logger = t0->getLogger();
  mio.cur_pnl = mm->getCurPNL();
  mio.cumulative_pnl = mm->getCumulativePNL();

  mio.input = &(mi->input);
  mio.output = &(mi->output);
  
  chdir(ADAM_PREFIX);
  void* handle = dlopen(module_so.c_str(),RTLD_LAZY);
  
  if (!handle) {
    cerr << dlerror() << endl;
    exit(1);
  }
  void* symbol = dlsym(handle,"module");

  if (!symbol) {
    cerr << dlerror() << endl;
    exit(1);
  }
  
  fctptr f = (fctptr) symbol;
  (*f)(mio);

  return NULL;

} 


void* broker_pos_sync(void* arg) {

  tsEngine* t0 = (tsEngine*) arg;
  moneyManager* mm = t0->getMoneyManager();
  broker* b0 = t0->getBroker();
  ticks_t ticks = t0->getTicks();
  vector<position>* poslist = mm->getPositions();
  igmLogger* logger = t0->getLogger();

  AssocArray<indice*> ilist = t0->getIndicesList();
  vector<string> si = iGetNames(ilist);

  indice* idx;

  //pos variables
  string epic;
  string indice;
  string dealid;
  string name;

  float stop;
  float limit;
  float open;
  int size;

  vector<bpex> mypos;
  vector<string> alive_pos;

  while(1) { 

    //perf profiling
    auto tt0 = std::chrono::high_resolution_clock::now();

    mypos = b0->getPositions();
    if (mypos.size() != 0) {

      alive_pos.clear();
      for (int i=0;i<mypos.size();i++) {
      
        idx = iResolve(ilist,mypos[i].epic);

        if (idx != NULL) {
          
          epic = mypos[i].epic;
          indice = idx->name;
          dealid = mypos[i].dealid;
          size = mypos[i].size;
          stop = mypos[i].stop;
          limit = mypos[i].limit;
          open = mypos[i].open;
          
          alive_pos.push_back(dealid);

          if (! mm->hasPos(dealid) ) {
            logger->log("New position found:" + dealid);       
   
            position p;
            p.epic = epic;
            p.indice = indice;
            p.dealid = dealid;
            p.name = name;
            p.open = open;
            p.stop = stop;
            p.vstop = stop;
            p.vlimit = limit;
            p.nb_inc = 1;
            p.limit = limit;
            p.size = size;
            p.pnl = 0;
            p.status = POS_OPEN;
            mm->addPosition(p);
 
          }    
        }
      }

      //removal of closed pos
      if (alive_pos.size() < poslist->size() ) {
        logger->log("Cleaning of closed positions");
        mm->cleanPositions(alive_pos);
      }
    }

    auto tt1 = std::chrono::high_resolution_clock::now();
    auto elapsed_t = tt1 - tt0;
    uint64_t elapsed = std::chrono::duration_cast<std::chrono::microseconds>(elapsed_t).count();

    if (elapsed < ticks.getpos) {  
      usleep(ticks.getpos - elapsed);
    }

  }
}

/*
void* broker_force_close(void* arg) {

  tsEngine* t0 = (tsEngine*) arg;
  moneyManager* mm = t0->getMoneyManager();
  broker* b0 = t0->getBroker();
  vector<position>* poslist = mm->getPositions();
  Queue<std::string> *orders_queue = t0->getOrdersQueue();

  while(1) {
    for (int i=0;i<poslist->size();i++) {
      if (poslist->at(i).status == POS_PENDING_CLOSE) {
        orders_queue->push("closepos:" + poslist->at(i).dealid );
      }
    }
    sleep(5);
  }
}
*/


void* tsEngine::aclock(void* arg) {
  tsEngine *t0 = (tsEngine*) arg;

  while(1) {
    t0->tick();
    sleep(1);
  }
}

void* tsEngine::moneyman(void* arg) {

  tsEngine *t0 = (tsEngine*) arg;
  broker* b0 = t0->getBroker();
  moneyManager* mm = t0->getMoneyManager();
  AssocArray<indice*> ilist = t0->getIndicesList();
  vector<string> si = iGetNames(ilist);
  igmLogger* logger = t0->getLogger();
  Queue<std::string> *orders_queue = t0->getOrdersQueue();
  strategy* st = t0->getStrategy();

  //TRADELIFE struct for fctptr
  typedef void* (*tl_fct)(pos_c*,tradelife_io*);
  void* tl_fct_fref = st->getTLFct(); 
  vector<position>* poslist = mm->getPositions();

  //pnl-needed vars
  float v;

  tradelife_io tl_io;
  
  int inc = 0;

  tl_io.ans = (char*) malloc(1024* sizeof(char));
  tl_io.log_s = (char*) malloc(1024* sizeof(char));
  tl_io.s = t0->getStore();

  while (1) {

    //plays tradelife callback inside strategy.
    if ( tl_fct_fref != NULL) {

      tl_fct tl = (tl_fct) tl_fct_fref;

      for (int i=0;i<poslist->size();i++) {

        tl_io.ans[0] = '\0';
        tl_io.log_s[0] = '\0';

        pos_c pos_io;
        pos_io.indice = poslist->at(i).indice.c_str();
        pos_io.dealid = poslist->at(i).dealid.c_str();
        pos_io.pnl = poslist->at(i).pnl;
        pos_io.open = poslist->at(i).open;
        pos_io.size = poslist->at(i).size;
        pos_io.stop = poslist->at(i).stop;
        pos_io.vstop = poslist->at(i).vstop;
        pos_io.vlimit = poslist->at(i).vlimit;
        pos_io.nb_inc = poslist->at(i).nb_inc; 
        pos_io.limit = poslist->at(i).limit;

        (*tl)(&pos_io,&tl_io);

        poslist->at(i).vstop = pos_io.vstop;
        poslist->at(i).vlimit = pos_io.vlimit;
        poslist->at(i).nb_inc = pos_io.nb_inc;

        if (std::string(tl_io.ans) != "" ) {
          orders_queue->push(tl_io.ans);
        }
         
        if (std::string(tl_io.log_s) != "" ) {
          logger->log(tl_io.log_s);
        }

        
        //code de value fetching + close if vpos_limit
        float cval = records_last(t0->getIndiceRecords(poslist->at(i).indice))->value;

        
        if ( poslist->at(i).size < 0  &&  cval >= poslist->at(i).vstop ) {
          orders_queue->push("closepos:" + poslist->at(i).dealid );
        }

        else if ( poslist->at(i).size > 0  &&  cval <= poslist->at(i).vstop ) {
          orders_queue->push("closepos:" + poslist->at(i).dealid );
        }
        
        if (poslist->at(i).vlimit != 0) {

          /* CLose on virtual limit */
          if ( poslist->at(i).size > 0  &&  cval >= poslist->at(i).vlimit ) {
            orders_queue->push("closepos:" + poslist->at(i).dealid );
          }
          
          /* CLose on virtual limit */
          else if ( poslist->at(i).size < 0  &&  cval <= poslist->at(i).vlimit ) {
            orders_queue->push("closepos:" + poslist->at(i).dealid );
          }

        }

      }

    }

    for(int j=0;j<si.size();j++) {
      
      records* mrecs = t0->getIndiceRecords(si.at(j));
      if (mrecs != NULL) {
        record* mr = records_last(mrecs);
        if (mr!= NULL) { 
          v = mr->value;
          //cout << v << endl;
          mm->computePNLs(si.at(j),v);
        }
      }
    }

    if (inc == 10) {
      //logger->log("Current PNL:" + float2string(mm->computeWholePNL()) );
      mm->saveCPNL();
      inc = 0;
    }
    
    inc++;
    sleep(1);
  } 

return NULL;

}


void* tsEngine::saveToBackend(void* arg) {

  tsEngine *t0 = (tsEngine*) arg;
  backend* back0 = t0->getBackend();
  t0->setBSP(0);
  int backend_save_pos = t0->getBSP();

  while(1) {

    AssocArray<records*>* inmem_recs = t0->getRecords();
    int rsize_snapshot;
    for (int i=0; i< inmem_recs->Size(); i++) {

      string iname = inmem_recs->GetItemName(i);
      records* recs = inmem_recs->at(i);

      //snap records size only once for first element.
      //(next elements should always have at leaest the same size)
      if (i==0) rsize_snapshot = recs->size;
      backend_save_pos = t0->getBSP();

      //cout << "BACKEND_SAVE_POS:" << backend_save_pos << endl;
      //cout << "RSIZE_SNAPSHOT:" << rsize_snapshot << endl;
      
      for (int j= backend_save_pos;j < rsize_snapshot ;j++) {
        record* r = (record*) malloc(sizeof(record));
        memcpy(r,&(recs->data[j]),sizeof(record));
        //cout << "{" << r->value << "," << r->spread << "}" << endl;
        back0->store(iname, r );
        free(r);
      }

    }

    backend_save_pos = rsize_snapshot;
    t0->setBSP(backend_save_pos);

    sleep(10);

  }

  return NULL;

}


void* tsEngine::poll(void* arg) {

  tsEngine *t0 = (tsEngine*) arg; 
  vector<bvex> values;

  AssocArray<indice*> ilist = t0->getIndicesList();
  igmLogger* logger = t0->getLogger();

  ticks_t ticks = t0->getTicks();

  uint32_t time_ms;
  string epic;
  string mepic; 
  float buy;
  float sell;
  float spread;

  while(1) {

    //perf profiling
    auto tt0 = std::chrono::high_resolution_clock::now();
    values = t0->getBroker()->getValues();

    time_ms = time(0);
    
    if (values.size() != 0 ) {

      for (int i=0;i<values.size();i++) {

        record r;
        epic = values[i].epic;
        buy = values[i].bid;
        sell = values[i].offer;

        r.timestamp = time_ms;
        r.value = (buy + sell) / 2;
        r.spread = (buy - sell) / 2;
        
        indice* idx = iResolve(ilist, epic);
        if (idx != NULL) {
          mepic = idx->name;
          t0->pushRecord(mepic,&r);

        }
      }
    }

    auto tt1 = std::chrono::high_resolution_clock::now();
    auto elapsed_t = tt1 - tt0;
    uint64_t elapsed = std::chrono::duration_cast<std::chrono::microseconds>(elapsed_t).count();
    
    if (elapsed < ticks.getval) {  
      usleep(ticks.getval - elapsed);
    }

  }

}



// ### VALUES EVALUATIONS ALGORITHM #######
void* tsEngine::evaluate(void* arg) {

  typedef void* (*eval_fct)(uint32_t,float,float, evaluate_io*);

  eval_thread* et = (eval_thread*) arg;
  tsEngine *t0 = et->engine;
  eval_fct f = (eval_fct) et->eval_ptr;
  string eval_name = et->eval_name;

  Queue<std::string> *orders_queue = t0->getOrdersQueue();
  igmLogger* logger = t0->getLogger();

  //######## EVALUATION-NEEDED VALUES
  uint32_t t;
  float v;
  float spread;
  evaluate_io ev_io;
  uint64_t previous_t = 0;

  ev_io.ans = (char*) malloc(256 * sizeof(char) +1);
  ev_io.log_s = (char*) malloc(1024 * sizeof(char) +1);
  ev_io.evmio_a = t0->getEVMIOArray();
  ev_io.s = t0->getStore();
  ev_io.genes = NULL;

  ev_io.recs = t0->getIndiceRecords(eval_name);

  ticks_t ticks = t0->getTicks();

  string ans_str;
  string log_str;

  indice* idx = iResolve(t0->getIndicesList(),eval_name);

  //waits for some data to be collected before starting to process;
  while(ev_io.recs->size == 0) { 
    cout << "Waiting for data population.." << endl;
    sleep(1);
  }

  while(1) {

    //perf profiling
    auto tt0 = std::chrono::high_resolution_clock::now();

    ev_io.indice_name = eval_name.c_str();

    ev_io.ans[0] = '\0';
    ev_io.log_s[0] = '\0';

    record* last_rec = records_last(ev_io.recs);

    t = last_rec->timestamp;
    v = last_rec->value;
    spread = last_rec->spread;

    if ( t0->eval_running(idx,t) == 1 && t != previous_t ) {
      //execution of found eval function
      (*f)(t,v, spread, &ev_io);
      
      ans_str = std::string(ev_io.ans);
      log_str = std::string(ev_io.log_s);


      if (log_str != "") {
           logger->log(log_str);
      }

      if ( ans_str != "") {
        orders_queue->push(ans_str);
      }
    }

    previous_t = t;

    auto tt1 = std::chrono::high_resolution_clock::now();
    auto elapsed_t = tt1 - tt0;
    uint64_t elapsed = std::chrono::duration_cast<std::chrono::microseconds>(elapsed_t).count();

    if (elapsed < ticks.eval) {  
      usleep(ticks.eval - elapsed);
    }

  }

}

// ############

void* tsEngine::execute(void* arg) {

  tsEngine *t0 = (tsEngine*) arg; 
  Queue<std::string> *orders_queue = t0->getOrdersQueue();
  broker* b0 = t0->getBroker();
  moneyManager* mm = t0->getMoneyManager();
  igmLogger* logger = t0->getLogger();
  AssocArray<indice*> ilist = t0->getIndicesList();
    
  std::string order;
  vector<std::string> order_params;
  
  while(1) {

    if (orders_queue->pop(order,false) ) {
      logger->log("Processing Order " + order);

      order_params = split(order,':');

      if (order_params.at(0) == "openpos" || order_params.at(0) == "smartpos"){

        std::string indice = order_params.at(1);
        std::string epic = iResolve(ilist,indice)->bmapping;
        std::string way = order_params.at(2);

        int nbc = 0;
        int stop = 0;
        int limit =0;

        if (order_params.at(0) == "openpos") {
          nbc= atoi(order_params.at(3).c_str());
          stop = atoi(order_params.at(4).c_str());
          limit = atoi(order_params.at(5).c_str());
        }

        else {

          int* smartval = (int*) malloc(2*sizeof(int));
          mm->smartAsk(smartval,indice);
          nbc= smartval[0];
          stop = smartval[1];
          limit = atoi(order_params.at(3).c_str());
          free(smartval);
        }

        logger->log("Opening Position on " + epic);

        int mm_answer = mm->ask(indice,way,nbc,stop);
     
        if (mm_answer == 0) {

         if (mm->getReversePosForceClose() == 1 ) {

           string reverse_way = ""; 
           vector<string> reverse_dealids;

           if (way == "sell") reverse_way = "buy";
           else reverse_way = "sell";

           reverse_dealids = mm->findPos(indice,reverse_way);
           for (int k=0;k<reverse_dealids.size();k++) b0->closePos(reverse_dealids[k]);
         }

         string res = b0->openPos(epic,way,nbc,stop,limit) ;
         logger->log("Broker Response:" + res);

        }
     
        else {
          logger->log("Position refused by moneymanager (" + mm->resolveError(mm_answer) + ")");
        }
      }    

      else if (order_params.at(0) == "closepos") {
        std::string dealid = order_params.at(1);
        position* cpos  = mm->getPositionByDealid(dealid);
        if (cpos != NULL) {
          cpos->status = POS_PENDING_CLOSE;
          string res = b0->closePos(dealid);
          logger->log("Broker Response:" + res);
        }
      }

      else if (order_params.at(0) == "closeallpos") {

        string indice = order_params.at(1);
        string way = order_params.at(2);

        vector<string> dealids = mm->findPos(indice,way);

        cout << "DEALIDS_SIZE:" << dealids.size() << endl;

        for (int k=0;k<dealids.size();k++) {
          string res = b0->closePos(dealids[k]);
          logger->log("Broker Response:" + res);
        }

      }



    }

    //execution loop needs to be really fast, so little sleep time
    usleep(1000);
  }

}

tsEngine::tsEngine() {

}

tsEngine::tsEngine(adamCfg* conf,
                   broker* b,
                   backend* back,
                   AssocArray<indice*> ilist,
                   strategy* s,
                   moneyManager* mm,
                   genetics* ge,
                   vector<string> mlist) {

  cout << "Initializing TS Engine.." << endl;

  //initializing store
  store_init(&tse_store);
  tse_genes = NULL;
  tse_broker = b;
  tse_ticks = conf->getTicks();
  tse_inmem_history = conf->getInMemHistory();
  tse_back = back;
  tse_strat = s;
  tse_mm = mm;
  tse_ge = ge;
  indices_list = ilist;
  modules_list = mlist;

  logger = new igmLogger();

  //initializes and connect to broker;
  tse_broker->initialize(conf->getBrokerParams(),true,true,"push");

  if (tse_broker->requiresIndicesList() == 1)  {
    tse_broker->setIndicesList(iGetEpics(indices_list));
  }
  int conn_status = tse_broker->connect();

  if (conn_status != 0) {
    logger->log("[broker] Can't connect to broker, please check your credentials/ API availability");
    exit(1);
  }

  else {
    logger->log("[broker] Connection to Broker successful");
  }
  


  vector<string> si = iGetNames(indices_list);
  for(int i=0;i<si.size();i++) {

    inmem_records[si[i]] = (records*) malloc(sizeof(records));
    //initializing records structures for each found indice
    records_init(inmem_records[si[i]],10000);

  }

  //uptime init
  uptime = 0;

  //loads potential previous cumulative PNL
  mm->loadCPNL();

  //loads history
  if (tse_back != NULL) {
    cout << "Loading history from backend.." << endl;
    loadHistory();
  }

  evmio_a.evmio = (eval_module_io*) malloc(modules_list.size() * sizeof(eval_module_io) );
  evmio_a.size = modules_list.size();


  for(int i=0;i<modules_list.size();i++) {

    cout << "Initializing Extra Module: " << modules_list[i] << endl;

    pthread_t t;
    module_initializer mi;
    modules_threads_list.push_back(t);
    modules_init_list.push_back(mi);

    module_initializer* cur_mi = &modules_init_list[modules_init_list.size()-1];

    cur_mi->engine = this;
    cur_mi->name = modules_list[i];
    cur_mi->input = CreateQueue(50);
    cur_mi->output = CreateQueue(50);

    strncpy(evmio_a.evmio[i].mname,cur_mi->name.c_str(),50*sizeof(char));
    evmio_a.evmio[i].input = &(cur_mi->input);
    evmio_a.evmio[i].output = &(cur_mi->output);

  }

  //Launching of all module threads AFTER the Initialisation of all of them
  for(int i=0;i<modules_init_list.size();i++) {
    module_initializer* cur_mi = &modules_init_list[i];
    pthread_t* cur_pthread = &modules_threads_list[i];
    pthread_create(cur_pthread,NULL,modulethread_wrapper,(void*) cur_mi);
  }

  printf ("initializing poller..\n");
  pthread_create(&poller,NULL,poll,(void*)this);
  
  printf ("initializing evaluators..\n");
  vector<string> evnames = iGetNames(getIndicesList());


  for (int i=0;i<evnames.size();i++) {
    eval_thread et;
    void* evptr = tse_strat->resolveFunction(evnames.at(i),"EVAL");
    if (evptr) {
      cout << "loading eval for indice "  << evnames.at(i)  << endl;
      et.eval_ptr = evptr;
      et.eval_name = evnames.at(i);
      et.engine = this;
      eval_threads.push_back(et);
    }
  }

  for (int i=0;i<eval_threads.size();i++) {
    pthread_create(&(eval_threads[i].th) ,NULL,evaluate,(void*)&(eval_threads[i]) );
  }

  printf ("Starting clock..\n");
  pthread_create(&clkth,NULL,aclock,(void*)this);  

  printf ("Initializing executor..\n");
  pthread_create(&executor,NULL,execute,(void*)this);

  printf ("Initializing money manager..\n");
  pthread_create(&mmth,NULL,moneyman,(void*)this);

  printf ("Initializing broker sync threads..\n");
  pthread_create(&bsync,NULL,broker_pos_sync,(void*)this);
  
  //pthread_create(&bfclose,NULL,broker_force_close,(void*)this);

  printf ("Initializing backend I/O Thread..\n");
  pthread_create(&backioth,NULL,saveToBackend,(void*)this);

}


adamCfg** tsEngine::getAdamConfig() {
  return &cfg;
}

backend* tsEngine::getBackend() {
  return tse_back;
}

broker* tsEngine::getBroker() {
  return tse_broker;
}

ticks_t tsEngine::getTicks() {
  return tse_ticks;
}

int tsEngine::getBSP() {
  return bsp;
}

void tsEngine::setBSP(int ibsp) {
  bsp = ibsp;
}


AssocArray<indice*> tsEngine::getIndicesList() {
  return indices_list;
}

Queue<std::string>* tsEngine::getOrdersQueue() {
  return &orders_queue;
}

strategy* tsEngine::getStrategy() {
  return tse_strat;
}

moneyManager* tsEngine::getMoneyManager() {
  return tse_mm;
}

 genetics* tsEngine::getGE() {
  return tse_ge;
 }

AssocArray<void*>* tsEngine::getEvalPointers() {
  return &eval_ptrs;
}



int tsEngine::pushRecord(string mepic,record* r) {
  records_push(inmem_records[mepic],*r);
  return 0;
}


AssocArray<records*>* tsEngine::getRecords() {
  return &inmem_records;
}

records* tsEngine::getIndiceRecords(string mepic) {
  return inmem_records[mepic];
}

// Loads indices history from backend to memory
int tsEngine::loadHistory() {

    if (tse_inmem_history == 0) return 0;

    vector<string> inames = iGetNames(indices_list);
    for (int i=0;i<inames.size();i++) {

      records* recs = tse_back->query(inames[i], -1 * tse_inmem_history, -1 );
      inmem_records[inames[i]] = recs;
      
    }

  return 0;
}


igmLogger* tsEngine::getLogger() {
  return logger;
}


evmio_array* tsEngine::getEVMIOArray() {
  return &evmio_a;
}

store* tsEngine::getStore() {
  return &tse_store;
}

store* tsEngine::getGeneticsStore() {
  return tse_genes;
}

void tsEngine::setGeneticsStore(store* gs) {
  tse_genes = gs;
}


int tsEngine::eval_running(indice* idx,time_t t) {

  int start_hour,end_hour;
  int start_minute,end_minute;
  
  struct tm* c_time = localtime(&t);

  sscanf(idx->start_hour.c_str(),"%d:%d",&start_hour,&start_minute);
  sscanf(idx->end_hour.c_str(),"%d:%d",&end_hour,&end_minute);

  uint8_t lim_inf = 0;
  uint8_t lim_sup = 0;

  if ( c_time->tm_hour > start_hour ) lim_inf = 1;
  else if (c_time->tm_hour == start_hour && c_time->tm_min >= start_minute  ) lim_inf = 1;

  if ( c_time->tm_hour < end_hour ) lim_sup = 1;
  else if (c_time->tm_hour == end_hour && c_time->tm_min < end_minute  ) lim_sup = 1;  

 if (lim_inf == 1 && lim_sup == 1) return 1;
 return 0;

} 

int tsEngine::getUptime() {
  return uptime;
}

void tsEngine::tick() {
  uptime++;
}
