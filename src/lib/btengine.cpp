#include "btengine.h"

/*
removed from executor()
//openpos
if (exec_mode == ADAM_MODE_BACKTEST || exec_mode == ADAM_MODE_GENETICS) {
           
         }

//closepos
if (exec_mode == ADAM_MODE_BACKTEST || exec_mode == ADAM_MODE_GENETICS) {
          mm->remPosition(dealid); 
        }

 
 //Main Thread callback for the run of genetic algorithms
 ///Is also in charge of fake polling during backtest.

void* btEngine::genetics_run(void* arg) {

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


void* btEngine::poll_backtest(void* arg ) {

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

  

  return NULL;
}
*/


btEngine::btEngine(adamCfg* conf,
                   broker* b,
                   AssocArray<indice*> ilist,
                   strategy* s,
                   moneyManager* mm,
                   genetics* ge,
                   vector<string> mlist) {


  tse_broker = b;
  tse_strat = s;
  tse_mm = mm;
  tse_ge = ge;
  indices_list = ilist;
  modules_list = mlist;

  cfg = conf;
  tse_mode = conf->getMode();
  backtest_dump = conf->getBDump();
  backtest_pos = 0;
  backtest_progress = 0;
  loadDump(backtest_dump);

  
  

  printf ("loading evaluators..\n");
  vector<string> evnames = iGetNames(getIndicesList());

  for (int i=0;i<evnames.size();i++) {
    void* evptr = tse_strat->resolveFunction(evnames.at(i),"EVAL");
    if (evptr) {
      eval_pointers[evnames.at(i)] = evptr;
    }
  }

}


void btEngine::evaluate_(string eval_name,void* eval_ptr) {

  typedef void* (*eval_fct)(uint32_t,float,evaluate_io*);

  uint32_t t;
  float v;
  evaluate_io ev_io;

  eval_fct f = (eval_fct) eval_ptr;

  ev_io.ans = (char*) malloc(256 * sizeof(char) +1);
  ev_io.log_s = (char*) malloc(1024 * sizeof(char) +1);
  ev_io.evmio_a = &evmio_a;
  ev_io.s = &tse_store;
  ev_io.genes = NULL;

  /*
  //waits for genes store to be filled before doing anything
  if (t0->getMode() == ADAM_MODE_GENETICS) {
    while (ev_io.genes == NULL) {
      ev_io.genes = t0->getGeneticsStore();
    }
  }
  */

  ev_io.tstamps = &timestamps;
  string ans_str;
  string log_str;
  indice* idx = iResolve(indices_list,eval_name);

  ev_io.indice_name = eval_name.c_str();
  ev_io.ans[0] = '\0';
  ev_io.log_s[0] = '\0';
  ev_io.values = values[eval_name];

  t = ev_io.tstamps->values[backtest_pos];
  v = ev_io.values->values[backtest_pos];

  if ( eval_running(idx,t) == 1) {
    //execution of found eval function
    (*f)(t,v,&ev_io);

    ans_str = std::string(ev_io.ans);
    log_str = std::string(ev_io.log_s);

    if (log_str != "") {
         logger->log(log_str);
    }

    if ( ans_str != "") {
      orders_queue.push(ans_str);
    }
  }

}

void btEngine::moneyman_() {

}

void btEngine::execute_() {

  std::string order;
  vector<std::string> order_params;

  if (orders_queue.pop(order,false) ) {
      logger->log("Processing Order " + order);
      order_params = split(order,':');
  }

  if (order_params.at(0) == "openpos" || order_params.at(0) == "smartpos"){

    std::string indice = order_params.at(1);
    std::string epic = iResolve(indices_list,indice)->bmapping;
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
      tse_mm->smartAsk(smartval,indice);
      nbc= smartval[0];
      stop = smartval[1];
      limit = atoi(order_params.at(3).c_str());
      free(smartval);
    }

    logger->log("Opening Position on " + epic);

    int mm_answer = tse_mm->ask(indice,way,nbc,stop);
  
    if (mm_answer == 0) {

     if (tse_mm->getReversePosForceClose() == 1 ) {

       string reverse_way = ""; 
       vector<string> reverse_dealids;

       if (way == "sell") reverse_way = "buy";
       else reverse_way = "sell";

       reverse_dealids = tse_mm->findPos(indice,reverse_way);
       for (int k=0;k<reverse_dealids.size();k++) {
         //* TO_IMPLEMENT closepos_reverse
       }
     }

     else { 

       position p;
       p.epic = epic;
       p.indice = indice;
       p.dealid = randstring(8);
       p.nb_inc = 1;
       
       if (way == "sell") {
         p.open = getValues(indice)->values[backtest_pos];
         p.size = -1 * nbc;
         p.stop = p.open + stop;
         p.vstop = p.stop;
         p.limit = p.open - limit;
       }

       else {
         p.open = getValues(indice)->values[backtest_pos];
         p.size = nbc;
         p.stop = p.open - stop;
         p.vstop = p.stop;
         p.limit = p.open + limit;
       }

       p.pnl = 0;
       p.status = POS_OPEN;
       tse_mm->addPosition(p);

     }

    }

    else {
      logger->log("Position refused by moneymanager (" + tse_mm->resolveError(mm_answer) + ")");
    }
  } 
}


void btEngine::run() {

  adamresult* result = new adamresult();
  result->start = time(0);
  result->from = timestamps.values[0]; 
  result->to = iarray_last(&timestamps);

  for(int i=0;i<timestamps.size;i++) {
    backtest_pos++;
    for (int j=0;j<eval_pointers.Size();j++) {
      evaluate_(eval_pointers.GetItemName(j), eval_pointers[j] );
    }

    moneyman_();
    execute_();
  }

  //completes end timestamp
  result->stop = time(0);

  //adds CFD/whatever assets statistics like
  //highest, lowest,variance, etc..
  addAStats(result); 
  addLogStats(result);

  cout << endl;
  cout << "=================" << endl;
  cout << "BACKTEST FINISHED" << endl;
  cout << "=================" << endl << endl;

  tse_mm->addStats(result);

  if (cfg->getBTResultFile() != "") {
    ofstream ofh (cfg->getBTResultFile());
    if (ofh.is_open()) {
      string s = result->json_encode();
      ofh << s << endl;
      ofh.close();
    }
  }
  else {
    tse_mm->displayStats();
  }

  exit(0);

}

void btEngine::runGenetics() {

}

int btEngine::getBacktestPos() {
  return backtest_pos;
}

int btEngine::getBacktestProgress() {
  return backtest_progress;
}

void btEngine::setBacktestPos(int bpos) {
  backtest_pos = bpos;
}

void btEngine::setBacktestProgress(int bprogress) {
  backtest_progress = bprogress;
}