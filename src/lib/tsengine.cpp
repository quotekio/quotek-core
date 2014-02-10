#include "tsengine.h"


void* modulethread_wrapper(void* arg) {

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

  iarray* tstamps = t0->getTimeStamps(); 

  mio.mode = t0->getMode();
  mio.tstamps = &tstamps;
  mio.values =  t0->getAllValues();
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
  vector<position>* poslist = mm->getPositions();
  igmLogger* logger = t0->getLogger();

  AssocArray<indice*> ilist = t0->getIndicesList();
  vector<string> si = iGetNames(ilist);

  rapidjson::Document d;

  indice* idx;

  //pos variables
  string epic;
  string indice;
  string dealid;
  string name;

  string stop_str;
  string limit_str;
  string open_str;
  string size_str;
  float stop;
  float limit;
  float open;
  int size;

  string mypos;
  vector<string> alive_pos;

  while(1) { 

    mypos = b0->getPositions();
    if (mypos != "") {

      d.Parse<0>(mypos.c_str());
      alive_pos.clear();
      for (int i=0;i<d.Capacity();i++) {
      
        idx = iResolve(ilist,d[SizeType(i)]["epic"].GetString());

        if (idx != NULL) {
          
          epic = d[SizeType(i)]["epic"].GetString();
          indice = idx->name;
          dealid = d[SizeType(i)]["id"].GetString();
          name = d[SizeType(i)]["name"].GetString();

          stop_str = d[SizeType(i)]["stop"].GetString();
          //limit_str = d[SizeType(i)]["limit"].GetString();
          open_str = d[SizeType(i)]["open"].GetString();
          size_str =  d[SizeType(i)]["size"].GetString();
      
          stop = 0;
          if (stop_str != "-") {
            stop = atof(stop_str.c_str());
          }

          limit = 0;
          //if (limit_str != "-") {
            //limit = atof(limit_str.c_str());
          //}        

          open = atof(open_str.c_str());
          size = atoi(size_str.c_str());

          alive_pos.push_back(dealid);

          if (! mm->hasPos(dealid) ) {
            logger->log("New pos found:" + dealid);       
   
            position p;
            p.epic = epic;
            p.indice = indice;
            p.dealid = dealid;
            p.name = name;
            p.open = open;
            p.stop = stop;
            p.vstop = stop;
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
    sleep(1);
  }
}

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


void* moneyman_backtest(void* arg) {

  tsEngine *t0 = (tsEngine*) arg;
  float mm_speed = t0->getSpeed();

  moneyManager* mm = t0->getMoneyManager();
  strategy* s = t0->getStrategy();

  AssocArray<indice*> ilist = t0->getIndicesList();
  vector<string> si = iGetNames(ilist);

  igmLogger* logger = t0->getLogger();
  Queue<std::string> *orders_queue = t0->getOrdersQueue();

  //TRADELIFE struct for fctptr
  typedef void* (*tl_fct)(pos_c*,tradelife_io*);
  void* tl_fct_fref = s->getTLFct(); 
  vector<position>* poslist = mm->getPositions();

  farray* fav;
  float v;
  
  tradelife_io tl_io;

  tl_io.ans = (char*) malloc(1024* sizeof(char));
  tl_io.log_s = (char*) malloc(1024* sizeof(char));
  tl_io.s = t0->getStore();

  while(1) {

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
        pos_io.nb_inc = poslist->at(i).nb_inc;
        pos_io.limit = poslist->at(i).limit;
        pos_io.status = poslist->at(i).status;

        (*tl)(&pos_io,&tl_io);

        poslist->at(i).vstop = pos_io.vstop ;
        poslist->at(i).nb_inc = pos_io.nb_inc;    
    
        if (std::string(tl_io.ans) != "" ) {
          orders_queue->push(tl_io.ans);
        }

        if (std::string(tl_io.log_s) != "" ) {
          logger->log(tl_io.log_s);
        }

      }

    }
    

    for(int j=0;j<si.size();j++) {
      fav = t0->getValues(si.at(j));
      v = fav->values[t0->getBacktestPos()];
      mm->computePNLs(si.at(j),v);

      //close positions where limit/stop is reached
      for(std::vector<position>::iterator iter = poslist->begin(); iter != poslist->end();++iter) {

        if ( poslist->size() == 0 ) break;

        position p0 = *iter;
        position* p = &p0;

        if  (p->indice == si.at(j)) {

          int rmpos = 0;

          if (p->size > 0 &&  v <= p->stop) {
            rmpos = REMPOS_STOP;
          }
 
          else if (p->size > 0 && v <= p->vstop) {
            rmpos = REMPOS_VSTOP;
          }

          else if (p->size > 0 && v >= p->limit) {
            rmpos = REMPOS_LIMIT;
          }

          else if (p->size < 0 && v >= p->stop ) {
            rmpos = REMPOS_STOP;
          }

          else if (p->size < 0 && v >= p->vstop ) {
            rmpos = REMPOS_VSTOP;
          }

          else if (p->size < 0 && v <= p->limit ) {
            rmpos = REMPOS_LIMIT;
          }

          if (rmpos > 0) {

            iter = mm->remPosition(iter);
            string logstr = "POS " + p->indice + " Removed";

            if (rmpos == REMPOS_STOP) logstr = logstr + " (STOP)";
            else if (rmpos == REMPOS_VSTOP) logstr = logstr + " (VSTOP)";
            else if (rmpos == REMPOS_LIMIT) logstr = logstr + " (LIMIT)";

            logger->log(logstr);
            if (iter == poslist->end() ) break;

          }

      }
    }

    }

    usleep(mm_speed);
  }

}



void* moneyman(void* arg) {

  tsEngine *t0 = (tsEngine*) arg;
  broker* b0 = t0->getBroker();
  moneyManager* mm = t0->getMoneyManager();
  AssocArray<indice*> ilist = t0->getIndicesList();
  vector<string> si = iGetNames(ilist);
  igmLogger* logger = t0->getLogger();
  Queue<std::string> *orders_queue = t0->getOrdersQueue();
  strategy* s = t0->getStrategy();

  //TRADELIFE struct for fctptr
  typedef void* (*tl_fct)(pos_c*,tradelife_io*);
  void* tl_fct_fref = s->getTLFct(); 
  vector<position>* poslist = mm->getPositions();

  //pnl-needed vars
  farray* fav;
  float v;

  tradelife_io tl_io;
  
  int inc = 0;

  tl_io.ans = (char*) malloc(1024* sizeof(char));
  tl_io.log_s = (char*) malloc(1024* sizeof(char));
  tl_io.s = t0->getStore();

  while (1) {

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
        pos_io.nb_inc = poslist->at(i).nb_inc; 
        pos_io.limit = poslist->at(i).limit;

        (*tl)(&pos_io,&tl_io);

        poslist->at(i).vstop = pos_io.vstop;
        poslist->at(i).nb_inc = pos_io.nb_inc;

        if (std::string(tl_io.ans) != "" ) {
          orders_queue->push(tl_io.ans);
        }
         
        if (std::string(tl_io.log_s) != "" ) {
          logger->log(tl_io.log_s);
        }

        
        //code de value fetching + close if vpos_limit
        float cval = farray_last( t0->getValues(poslist->at(i).indice));
        if ( poslist->at(i).size < 0  &&  cval >= poslist->at(i).vstop ) {
          orders_queue->push("closepos:" + poslist->at(i).dealid );
        }

        else if ( poslist->at(i).size > 0  &&  cval <= poslist->at(i).vstop ) {
          orders_queue->push("closepos:" + poslist->at(i).dealid );
        }
        
      }

    }

    for(int j=0;j<si.size();j++) {
      fav = t0->getValues(si.at(j));
      v = farray_last(fav);
      mm->computePNLs(si.at(j),v);
    }

    if (inc == 10) {
      logger->log("Current PNL:" + float2string(mm->computeWholePNL()) );
      inc = 0;
    }
    
    inc++;
    sleep(1);
  } 

return NULL;

}

