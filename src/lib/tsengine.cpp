#include "tsengine.h"

tsexport* tsEngine::eexport() {

  return new tsexport(1,
                      getLogger(),
                      tse_mm->getCurPNL(),
                      tse_mm->getCumulativePNL(),
                      tse_mm->getPositions(),
                      &tse_store
                       );

}

void* tsEngine::modulethread_wrapper(void* arg) {

  if (!arg) {
    cout << "Fatal error: Null pointer as module argument" << endl;
    exit(1);
  }

  /*

  //typedef void* (*fctptr)(module_io);
  //module_initializer* mi = (module_initializer*) arg;
  //tsEngine* t0 = mi->engine;
  //moneyManager* mm = t0->getMoneyManager();
  //string module_so = "lib" + mi->name + ".so";
 
  module_io mio;
  mio.cur_pnl = mm->getCurPNL();
  mio.cumulative_pnl = mm->getCumulativePNL();
  mio.s = t0->getStore() ;
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
  
  //fctptr f = (fctptr) symbol;
  //(*f)(mio);
  */
  return NULL;
} 

void tsEngine::openPosition(string epic, string way, int nbc, int stop, int limit) {

  //API Performance profiling.
  auto tt0 = std::chrono::high_resolution_clock::now();

  bpex ex1;
  indice* idx = iResolve(indices_list, epic);

  ex1 = tse_broker->openPos(epic, way, nbc, stop, limit);

  if (ex1.status == "ACCEPTED")  {

    position p;
    p.epic = epic;
    p.indice = idx->name;
    p.dealid = ex1.dealid;
    p.open = ex1.open;
    p.stop = ex1.stop;
    p.vstop = ex1.stop;
    p.vlimit = ex1.limit;
    p.nb_inc = 1;
    p.limit = ex1.limit;
    p.size = ex1.size;
    p.pnl = 0;
    p.pnl_peak = -1000000000;
    p.status = POS_OPEN;
    p.open_time = time(0);
    tse_mm->addPosition(p);

    auto tt1 = std::chrono::high_resolution_clock::now();
    auto elapsed_t = tt1 - tt0;
    uint64_t elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_t).count();

    logger->log("New Position Opened: " + ex1.dealid + " (" + int2string(elapsed) + "ms)");
  }

  else  {
    logger->log("[broker] Error opening position:" + ex1.reason );
  }

}

void tsEngine::closePosition(string dealid) {

  //API Performance profiling.
  auto tt0 = std::chrono::high_resolution_clock::now();

  position* p = tse_mm->getPositionByDealid(dealid);
  p->status = POS_PENDING_CLOSE;

  string result = tse_broker->closePos(dealid, p->size);
  if (result == "ACCEPTED:SUCCESS" ) {
    tse_mm->remPosition(dealid);

    auto tt1 = std::chrono::high_resolution_clock::now();
    auto elapsed_t = tt1 - tt0;
    uint64_t elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_t).count();

    logger->log("Position Closed: " + dealid + " (" + int2string(elapsed) + "ms)" );
  }

  else  {
    logger->log("[broker] Error closing position:" + result );
  }

}

