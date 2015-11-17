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

void tsEngine::modulethread_wrapper() {

  /*

  //typedef void* (*fctptr)(module_io);
  //module_initializer* mi = (module_initializer*) arg;
  //tsEngine* t0 = mi->engine;
  //moneyManager* mm = this->getMoneyManager();
  //string module_so = "lib" + mi->name + ".so";
 
  module_io mio;
  mio.cur_pnl = mm->getCurPNL();
  mio.cumulative_pnl = mm->getCumulativePNL();
  mio.s = this->getStore() ;
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
} 

void tsEngine::openPosition(string id, string epic, string way, int nbc, int stop, int limit) {

  //API Performance profiling.
  auto tt0 = std::chrono::high_resolution_clock::now();

  bpex ex1;
  indice* idx = iResolve(indices_list, epic);

  ex1 = tse_broker->openPos(epic, way, nbc, stop, limit);

  if (ex1.status == "ACCEPTED")  {

    quotek::core::position p;
    p.identifier = id;
    p.asset_id = epic;
    p.asset_name = idx->name;
    p.ticket_id = ex1.dealid;
    p.open = ex1.open;
    p.stop = ex1.stop;
    p.set_vstop(ex1.stop);
    p.set_vlimit(ex1.limit);

    p.limit = ex1.limit;
    p.size = ex1.size;
    p.pnl = 0;
    //p.status = POS_OPEN;
    p.open_date = time(0);
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

  quotek::core::position* p = tse_mm->getPositionByDealid(dealid);
  //p->status = POS_PENDING_CLOSE;

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
void tsEngine::broker_sync_start() {
  
  moneyManager* mm = this->getMoneyManager();
  broker* b0 = this->getBroker();
  ticks_t ticks = this->getTicks();
  quotek::data::cvector<quotek::core::position>& poslist = mm->getPositions();
  igmLogger* logger = this->getLogger();

  AssocArray<indice*> ilist = this->getIndicesList();
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

        std::cout << "New position found:" << dealid << std::endl;
        logger->log("New position found:" + dealid);       

        quotek::core::position p;
        p.identifier = "none";
        p.asset_id = epic;
        p.asset_name = indice;
        p.ticket_id = dealid;
        p.open = open;
        p.stop = stop;
        p.set_vstop(stop);
        p.set_vlimit(limit);
        p.limit = limit;
        p.size = size;
        p.pnl = 0;

        //p.status = POS_OPEN;
        mm->addPosition(p);
      }    
    }
  }
}

void tsEngine::aclock() {

  while(1) {
    this->tick();
    sleep(1);
  }
}

void tsEngine::moneyman() {

  moneyManager* mm = this->getMoneyManager();
  AssocArray<indice*> ilist = this->getIndicesList();
  vector<string> si = iGetNames(ilist);
  igmLogger* logger = this->getLogger();
  quotek::data::cqueue<std::string> *orders_queue = this->getOrdersQueue();
  std::vector<strategyHandler*> sh_list = this->getStratHandlers();

  quotek::data::cvector<quotek::core::position>& poslist = mm->getPositions();

  //pnl-needed vars
  float v;
  float cval;
  quotek::data::record r;

  int inc = 0;

  while (1) {

    //checks STOPS & LIMIT and cleans positions if needed.
    for(int i=0;i< poslist.size();i++ ) {

      quotek::core::position& p = poslist[i];

      if ( this->getAssetRecords(p.asset_name).size() == 0 ) continue;

      r = this->getAssetRecords(p.asset_name).last();
      cval = r.value;

      //checking of Real Stops
    
      if ( p.size < 0  &&  cval >= p.stop ) {
        mm->remPosition(p.ticket_id);
        logger->log("Position " +  p.ticket_id + " closed ! (STOP)");
        continue;
      }

      else if ( p.size > 0  &&  cval <= p.stop ) {
        mm->remPosition(p.ticket_id);
        logger->log("Position " +  p.ticket_id + " closed ! (STOP)");
        continue;
      }

      //checking of virtual Stops

      float vstop = p.get_vstop();

      if ( vstop != p.stop ) {

        if (p.size < 0 && cval >= vstop) {
         closePosition(p.ticket_id);
         logger->log("Position " +  p.ticket_id + " closed ! (VSTOP)");
         continue;
        }

        else if (p.size > 0 && cval <= vstop) {
         closePosition(p.ticket_id);
         logger->log("Position " +  p.ticket_id + " closed ! (VSTOP)");
         continue;
        }
      }

      if (p.limit > 0) {

        if ( p.size < 0  &&  cval < p.limit ) {
          mm->remPosition(p.ticket_id);
          logger->log("Position " +  p.ticket_id + " closed ! (LIMIT)");
          continue;
        }

        else if ( p.size > 0  &&  cval > p.limit ) {
          mm->remPosition(p.ticket_id);
          logger->log("Position " +  p.ticket_id + " closed ! (LIMIT)");
          continue;
        }
      }
    }

    //plays tradelife callback inside strategy.
    //STUBBED

    for(int j=0;j<si.size();j++) {
      
      quotek::data::records& mrecs = this->getAssetRecords(si.at(j));
        if ( mrecs.size() > 0 ) {
          quotek::data::record& mr = mrecs.last();
          v = mr.value;
          //cout << v << endl;
          mm->computePNLs(si.at(j),v);
      }
    }

    if (inc == 10) {
      //logger->log("Current PNL:" + float2string(mm->computeWholePNL()) );
      float cp = mm->computeWholePNL();
  
      //The shit just hit the fan !
      if ( ! mm->heartbeat() ) {
        
        cerr << "* CRITICAL LOSS, BLOCKING MONEY MANAGER!*" << std::endl;
        logger->log("* CRITICAL LOSS, BLOCKING MONEY MANAGER! *");

      }

      mm->saveCPNL();
      inc = 0;
    }
    
    inc++;

    usleep(1000000);
  } 

}



void tsEngine::saveToBackend2() {

  backend* back0 = this->getBackend();

  while(1) {

    std::string savestr;
    std::string tag;
    long timestamp;

    auto tt0 = std::chrono::high_resolution_clock::now();

    //we pop the full tsEngine saving queue
    while( save_queue.pop(savestr,false)  ) {
      
     std::vector<std::string> vstr = split(savestr,0x1e);
     timestamp = atoi(vstr[0].c_str());
     back0->save(timestamp,vstr[1],vstr[2]);

    }

    auto tt1 = std::chrono::high_resolution_clock::now();
    auto elapsed_t = tt1 - tt0;
    uint64_t elapsed = std::chrono::duration_cast<std::chrono::microseconds>(elapsed_t).count();
    
    if (elapsed < 5000000) {  
      usleep(5000000 - elapsed);
    }

  }

}



void tsEngine::saveToBackend() {

  backend* back0 = this->getBackend();
  quotek::data::cvector<quotek::core::position>* pos_history = this->getMoneyManager()->getPositionsHistory();
  int prev_t = 0;

  while(1) {

    auto tt0 = std::chrono::high_resolution_clock::now();

    //saves history
    for (int i=0; i< pos_history->size();i++ ) {
      quotek::core::position p = pos_history->at(i);
      if ( p.close_date > prev_t ) {
        prev_t = p.close_date;
        back0->saveHistory(p);
      }
    }

    for (int i=0; i< inmem_records.Size(); i++) {

      string iname = inmem_records.GetItemName(i);
      quotek::data::records& recs = inmem_records[i];

      save_positions[iname].size = recs.size();

      //cout << "BACKEND_SAVE_POS:" << save_positions[iname].saved << endl;
      //cout << "RSIZE_SNAPSHOT:" << save_positions[iname].size << endl;
      
      for (int j=  save_positions[iname].saved ;j <  save_positions[iname].size ;j++) {
        quotek::data::record& r = recs[j];
        back0->store(iname, r );
      }

      save_positions[iname].saved = save_positions[iname].size;

    }

    auto tt1 = std::chrono::high_resolution_clock::now();
    auto elapsed_t = tt1 - tt0;
    uint64_t elapsed = std::chrono::duration_cast<std::chrono::microseconds>(elapsed_t).count();
    
    if (elapsed < 2000000) {  
      usleep(2000000 - elapsed);
    }

  }
}


void tsEngine::poll() {

  vector<bvex> values;

  AssocArray<indice*> ilist = this->getIndicesList();
  igmLogger* logger = this->getLogger();

  ticks_t ticks = this->getTicks();

  uint32_t time_ms;
  string epic;
  string mepic; 
  float buy;
  float sell;
  float spread;

  while(1) {

    //perf profiling
    auto tthis = std::chrono::high_resolution_clock::now();
    values = this->getBroker()->getValues();

    time_ms = time(0);
    
    if (values.size() != 0 ) {

      for (int i=0;i<values.size();i++) {

        epic = values[i].epic;

        indice* idx = iResolve(ilist, epic);
        float unit_coef = 1.0 / idx->pip_value;

        quotek::data::record r;
        buy = values[i].offer;
        sell = values[i].bid;

        r.timestamp = time_ms;
        r.value = (buy + sell) / 2;
        r.spread =  roundfloat( (buy - sell) * unit_coef, 0.1 ) ;
        
        if (idx != NULL) {
          mepic = idx->name;
          this->pushRecord(mepic,r);

        }
      }
    }

    auto tt1 = std::chrono::high_resolution_clock::now();
    auto elapsed_t = tt1 - tthis;
    uint64_t elapsed = std::chrono::duration_cast<std::chrono::microseconds>(elapsed_t).count();
    
    if (elapsed < ticks.getval) {  
      usleep(ticks.getval - elapsed);
    }

  }

}


void tsEngine::evaluate(strategy* s) {

  //declares work variables
  std::string order;
  std::string logstr;
  std::string savestr;

  //fetch tsEngine objects to avoid too mluch function calls
  igmLogger* logger = this->getLogger();
  ticks_t ticks = this->getTicks();

  //starts strategy initialization
  s->initialize();

  //flushing of data to save to another, more general queue.
  while(s->save_queue.pop(savestr,false)) {
      save_queue.push(savestr);
  }

  while(s->recs->size() == 0) { 
    std::cout << "Waiting for data population.." << std::endl;
    usleep(ticks.eval);
  }

  while(1) {
    //perf profiling
    auto tt0 = std::chrono::high_resolution_clock::now();

    //setting of evaluation context.
    quotek::data::record& last_rec = s->recs->last();
    s->value = last_rec.value;
    s->spread = last_rec.spread;
    s->t = last_rec.timestamp;

    //user algo tick evaluation.
    s->evaluate();

    while ( s->orders_queue.pop(order,false) ) {
      this->orders_queue.push(order);
    }

    //flushing of logs
    s->flushlogs();

    while( s->log_queue.pop(logstr,false) ) {
      logger->log(logstr);
    }

    //flushing of data to save to another, more general queue.
    while(s->save_queue.pop(savestr,false)) {
      save_queue.push(savestr);
    }

    auto tt1 = std::chrono::high_resolution_clock::now();
    auto elapsed_t = tt1 - tt0;
    uint64_t elapsed = std::chrono::duration_cast<std::chrono::microseconds>(elapsed_t).count();

    if (elapsed < ticks.eval) {  
      usleep(ticks.eval - elapsed);
    }

  }

}


// ############

void tsEngine::execute() {

  quotek::data::cqueue<std::string> *orders_queue = this->getOrdersQueue();
  broker* b0 = this->getBroker();
  moneyManager* mm = this->getMoneyManager();
  igmLogger* logger = this->getLogger();
  AssocArray<indice*> ilist = this->getIndicesList();
    
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
        std::string id = order_params.at(6);

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
           for (int k=0;k<reverse_dealids.size();k++) this->closePosition(reverse_dealids[k]);
         }

          this->openPosition(id,epic,way,nbc,stop,limit);

        }
     
        else {
          logger->log("Position refused by moneymanager (" + mm->resolveError(mm_answer) + ")");
        }
      }    

      else if (order_params.at(0) == "closepos") {
        std::string dealid = order_params.at(1);
        quotek::core::position* cpos  = mm->getPositionByDealid(dealid);
        if (cpos != NULL) {
          //cpos->status = POS_PENDING_CLOSE;
          this->closePosition(dealid);
        }
      }

      else if (order_params.at(0) == "closeallpos") {

        string indice = order_params.at(1);
        string way = order_params.at(2);

        vector<string> dealids = mm->findPos(indice,way);

        cout << "DEALIDS_SIZE:" << dealids.size() << endl;

        for (int k=0;k<dealids.size();k++) {
          this->closePosition(dealids[k]);
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
                   std::vector<strategyHandler*> sh_list,
                   moneyManager* mm,
                   genetics* ge,
                   vector<string> mlist) {

  cout << "Initializing TS Engine.." << endl;

  //initializing store
  //store_init(&tse_store);
  tse_genes = NULL;
  tse_broker = b;
  tse_ticks = conf->getTicks();
  tse_inmem_history = conf->getInMemHistory();
  tse_back = back;
  tse_strathandlers = sh_list;
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

  //uptime init
  uptime = 0;

  //loads potential previous cumulative PNL
  mm->loadCPNL();

  //initializes inmem_records and backend save pointer (else we get some pretty bad shit)
  for (int i=0;i<si.size();i++) {
    inmem_records[si[i]] = quotek::data::records();
    save_positions[si[i]].saved = 0;
  }

  //loads history
  if (tse_back != NULL) {
    cout << "Loading history from backend.." << endl;
    loadHistory();
  }

  /* Loading of external modules */
  for(int i=0;i<modules_list.size();i++) {

    cout << "Initializing Extra Module: " << modules_list[i] << endl;

    //std::thread t;
    //modules_threads_list.push_back(t);
    
  }

  printf ("initializing poller..\n");
  poller = new std::thread( [this] { poll() ;} );
  
  printf ("initializing evaluators..\n");
  vector<string> evnames = iGetNames(getIndicesList());

  for (int j=0;j<tse_strathandlers.size();j++) {

    void* strat_ptr = tse_strathandlers[j]->getExportFct();
    std::regex asset_match(tse_strathandlers[j]->getAssetMatch());

    for (int i=0;i<evnames.size();i++) {

      if (strat_ptr &&  std::regex_match(evnames.at(i), asset_match) ) {

        algo al;
        cout << "loading eval for indice "  << evnames.at(i)  << endl;
        al.eval_ptr = strat_ptr;
        al.eval_name = evnames.at(i);
        al.strategy = tse_strathandlers[j]->getName();
        al.pnl = 0;
        algos.emplace_back(al);

      }
    }
  }

  for (int i=0;i<algos.size();i++) {

    //function pointer to extern C create_st symbol.
    create_st* c_strat = (create_st*) algos[i].eval_ptr ;
    strategy* st = c_strat();

    st->recs = &this->getAssetRecords(algos[i].eval_name);
    
    //To Add later on.
    st->portfolio = this->getMoneyManager()->getPositionsPtr();
    st->asset_name = algos[i].eval_name;
    st->identifier = algos[i].strategy + "@" + algos[i].eval_name;
    st->store = &tse_store;

    algos[i].th = new std::thread( [st, this] {  this->evaluate(st); });   
    algos[i].th->detach();

  }

  printf ("Starting clock..\n");
  clkth = new std::thread([this] { aclock(); });

  printf ("Initializing executor..\n");
  executor = new std::thread([this] { execute(); });

  
  printf ("Initializing money manager..\n");
  mmth = new std::thread([this] { moneyman(); });
  
  printf ("Synchronizing broker Positions with adam..\n");
  broker_sync_start();


  printf ("Initializing backend I/O Thread..\n");
  backioth = new std::thread( [this] { saveToBackend(); }  );

  printf ("Initializing backend saving Thread..\n");
  backsaveth = new std::thread( [this] { saveToBackend2(); }  );

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

quotek::data::cqueue<std::string>* tsEngine::getOrdersQueue() {
  return &orders_queue;
}

std::vector<strategyHandler*> tsEngine::getStratHandlers() {
  return tse_strathandlers;
}

moneyManager* tsEngine::getMoneyManager() {
  return tse_mm;
}

 genetics* tsEngine::getGE() {
  return tse_ge;
 }


std::vector<algo> tsEngine::getAlgos() {
  return algos;
}


AssocArray<void*>* tsEngine::getEvalPointers() {
  return &eval_ptrs;
}

int tsEngine::pushRecord(string mepic, quotek::data::record& r) {
  inmem_records[mepic].append(r);
  return 0;
}


AssocArray<quotek::data::records>& tsEngine::getRecords() {
  return inmem_records;
}

quotek::data::records& tsEngine::getAssetRecords(string mepic) {
  return inmem_records[mepic];
}

// Loads indices history from backend to memory
int tsEngine::loadHistory() {

    if (tse_inmem_history == 0) return 0;

    vector<string> inames = iGetNames(indices_list);
    for (int i=0;i<inames.size();i++) {

      inmem_records[inames[i]] = tse_back->query(inames[i], -1 * tse_inmem_history, -1 );

      //we place backend save pointer forward to avoid double data in backend.
      save_positions[inames[i]].size = inmem_records[inames[i]].size();
      save_positions[inames[i]].saved = inmem_records[inames[i]].size();

      std::cout << "Loaded " << inmem_records[inames[i]].size() << " records for asset " << inames[i] << std::endl; 

    }

  return 0;
}


igmLogger* tsEngine::getLogger() {
  return logger;
}

std::map<std::string, quotek::data::any>& tsEngine::getStore() {
  return tse_store;
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