/* Main Thread callback for the run of genetic algorithms */
/* Is also in charge of fake polling during backtest */

void* genetics_run(void* arg) {

  tsEngine* t0 = (tsEngine*) arg;
  iarray* tstamps = t0->getTimeStamps();
  igmLogger* logger = t0->getLogger();
  genetics* ge = t0->getGE();
  moneyManager* mm = t0->getMoneyManager();

  cout << "Initializing Population.." << endl;
  ge->initPopulation();

  int gen = 0;

  char bt_status[128];
  int bt_adv;
  int bt_adv_dlock;


  while(1) {

    for (int i=0;i<ge->getPopulationSize();i++) {

      individual* iv = ge->getIndividualFromPopulation(i);

      if ( ge->mustCompute(iv) ) {

        cout << "Processing Generation #" << gen << ", individual #" << i << ".." <<endl; 
        store* gstore = &(iv->attributes);
        t0->setGeneticsStore(gstore);

        bt_adv = 0;
        bt_adv_dlock = 0;

        for(int j=0;j<tstamps->size;j++) {
          t0->setBacktestPos(j);

          if ( j % ( tstamps->size / 100 )  == 0) {
            bt_adv++;
            bt_adv_dlock = 0;
          }

          if (bt_adv % 10 == 0 && bt_adv_dlock == 0) {
            sprintf(bt_status,"Backtest Status: %d%%", bt_adv); 
            logger->log(bt_status);
            bt_adv_dlock = 1;
          }

          usleep(t0->getSpeed());
        }
      
        iv->result = mm->getEndResult();
        store_clear(t0->getStore());
        mm->clear();
      }
    }


    gen++;
    if (  ge->getMaxGenerations() != 0 &&  gen >= ge->getMaxGenerations()  ) {
      cout << "Genetics Maximum number of generations reached" << endl;
      ge->dumpWinner();
      exit(0);
    }

    else if (ge->converges() ) {
      cout << "Genetics Convergence achieved !" << endl;
      ge->dumpWinner();
      exit(0);
    }

    ge->dumpPopulation();
    ge->newgen();


  }

  return NULL;

}


