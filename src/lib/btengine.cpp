#include "btengine.h"

/*
removed from executor()
//openpos
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

//closepos
if (exec_mode == ADAM_MODE_BACKTEST || exec_mode == ADAM_MODE_GENETICS) {
          mm->remPosition(dealid); 
        }



tsEngine() removed
  if (tse_mode == ADAM_MODE_BACKTEST ) {
    printf ("initializing poller (backtest mode)..\n");
    pthread_create(&poller,NULL,poll_backtest,(void*)this); 
  }

  if (tse_mode == ADAM_MODE_BACKTEST || tse_mode == ADAM_MODE_GENETICS ) {
    printf ("initializing money manager (backtest mode)..\n");
    pthread_create(&mmth,NULL,moneyman_backtest,(void*)this);
  }
  else {

  else if (tse_mode == ADAM_MODE_GENETICS) {
    printf("Starting Genetics (backtest mode)..\n");
    pthread_create(&poller,NULL,genetics_run,(void*)this);
  }

EVAL() REMOVED
if (eval_mode == ADAM_MODE_BACKTEST || eval_mode == ADAM_MODE_GENETICS) {
      int i = t0->getBacktestPos();
      t = ev_io.tstamps->values[i];
      v = ev_io.values->values[i];
    }


////####################################

void* btEngine::moneyman(void* arg) {

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

  //completes end timestamp
  result->stop = time(0);

  //adds CFD/whatever assets statistics like
  //highest, lowest,variance, etc..
  t0->addAStats(result); 
  t0->addLogStats(result);

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
*/

btEngine::btEngine(adamCfg* conf,
                   broker* b,
                   AssocArray<indice*> ilist,
                   strategy* s,
                   moneyManager* mm,
                   genetics* ge,
                   vector<string> mlist) {

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

}

void btEngine::run() {

  for(int i=0;i<timestamps.size;i++) {
    backtest_pos++;

    evaluate_("",NULL);

    moneyman_();
    execute_();
  }
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