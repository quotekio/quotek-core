#include "btengine.h"

/* REMAINS TO IMPLEMENT:
 1) Tradelife analysis in moneyman()
 2) All genetics handling !
*/

btEngine::btEngine(adamCfg* conf,
                   broker* b,
                   backend* back,
                   AssocArray<indice*> ilist,
                   strategy* s,
                   moneyManager* mm,
                   genetics* ge,
                   vector<string> mlist) {


  tse_broker = b;
  tse_back = back;
  tse_strat = s;
  tse_mm = mm;
  tse_ge = ge;
  indices_list = ilist;
  modules_list = mlist;

  cfg = conf;
  tse_mode = conf->getMode();
  backtest_from = conf->getBFrom();
  backtest_to = conf->getBTo();
  backtest_pos = 0;
  backtest_progress = 0;

  if (tse_back == NULL) {
    cout << "*ERROR: Cannot run adam in Backtest or genetics mode without a working backend, leaving ! *" << endl;
    exit(1);
  }


  //initializes inmem_records, backtest_inmem_records
  vector<string> si = iGetNames(indices_list);
  for(int i=0;i<si.size();i++) {
    inmem_records[si[i]] = (records*) malloc(sizeof(records));
    backtest_inmem_records[si[i]] = (records*) malloc(sizeof(records));
    //initializing records structures for each found indice
    records_init(inmem_records[si[i]],10000);
    records_init(backtest_inmem_records[si[i]], 10000);

  }

  //loads backtest history
  loadBacktestData_();

  //initializes logger
  logger = new igmLogger();

  //initializes store
  store_init(&tse_store);
  
  printf ("loading evaluators..\n");
  vector<string> evnames = iGetNames(getIndicesList());

  for (int i=0;i<evnames.size();i++) {
    void* evptr = tse_strat->resolveFunction(evnames.at(i),"EVAL");
    if (evptr) {
      cout << "loading eval for indice "  << evnames.at(i)  << endl;
      eval_pointers[evnames.at(i)] = evptr;
    }
  }

}

// Loads indices history from backend to memory
int btEngine::loadBacktestData_() {

    string q;
    vector<string> inames = iGetNames(indices_list);
    for (int i=0;i<inames.size();i++) {
      records* recs = tse_back->query(inames[i], backtest_from, backtest_to);
      backtest_inmem_records[inames[i]] = recs;
    }

  return 0;
}


void btEngine::evaluate_(string eval_name,void* eval_ptr) {

  typedef void* (*eval_fct)(uint32_t,float,float,evaluate_io*);

  uint32_t t;
  float v;
  float spread;
  evaluate_io ev_io;

  eval_fct f = (eval_fct) eval_ptr;

  ev_io.ans = (char*) malloc(256 * sizeof(char) +1);
  ev_io.log_s = (char*) malloc(1024 * sizeof(char) +1);
  ev_io.evmio_a = &evmio_a;
  ev_io.s = &tse_store;
  ev_io.genes = tse_genes;

  string ans_str;
  string log_str;
  indice* idx = iResolve(indices_list,eval_name);

  ev_io.indice_name = eval_name.c_str();
  ev_io.ans[0] = '\0';
  ev_io.log_s[0] = '\0';
  ev_io.recs = getIndiceRecords(eval_name);

  record* r = records_last(ev_io.recs);

  t = r->timestamp;
  v = r->value;
  spread = r->spread;
  //debug
  //cout << "SIMUTIME:" << epochToDateTime(t) << endl;

  if ( eval_running(idx,t) == 1) {
    //execution of found eval function
    (*f)(t,v,spread, &ev_io);

    ans_str = std::string(ev_io.ans);
    log_str = std::string(ev_io.log_s);

    if (log_str != "") {
         logger->log(log_str, progress_tstamp);
    }

    if ( ans_str != "") {
      orders_queue.push(ans_str);
    }
  }

}