void* poll_backtest(void* arg ) {

  tsEngine* t0 = (tsEngine*) arg;
  iarray* tstamps = t0->getTimeStamps();
  igmLogger* logger = t0->getLogger();

  adamresult* result = new adamresult();
  result->start = time(0);
  result->from = tstamps->values[0]; 
  result->to = tstamps->values[tstamps->size -1];
  
  char bt_status[128];

  int bt_adv = 0;
  int bt_adv_dlock = 0;
  
  for(int i=0;i<tstamps->size;i++) {
    t0->setBacktestPos(i);

    if ( i % ( tstamps->size / 100 )  == 0) {
      bt_adv++;
      t0->setBacktestProgress(bt_adv);
      bt_adv_dlock = 0;
    }

    if (bt_adv % 10 == 0 && bt_adv_dlock == 0) {
      sprintf(bt_status,"Backtest Status: %d%%", bt_adv); 
      logger->log(bt_status);
      bt_adv_dlock = 1;
    }

    usleep(t0->getSpeed());

  }

  //completes end timestamp
  result->stop = time(0);

  //adds CFD/whatever assets statistics like
  //highest, lowest,variance, etc..
  t0->addAStats(result); 

  cout << endl;
  cout << "=================" << endl;
  cout << "BACKTEST FINISHED" << endl;
  cout << "=================" << endl << endl;

  moneyManager* mm = t0->getMoneyManager();
  mm->addStats(result);

  if (t0->getAdamConfig()->getBTResultFile() != "") {
    ofstream ofh (t0->getAdamConfig()->getBTResultFile());
    if (ofh.is_open()) {
      string s = result->json_encode();
      //mm->getStats(&s);
      ofh << s << endl;
      ofh.close();
    }
  }
  else {
    mm->displayStats();
  }

  exit(0);

  return NULL;
}