/**
* broker_sync method enables to synchronize broker and bot at start
*/
void broker_sync_start(void* arg) {

  tsEngine* t0 = (tsEngine*) arg;
  moneyManager* mm = t0->getMoneyManager();
  broker* b0 = t0->getBroker();
  ticks_t ticks = t0->getTicks();
  std::deque<position>* poslist = mm->getPositions();
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

  vector<bpex> broker_poslist;
  vector<string> alive_pos;

  poslist = mm->getPositions();
  broker_poslist = b0->getPositions();

  alive_pos.clear();
  for (int i=0;i < broker_poslist.size();i++) {
      
    idx = iResolve(ilist, broker_poslist[i].epic);

    if (idx != NULL) {
          
      epic = broker_poslist[i].epic;
      indice = idx->name;
      dealid = broker_poslist[i].dealid;
      size = broker_poslist[i].size;
      stop = broker_poslist[i].stop;
      limit = broker_poslist[i].limit;
      open = broker_poslist[i].open;
          
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
  std::deque<position>* poslist = mm->getPositions();

  //pnl-needed vars
  float v;

  float cval;
  record* r;
 
  position* p;

  tradelife_io tl_io;
  
  int inc = 0;

  Queue_c orders_q = CreateQueue(50);
  Queue_c logs_q = CreateQueue(50);

  tl_io.orders = &orders_q;
  tl_io.logs = &logs_q;
  tl_io.s = t0->getStore();

  while (1) {

    //checks STOPS & LIMIT and cleans positions if needed.
    for(std::deque<position>::iterator iter = poslist->begin(); iter != poslist->end();++iter) {

      p = &*iter;
      r = records_last(t0->getIndiceRecords(p->indice));
      if (r != NULL) cval = r->value;
      else continue;

      //checking of Real Stops
      if ( p->size < 0  &&  cval >= p->stop ) {
        mm->remPosition(iter);
        logger->log("Position " +  p->dealid + " closed ! (STOP)");
        continue;
      }

      else if ( p->size > 0  &&  cval <= p->stop ) {
        mm->remPosition(iter);
        logger->log("Position " +  p->dealid + " closed ! (STOP)");
        continue;
      }

      if (p->limit > 0) {

        if ( p->size < 0  &&  cval < p->limit ) {
          mm->remPosition(iter);
          logger->log("Position " +  p->dealid + " closed ! (LIMIT)");
          continue;
        }

        else if ( p->size > 0  &&  cval > p->limit ) {
          mm->remPosition(iter);
          logger->log("Position " +  p->dealid + " closed ! (LIMIT)");
          continue;
        }
      }
    }

    //plays tradelife callback inside strategy.
    if ( tl_fct_fref != NULL) {

      tl_fct tl = (tl_fct) tl_fct_fref;

      for(std::deque<position>::iterator iter = poslist->begin(); iter != poslist->end();++iter) {

        
        p = &*iter;

        pos_c pos_io;
        pos_io.indice = p->indice.c_str();
        pos_io.dealid = p->dealid.c_str();
        pos_io.pnl = p->pnl;
        pos_io.open = p->open;
        pos_io.size = p->size;
        pos_io.stop = p->stop;
        pos_io.vstop = p->vstop;
        pos_io.vlimit = p->vlimit;
        pos_io.nb_inc = p->nb_inc; 
        pos_io.limit = p->limit;

        (*tl)(&pos_io,&tl_io);

        p->vstop = pos_io.vstop;
        p->vlimit = pos_io.vlimit;
        p->nb_inc = pos_io.nb_inc;

        while( ! IsEmpty( orders_q ) ) {
          char* order = (char*) FrontAndDequeue( orders_q );
          std::string order_str = std::string(order);
          if ( order_str != "") {
            orders_queue->push(order_str);
          }
          free(order);
        }

        while( ! IsEmpty( logs_q ) ) {
          char* logstr = (char*) FrontAndDequeue( logs_q );
          std::string log_str = std::string(logstr);
          if ( log_str != "") {
            logger->log(log_str);
          }
          free(logstr);
        }

        r = records_last(t0->getIndiceRecords(p->indice));
        if (r != NULL) cval = r->value;
        else continue;
        
        //Checking of Virtual Stops/Limits
        if ( p->size < 0  &&  cval >= p->vstop ) {
          orders_queue->push("closepos:" + p->dealid );
        }

        else if ( p->size > 0  &&  cval <= p->vstop ) {
          orders_queue->push("closepos:" + p->dealid );
        }
          
        if (p->vlimit != 0) {

          /* CLose on virtual limit */
          if ( p->size > 0  &&  cval >= p->vlimit ) {
            orders_queue->push("closepos:" + p->dealid );
          }
            
          /* CLose on virtual limit */
          else if ( p->size < 0  &&  cval <= p->vlimit ) {
            orders_queue->push("closepos:" + p->dealid );
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

    usleep(1000000);
  } 

return NULL;

}


void* tsEngine::saveToBackend(void* arg) {

  tsEngine *t0 = (tsEngine*) arg;
  backend* back0 = t0->getBackend();
  t0->setBSP(0);
  int backend_save_pos = t0->getBSP();
  std::deque<position>* pos_history = t0->getMoneyManager()->getPositionsHistory();
   
  int prev_t = 0;

  while(1) {

    auto tt0 = std::chrono::high_resolution_clock::now();

    //saves history
    for (int i=0; i< pos_history->size();i++ ) {
      if ( pos_history->at(i).close_time > prev_t ) {
        prev_t = pos_history->at(i).close_time;
        back0->saveHistory(&(pos_history->at(i)));
      }
    }

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

    auto tt1 = std::chrono::high_resolution_clock::now();
    auto elapsed_t = tt1 - tt0;
    uint64_t elapsed = std::chrono::duration_cast<std::chrono::microseconds>(elapsed_t).count();
    
    if (elapsed < 1000000) {  
      usleep(1000000 - elapsed);
    }

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

  Queue_c orders_q = CreateQueue(50);
  Queue_c logs_q = CreateQueue(50);

  ev_io.orders = &orders_q;
  ev_io.logs = &logs_q;
  ev_io.s = t0->getStore();
  ev_io.genes = NULL;
  ev_io.state = 0;

  ev_io.recs = t0->getIndiceRecords(eval_name);

  ticks_t ticks = t0->getTicks();

  string order_str;
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

    record* last_rec = records_last(ev_io.recs);

    t = last_rec->timestamp;
    v = last_rec->value;
    spread = last_rec->spread;

    if ( t0->eval_running(idx,t) == 1 && t != previous_t ) {
      
      //execution of found eval function
      (*f)(t,v, spread, &ev_io);
      
      while( ! IsEmpty( orders_q ) ) {
        char* order = (char*) FrontAndDequeue( orders_q );
        std::string order_str = std::string(order);
        if ( order_str != "") {
          orders_queue->push(order_str);
        }
        free(order);
      }

      while( ! IsEmpty( logs_q ) ) {
        char* logstr = (char*) FrontAndDequeue( logs_q );
        std::string log_str = std::string(logstr);
        if ( log_str != "") {
          logger->log(log_str);
        }
        free(logstr);
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
           for (int k=0;k<reverse_dealids.size();k++) t0->closePosition(reverse_dealids[k]);
         }

          t0->openPosition(epic,way,nbc,stop,limit) ;

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
          t0->closePosition(dealid);
        }
      }

      else if (order_params.at(0) == "closeallpos") {

        string indice = order_params.at(1);
        string way = order_params.at(2);

        vector<string> dealids = mm->findPos(indice,way);

        cout << "DEALIDS_SIZE:" << dealids.size() << endl;

        for (int k=0;k<dealids.size();k++) {
          t0->closePosition(dealids[k]);
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
  tse_broker->initialize(conf->getBrokerParams(),true,true, conf->getBrokerMode() );

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

  /* Loading of external modules */
  for(int i=0;i<modules_list.size();i++) {

    cout << "Initializing Extra Module: " << modules_list[i] << endl;

    pthread_t t;
    modules_threads_list.push_back(t);
    
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

  printf ("Synchronizing broker Positions with adam..\n");
  broker_sync_start((void*)this);

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