void btEngine::moneyman_() {

  vector<string> si = iGetNames(indices_list);
  records* recs;
  float v;

  //TRADELIFE struct for fctptr
  typedef void* (*tl_fct)(pos_c*,tradelife_io*);
  void* tl_fct_fref = tse_strat->getTLFct(); 

  tradelife_io tl_io;

  vector<position>* poslist = tse_mm->getPositions();

  if ( tl_fct_fref != NULL) {

    tl_io.ans = (char*) malloc(1024* sizeof(char));
    tl_io.log_s = (char*) malloc(1024* sizeof(char));
    tl_io.s = &tse_store;

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
        orders_queue.push(tl_io.ans);
      }
       
      if (std::string(tl_io.log_s) != "" ) {
        logger->log(tl_io.log_s, progress_tstamp);
      }
 
    }

    free(tl_io.ans);
    free(tl_io.log_s);

  }

  for(int j=0;j<si.size();j++) {
    
    recs = getIndiceRecords(si.at(j));
    v = recs->data[backtest_pos].value;

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

        else if (p->size > 0 && v >= p->vlimit && p->vlimit != p->open ) {
          rmpos = REMPOS_VLIMIT;
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

        else if (p->size < 0 && v <= p->vlimit && p->vlimit != p->open ) {
          rmpos = REMPOS_VLIMIT;
        }
        



        if (rmpos > 0) {
 
          p->close_time = inmem_records[0]->data[backtest_pos].timestamp;
          positions_history.push_back(p0);
          iter = tse_mm->remPosition(iter);

          string logstr = "POS " + p->indice + " Removed";

          if (rmpos == REMPOS_STOP) logstr = logstr + " (STOP)";
          else if (rmpos == REMPOS_VSTOP) logstr = logstr + " (VSTOP)";
          else if (rmpos == REMPOS_LIMIT) logstr = logstr + " (LIMIT)";
          else if (rmpos == REMPOS_VLIMIT) logstr = logstr + " (VLIMIT)";

          logger->log(logstr, progress_tstamp);
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
      logger->log("Processing Order " + order, progress_tstamp);
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

    logger->log("Opening Position on " + epic, progress_tstamp);

    int mm_answer = tse_mm->ask(indice,way,nbc,stop);
  
    if (mm_answer == 0) {

     if (tse_mm->getReversePosForceClose() == 1) {

       string reverse_way = ""; 
       vector<string> reverse_dealids;

       if (way == "sell") reverse_way = "buy";
       else reverse_way = "sell";

       reverse_dealids = tse_mm->findPos(indice,reverse_way);
       for (int k=0;k<reverse_dealids.size();k++) {

         //Store pos to be removed to the postions history.
         position* pinv = tse_mm->getPositionByDealid(reverse_dealids[k]);
         pinv->close_time = inmem_records[0]->data[backtest_pos].timestamp;
         positions_history.push_back(*pinv);
         //removes inverse pos.
         tse_mm->remPosition(reverse_dealids[k]);
       }
     }

       position p;
       p.epic = epic;
       p.indice = indice;
       p.dealid = randstring(8);
       p.nb_inc = 1;
       
       if (way == "sell") {
         p.open = getIndiceRecords(indice)->data[backtest_pos].value;
         p.size = -1 * nbc;
         p.stop = p.open + stop;
         p.vstop = p.stop;
         p.limit = p.open - limit;
         p.vlimit = p.limit;
       }

       else {
         p.open = getIndiceRecords(indice)->data[backtest_pos].value;
         p.size = nbc;
         p.stop = p.open - stop;
         p.vstop = p.stop;
         p.limit = p.open + limit;
         p.vlimit = p.limit;
       }

       p.pnl = 0;
       p.status = POS_OPEN;
       //adds position to the backtest history.
       p.open_time = inmem_records[0]->data[backtest_pos].timestamp;
       tse_mm->addPosition(p);
      
    }

    else {
      logger->log("Position refused by moneymanager (" + tse_mm->resolveError(mm_answer) + ")", progress_tstamp);
    }
  }

  else if (order_params.at(0) == "closepos") {
    std::string dealid = order_params.at(1);

    //stores position to be removed to the positions history.
    position* pinv = tse_mm->getPositionByDealid(dealid);
    pinv->close_time = inmem_records[0]->data[backtest_pos].timestamp;
    positions_history.push_back(*pinv);

    tse_mm->remPosition(dealid);       
  }

  else if (order_params.at(0) == "closeallpos") {

    string indice = order_params.at(1);
    string way = order_params.at(2);

    vector<string> dealids = tse_mm->findPos(indice,way);

    for (int k=0;k<dealids.size();k++) {
      tse_mm->remPosition(dealids[k]);
    }

  }

}


adamresult* btEngine::run() {

  adamresult* result = new adamresult();
  result->start = time(0);
  result->from = backtest_from; 
  result->to = backtest_to;

  int bpp = 0;

  for(int i=0;i<backtest_inmem_records[0]->size -1 ;i++) {

    //displace data from backtest_inmem to inmem
    for ( int j=0; j< backtest_inmem_records.Size(); j++  )  {
      records_push( inmem_records[j], backtest_inmem_records[j]->data[i]);
    }

    progress_tstamp = backtest_inmem_records[0]->data[backtest_pos].timestamp ;

    for (int k=0;k<eval_pointers.Size();k++) {
      evaluate_(eval_pointers.GetItemName(k), eval_pointers[k] );
    }

    moneyman_();
    execute_();
 
    backtest_pos++;

    //Computes backtest Progress.
    backtest_progress =  ( (float) backtest_pos / (float) ( backtest_inmem_records[0]->size -1) ) * 100;
    //cout << backtest_progress << endl;
    if (backtest_progress % 10 == 0 && backtest_progress > bpp) {
      bpp = backtest_progress;
      logger->log("Backtest Progress: " + int2string(backtest_progress) + "%", progress_tstamp);
    }

  }

  //completes end timestamp
  result->stop = time(0);

  //Adds remaining pos to history
  vector<position>* rpos = tse_mm->getPositions();
  result->remainingpos = rpos->size();

  for (int i=0;i<result->remainingpos;i++) {
    rpos->at(i).close_time = inmem_records[0]->data[backtest_pos].timestamp;
    positions_history.push_back(rpos->at(i));
  }

  //adds CFD/whatever assets statistics like
  //highest, lowest,variance, etc..
  addAStats(result); 
  addLogStats(result);
  result->positions_history = positions_history;  

  cout << endl;
  cout << "=================" << endl;
  cout << "BACKTEST FINISHED" << endl;
  cout << "=================" << endl << endl;

  tse_mm->addStats(result);
  return result;
}

adamGeneticsResult* btEngine::runGenetics() {
  
  adamGeneticsResult* result = new adamGeneticsResult();

  cout << "Initializing Population.." << endl;
  tse_ge->initPopulation();

  int gen = 0;
  char bt_status[128];
  int bt_adv;
  int bt_adv_dlock;


  while(1) {

    for (int i=0;i < tse_ge->getPopulationSize();i++) {

      individual* iv = tse_ge->getIndividualFromPopulation(i);
      adamresult* grentry;

      if ( tse_ge->mustCompute(iv) ) {

        cout << "Processing Generation #" << gen << ", individual #" << i << ".." <<endl; 
        store* gstore = &(iv->attributes);
        setGeneticsStore(gstore);

        bt_adv = 0;
        bt_adv_dlock = 0;

        //reinit backtest variables
        backtest_pos = 0;
        backtest_progress = 0;

        //clear logs
        logger->clear();
        
        //runs backtest with current genes store and fetch back result
        grentry = run();
        grentry->generation_id = gen;
        grentry->individual_id = i;
        grentry->genes_repr = tse_ge->serializeIV(iv);

        result->entries.push_back(grentry);
        iv->result = tse_mm->getEndResult();

        //Clean components for next iteration
        store_clear(getStore());
        tse_mm->clear();
        positions_history.clear();

      }
    }


    gen++;
    if (  tse_ge->getMaxGenerations() != 0 &&  gen >= tse_ge->getMaxGenerations()  ) {
      cout << "Genetics Maximum number of generations reached" << endl;
      return result;
    }

    else if (tse_ge->converges() ) {
      cout << "Genetics Convergence achieved !" << endl;
      return result;
    }

    tse_ge->newgen();

  }

  return result;

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


void btEngine::addAStats(adamresult* result) {

  for(int i=0;i< inmem_records.Size();i++ ) {

    assetstats* a1 = new assetstats();
    a1->name = inmem_records.GetItemName(i);
    a1->variation = percentDelta(inmem_records[a1->name]);
    a1->deviation = stdDeviation(inmem_records[a1->name]);
    a1->highest = max(inmem_records[i]);
    a1->lowest = min(inmem_records[i]);
    
    result->astats.push_back(a1);


  }
}

void btEngine::addLogStats(adamresult* result) {
  vector<log_entry>* lentries = logger->getAllEntries();
  for (int i=0;i<lentries->size();i++) {
    result->loglines.push_back(lentries->at(i).entry);
  }

}