void* poll(void* arg) {

  tsEngine *t0 = (tsEngine*) arg; 
  string val;
  rapidjson::Document d;

  AssocArray<indice*> ilist = t0->getIndicesList();
  iarray* tstamps = t0->getTimeStamps();
  igmLogger* logger = t0->getLogger();

  uint32_t time_ms;

  string epic;
  string mepic; 
  string buystr;
  string sellstr;
  float buy;
  float sell;

  float spreadless_val;

  while(1) {

    val = t0->getBroker()->getValues();

    time_ms = time(0);
    iarray_push(tstamps,time_ms);
    
    if (val != "") {

      d.Parse<0>(val.c_str());

      for (int i=0;i<d.Capacity();i++) {

        epic = d[SizeType(i)]["epic"].GetString();
        buystr =  d[SizeType(i)]["buy"].GetString();
        sellstr =  d[SizeType(i)]["sell"].GetString();
        buy = atof(buystr.c_str());
        sell = atof(sellstr.c_str());
        spreadless_val = (buy + sell) / 2;
        
        indice* idx = iResolve(ilist,epic);
        if (idx != NULL) {
          mepic = idx->name;
          if (spreadless_val != 0) {
            t0->pushValues(mepic,spreadless_val);
          }
          else {
            logger->log("*ERROR: " + mepic +  " Value = 0, skipping data*");
          }
        }

      }
    }
    sleep(1);
  }

}



// ### VALUES EVALUATIONS ALGORITHM #######

void* evaluate(void* arg) {

  typedef void* (*eval_fct)(uint32_t,float,evaluate_io*);

  eval_thread* et = (eval_thread*) arg;
  tsEngine *t0 = et->engine;
  eval_fct f = (eval_fct) et->eval_ptr;
  string eval_name = et->eval_name;

  Queue<std::string> *orders_queue = t0->getOrdersQueue();
  igmLogger* logger = t0->getLogger();

  float eval_speed = t0->getSpeed();
  int eval_mode = t0->getMode();

  //######## EVALUATION-NEEDED VALUES
  uint32_t t;
  float v;
  evaluate_io ev_io;

  ev_io.ans = (char*) malloc(256 * sizeof(char) +1);
  ev_io.log_s = (char*) malloc(1024 * sizeof(char) +1);
  ev_io.evmio_a = t0->getEVMIOArray();
  ev_io.s = t0->getStore();

  ev_io.genes = NULL;

  //waits for genes store to be filled before doing anything
  if (t0->getMode() == ADAM_MODE_GENETICS) {
    while (ev_io.genes == NULL) {
      ev_io.genes = t0->getGeneticsStore();
    }
  }

  ev_io.tstamps = t0->getTimeStamps();

  string ans_str;
  string log_str;

  indice* idx = iResolve(t0->getIndicesList(),eval_name);

  //waits for some data to be collected before starting to process;
  while(ev_io.tstamps->size == 0) sleep(1);

  while(1) {

    ev_io.indice_name = eval_name.c_str();

    ev_io.ans[0] = '\0';
    ev_io.log_s[0] = '\0';

    ev_io.values = t0->getValues(eval_name);

    if (eval_mode == ADAM_MODE_BACKTEST || eval_mode == ADAM_MODE_GENETICS) {
      int i = t0->getBacktestPos();
      t = ev_io.tstamps->values[i];
      v = ev_io.values->values[i];
    }

    else {
      t = iarray_last(ev_io.tstamps);
      v = farray_last(ev_io.values);
    }

    if ( t0->eval_running(idx,t) == 1) {
      //execution of found eval function
      (*f)(t,v,&ev_io);

      ans_str = std::string(ev_io.ans);
      log_str = std::string(ev_io.log_s);

      if (log_str != "") {
           logger->log(log_str);
      }

      if ( ans_str != "") {
        orders_queue->push(ans_str);
      }
    }

    if (eval_mode == ADAM_MODE_BACKTEST || eval_mode == ADAM_MODE_GENETICS ) usleep(eval_speed);
    else sleep(1);

  }

}

// ############

