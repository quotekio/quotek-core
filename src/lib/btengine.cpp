#include "btengine.h"

/* REMAINS TO IMPLEMENT:
 3) Tradelife analysis in moneyman()
 4) All genetics handling !
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

  //timestamps array init
  iarray_init(&timestamps,10000);

  //initializes logger
  logger = new igmLogger();

  //initializes store
  store_init(&tse_store,256);

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

  //debug
  //cout << "SIMUTIME:" << epochToDateTime(t) << endl;

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

  vector<string> si = iGetNames(indices_list);
  farray* fav;
  float v;

  vector<position>* poslist = tse_mm->getPositions();

  for(int j=0;j<si.size();j++) {
    fav = getValues(si.at(j));
    v = fav->values[backtest_pos];
    tse_mm->computePNLs(si.at(j),v);

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

        else if (p->size > 0 && v >= p->limit && p->limit != p->open ) {
          rmpos = REMPOS_LIMIT;
        }

        else if (p->size < 0 && v >= p->stop ) {
          rmpos = REMPOS_STOP;
        }

        else if (p->size < 0 && v >= p->vstop ) {
          rmpos = REMPOS_VSTOP;
        }

        else if (p->size < 0 && v <= p->limit && p->limit != p->open ) {
          rmpos = REMPOS_LIMIT;
        }

        if (rmpos > 0) {

          iter = tse_mm->remPosition(iter);
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
}

void btEngine::execute_() {

  std::string order;
  vector<std::string> order_params;

  if (orders_queue.pop(order,false) ) {
      logger->log("Processing Order " + order);
      order_params = split(order,':');
  }
  else return;

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
         tse_mm->remPosition(reverse_dealids[k]);
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

  else if (order_params.at(0) == "closepos") {
    std::string dealid = order_params.at(1);
    tse_mm->remPosition(dealid);       
  }
}


void btEngine::run() {

  adamresult* result = new adamresult();
  result->start = time(0);
  result->from = timestamps.values[0]; 
  result->to = iarray_last(&timestamps);

  for(int i=0;i<timestamps.size -1 ;i++) {
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