void* execute(void* arg) {

  tsEngine *t0 = (tsEngine*) arg; 
  Queue<std::string> *orders_queue = t0->getOrdersQueue();
  broker* b0 = t0->getBroker();
  moneyManager* mm = t0->getMoneyManager();
  igmLogger* logger = t0->getLogger();
  AssocArray<indice*> ilist = t0->getIndicesList();
  
  int exec_mode = t0->getMode();
  float exec_speed = t0->getSpeed();

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


         if (exec_mode == ADAM_MODE_BACKTEST || exec_mode == ADAM_MODE_GENETICS) {
           position p;
           p.epic = epic;
           p.indice = indice;
           p.dealid = randstring(8);
           p.nb_inc = 1;
           
           if (way == "sell") {
             //-1 is for spread
             p.open = t0->getValues(indice)->values[ t0->getBacktestPos() ] -1 ;
             p.size = -1 * nbc;
             p.stop = p.open + stop;
             p.vstop = p.stop;
             p.limit = p.open - limit;
           }

           else {
             //+1 is for spread
             p.open = t0->getValues(indice)->values[ t0->getBacktestPos() ] +1 ;
             p.size = nbc;
             p.stop = p.open - stop;
             p.vstop = p.stop;
             p.limit = p.open + limit;
           }
           p.pnl = 0;
           p.status = POS_OPEN;
           mm->addPosition(p);
         }

         else b0->openPos(epic,way,nbc,stop,limit) ;

        }
     
        else {
          logger->log("Position refused by moneymanager (" + mm->resolveError(mm_answer) + ")");
        }


      }    

      else if (order_params.at(0) == "closepos") {
        std::string dealid = order_params.at(1);
        if (exec_mode == ADAM_MODE_BACKTEST || exec_mode == ADAM_MODE_GENETICS) {
          mm->remPosition(dealid); 
        }

        else {
          position* cpos  = mm->getPositionByDealid(dealid);
          if (cpos != NULL) {
            cpos->status = POS_PENDING_CLOSE;
            b0->closePos(dealid);
          }
        }
      }
    }

    if (exec_mode == ADAM_MODE_BACKTEST || exec_mode == ADAM_MODE_GENETICS) usleep(exec_speed);
    else sleep(1);
  }

}





tsEngine::tsEngine(adamCfg* conf,
                   broker* b,
                   AssocArray<indice*> ilist,
                   strategy* s,
                   moneyManager* mm,
                   genetics* ge,
                   vector<string> mlist) {

  cout << "Initializing TS Engine.." << endl;

  cfg = conf;
  tse_mode = conf->getMode();

  tse_speed = (float) 1 / (float) conf->getBSpeed() * 1000000;
  tse_dump = conf->getBDump();
  tse_backtest_pos = 0;
  tse_backtest_progress = 0;

  //initializing store
  store_init(&tse_store,256);
  tse_genes = NULL;

  tse_broker = b;
  tse_strat = s;
  tse_mm = mm;
  tse_ge = ge;
  indices_list = ilist;
  modules_list = mlist;

  logger = new igmLogger();

  vector<string> si = iGetNames(indices_list);

  for(int i=0;i<si.size();i++) {

    //initializing values structure for each found indice
    values[si[i]] = (farray*) malloc(1*sizeof(farray));
    farray_init(values[si[i]],10);
  }
  
  //timestamps array init
  iarray_init(&timestamps,10);


  //Si dump backtest fourni en argument => on charge
  if (tse_dump != "") loadDump(tse_dump);
  else loadDump();
    

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


  
  if (tse_mode == ADAM_MODE_BACKTEST ) {
    printf ("initializing poller (backtest mode)..\n");
    pthread_create(&poller,NULL,poll_backtest,(void*)this); 
  }

  else if (tse_mode == ADAM_MODE_GENETICS) {
    printf("Starting Genetics (backtest mode)..\n");
    pthread_create(&poller,NULL,genetics_run,(void*)this);
  }

  else {
    printf ("initializing poller..\n");
    pthread_create(&poller,NULL,poll,(void*)this);
  }

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


  printf ("initializing executor..\n");
  pthread_create(&executor,NULL,execute,(void*)this);

  if (tse_mode == ADAM_MODE_BACKTEST || tse_mode == ADAM_MODE_GENETICS ) {
    printf ("initializing money manager (backtest mode)..\n");
    pthread_create(&mmth,NULL,moneyman_backtest,(void*)this);
  }
  else {
    printf ("initializing money manager..\n");
    pthread_create(&mmth,NULL,moneyman,(void*)this);
  }

  if (tse_mode != ADAM_MODE_BACKTEST && tse_mode != ADAM_MODE_GENETICS  ) {
    printf ("initializing broker sync threads..\n");
    pthread_create(&bsync,NULL,broker_pos_sync,(void*)this);
    pthread_create(&bfclose,NULL,broker_force_close,(void*)this);
  }

}


adamCfg* tsEngine::getAdamConfig() {
  return cfg;
}

broker* tsEngine::getBroker() {
  return tse_broker;
}

AssocArray<indice*> tsEngine::getIndicesList() {
  return indices_list;
}

iarray* tsEngine::getTimeStamps() {
  return &timestamps;
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


int tsEngine::pushValues(string mepic,float v) {
  farray_push(values[mepic],v);
  return 0;
}

farray* tsEngine::getValues(string mepic) {
  return values[mepic];
}

AssocArray<farray*>* tsEngine::getAllValues() {
  return &values;
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


int tsEngine::getMode() {
  return tse_mode;
}

float tsEngine::getSpeed() {
  return tse_speed;
}

int tsEngine::getBacktestPos() {
  return tse_backtest_pos;
}

int tsEngine::getBacktestProgress() {
  return tse_backtest_progress;
}

void tsEngine::setBacktestPos(int bpos) {
  tse_backtest_pos = bpos;
}

void tsEngine::setBacktestProgress(int bprogress) {
  tse_backtest_progress = bprogress;
}


store* tsEngine::getGeneticsStore() {
  return tse_genes;
}

void tsEngine::setGeneticsStore(store* gs) {
  tse_genes = gs;
}


int tsEngine::loadDump() {
  cout << "loading previous dump.." << endl;
  return loadDump("/tmp/adam/adam.vdump");
}

int tsEngine::loadDump(string dump_file) {

  ifstream dumpfh (dump_file);

  if (dumpfh.is_open() ) {
  
    string line;
    vector<string> sline;

    while(dumpfh.good()) {
      getline(dumpfh,line);
      sline = split(line,':');

      if (sline.size() == 2) {

        if (sline.at(0) == "T" ) {
          iarray_push(&timestamps,atoi(sline.at(1).c_str()));
        }

        else {
          if (! values.IsItem(sline.at(0))) {
            values[sline.at(0)] = (farray*) malloc(1*sizeof(farray));
            farray_init(values[sline.at(0)],100);
          }
          farray_push(values[sline.at(0)], atof(sline.at(1).c_str()) );
        }
      }
    }  

  }

  dumpfh.close();
  return 0;

}


int tsEngine::dumpAll() {
  
  ofstream dumpfh ("/tmp/adam/adam.vdump");
  int i,j;
  vector<string> si = iGetNames(indices_list);

  for (i=0;i<timestamps.size;i++ ) {

    dumpfh << "T:"  << timestamps.values[i] << endl ;

    for (j=0;j<si.size();j++) {
      dumpfh << si.at(j) << ":" << values[si.at(j)]->values[i] << endl;
    }

  }
  dumpfh.close();

  return 0;
 
}


void tsEngine::addAStats(adamresult* result) {

  for(int i=0;i< values.Size();i++ ) {

    assetstats* a1 = new assetstats();
    a1->name = values.GetItemName(i);
    a1->variation = percentDelta(values[a1->name]);
    a1->deviation = stdDeviation(values[a1->name]);
    a1->highest = max(values[i]);
    a1->lowest = min(values[i]);

    result->astats.push_back(a1);


  }

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

