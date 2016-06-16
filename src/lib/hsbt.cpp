#include "hsbt.h"

/* REMAINS TO IMPLEMENT:
 1) Tradelife analysis in moneyman()
 2) All genetics handling !
*/


/* Serializes a tradestats object to JSON string */
std::string tradestats2json(tradestats& s) {

  stringstream ss;

  ss << "{";
  ss << "\"nb_long\":" << "\"" << s.nb_long << "\",";
  ss << "\"nb_short\":" << "\"" << s.nb_short << "\",";
  ss << "\"losing\":" << "\"" << s.losing << "\",";
  ss << "\"winning\":" << "\"" << s.winning << "\",";
  ss << "\"max_drawdown\":" << "\"" << s.max_drawdown << "\",";
  ss << "\"profit_factor\":" << "\"" << s.profit_factor << "\"";
  ss << "}";

  return ss.str();
}



hsbt::hsbt(qateCfg* conf,
                   broker* b,
                   backend* back,
                   cache* ca_,
                   AssocArray<indice*> ilist,
                   std::vector<strategyHandler*> sh_list,
                   moneyManager* mm,
                   genetics* ge,
                   vector<string> mlist) {


  tse_broker = b;
  tse_back = back;
  tse_cache = ca_;

  tse_strathandlers = sh_list;
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
    cout << "*ERROR: Cannot run qate in Backtest/Backtest Batch/Genetics mode without a working backend, leaving ! *" << endl;
    exit(1);
  }

  si = iGetNames(indices_list);
  si_size = si.size();

  //initializes inmem_records, backtest_inmem_records
  for (int i=0;i<si.size();i++) {
    inmem_records[si[i]] = quotek::data::records();
    backtest_inmem_records[si[i]] = quotek::data::records();
  }

}

void hsbt::init_finalize() {


  //sets the results handler
  this->qrh = new qateResultsHandler(cfg->getBTResultFile(), tse_mode);

  //loads backtest history
  if ( loadBacktestData_() == 0 ) {
    std::cerr << "[ERROR] No records found for backtest, quitting.." << std::endl;
    exit(1);
  }

  //initializes btfilter
  btfilter = false;
  btfilter_from = 0;
  btfilter_to = 0;

  btf b_ = tse_strathandlers[0]->getBTFilter();
  btfilter = b_.enable;
  btfilter_from = b_.from;
  btfilter_to = b_.to;
  btfilter_skip = b_.skip;

  //std::thread tload( [this] {} );

  //initializes logger
  this->logger = new igmLogger();
  
  std::cout << "loading evaluators.." << std::endl;

  for (int j=0;j<tse_strathandlers.size();j++) {

    void* strat_ptr = tse_strathandlers[j]->getExportFct();
    std::regex asset_match(tse_strathandlers[j]->getAssetMatch());

    for (int i=0;i<si.size();i++) {

      if (strat_ptr &&  std::regex_match(si.at(i), asset_match) ) {

        algo al;
        cout << "loading eval for indice " << si.at(i)  << endl;
        al.eval_ptr = strat_ptr;
        al.eval_name = si.at(i);
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
    st->portfolio = this->getMoneyManager()->getPositionsPtr();
	st->back = tse_back;
    st->asset_name = algos[i].eval_name;
    st->identifier = algos[i].strategy + "@" + algos[i].eval_name;
    st->store = &tse_store;

    //initializes algo.
    st->initialize();

    //append strat to list.
    strategies.emplace_back(st);


  }
}


// Loads indices history from backend to memory

int hsbt::loadBacktestData_() {

    std::cout << "loading backtest data from " << backtest_from << " to " << backtest_to << std::endl;

    int bt_time_size = backtest_to - backtest_from;
    int step = (bt_time_size < 100000 ) ? bt_time_size : 100000;
    vector<string> inames = iGetNames(indices_list);

    int nb_iter = (bt_time_size / step);
    int nbrecs = 0;

    auto tt0 = std::chrono::high_resolution_clock::now();

    
    for (int i=0; i< inames.size(); i++) {

      quotek::data::records recs;
        
      //Cache lookup
      if (tse_cache != NULL) {
        recs = tse_cache->query(inames[i],backtest_from,backtest_to);
      }
      //We found data in cache, so we use it!
      if (recs.size() != 0) {
        nbrecs += recs.size();
        backtest_inmem_records[inames[i]].append(recs);
      }
      
      //Failback to storage backend queries
      else {
        for ( int j=0; j < nb_iter; j++) {
          int from = backtest_from + ( step * j );
          int to =  from + step ; 
          if ( to > backtest_to  ) to = backtest_to;

          recs = tse_back->query(inames[i],from,to);
          nbrecs += recs.size();
          backtest_inmem_records[inames[i]].append(recs);
        }

        //storage to cache if needed.
        if (tse_cache != NULL) {
          tse_cache->store(inames[i], backtest_from, backtest_to, backtest_inmem_records[inames[i]]);
        }

      }

    }
    
    auto tt1 = std::chrono::high_resolution_clock::now();
    auto elapsed_t = tt1 - tt0;
    uint64_t elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_t).count();
    float elapsed_secs = elapsed / (float) 1000;

    std::cout << "Loaded " << nbrecs << " Backtest records in  " << elapsed_secs  << "s" << std::endl; 

    //WE FLAG BT STATE AS FULLY LOADED
    

    //DEBUG (display loaded data)
    /*for (int i=0;i< backtest_inmem_records[0].size();i++) {
      std::cout << "BIR:" << backtest_inmem_records[0][i] << std::endl;
    }
    */

    //Tests if there is data loaded
    int has_recs = 0 ;
    for (int i = 0;i<  backtest_inmem_records.Size(); i++  ) {

      if ( backtest_inmem_records[i].size() > 0 ) {
        has_recs = 1;
        break;
      }
    }


    return has_recs;
}


int hsbt::evaluate_(strategy* s) {

  std::string logstr;
  std::string order;

  quotek::data::record& last_rec = s->recs->last();
  s->value = last_rec.value;
  s->spread = last_rec.spread;
  s->t = last_rec.timestamp;

  //user algo tick evaluation.
  s->__evaluate__();

  while ( s->orders_queue.pop(order,false) ) {
    this->orders_queue.push(order);
  }

  //flushing of logs
  s->flushlogs();

  while( s->log_queue.pop(logstr,false) ) {
    logger->log(logstr, progress_tstamp);
  }

  return 0;

}

void hsbt::moneyman_() {

  float v;
  
  quotek::data::cvector<quotek::core::position>& poslist = tse_mm->getPositions();

  for(int i=0;i<si_size;i++) { 
    v = inmem_records[si.at(i)][backtest_pos].value;
    tse_mm->computePNLs(si.at(i),v);
  }

  //close positions where limit/stop is reached
  for(vector<quotek::core::position>::iterator iter = poslist.begin(); 
        iter != poslist.end();
        ++iter) {

    if ( poslist.size() == 0 ) break;

    quotek::core::position p0 = *iter;
    quotek::core::position* p = &p0;

    v = inmem_records[p->asset_name][backtest_pos].value;

    float vstop = p->get_vstop();
    float vlimit = p->get_vlimit();
    int rmpos = 0;

    //** LONG POS RM RULES **//
    if (p->size > 0) {

      if (v <= p->stop) rmpos = REMPOS_STOP;
      else if (v <= vstop) rmpos = REMPOS_VSTOP;
      else if (v >= p->limit && p->limit != p->open ) rmpos = REMPOS_LIMIT;
      else if (v >= vlimit && vlimit != p->open ) rmpos = REMPOS_VLIMIT;

    }

    //** SHORT POS RM RULES **//
    else {
        
      if (v >= p->stop ) rmpos = REMPOS_STOP;
      else if (v >= vstop ) rmpos = REMPOS_VSTOP;
      else if (v <= p->limit && p->limit != p->open ) rmpos = REMPOS_LIMIT;
      else if (v <= vlimit && vlimit != p->open ) rmpos = REMPOS_VLIMIT;

    }

    if (rmpos > 0) {

      p->close_date = inmem_records[0].get_data()[backtest_pos].timestamp;
      positions_history.push_back(p0);
      iter = tse_mm->remPosition(iter);

      string logstr = "POS " + p->asset_name + " Removed";

      if (rmpos == REMPOS_STOP) logstr = logstr + " (STOP)";
      else if (rmpos == REMPOS_VSTOP) logstr = logstr + " (VSTOP)";
      else if (rmpos == REMPOS_LIMIT) logstr = logstr + " (LIMIT)";
      else if (rmpos == REMPOS_VLIMIT) logstr = logstr + " (VLIMIT)";

      logger->log(logstr, progress_tstamp);
      if (iter == poslist.end() ) break;

    }
  }
}

void hsbt::execute_() {

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

    std::string id = order_params.at(6);

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
         quotek::core::position* pinv = tse_mm->getPositionByDealid(reverse_dealids[k]);
         pinv->close_date = inmem_records[0].get_data()[backtest_pos].timestamp;
         positions_history.push_back(*pinv);
         //removes inverse pos.
         tse_mm->remPosition(reverse_dealids[k]);
       }
     }

       quotek::core::position p;
       p.identifier = id;
       p.asset_id = epic;
       p.asset_name = indice;
       p.ticket_id = randstring(8);
       
       
       if (way == "sell") {
         p.open = getAssetRecords(indice).get_data()[backtest_pos].value;
         p.size = -1 * nbc;
         p.stop = p.open + stop;
         p.set_vstop(p.stop);
         p.limit = p.open - limit;
         p.set_vlimit(p.limit);
       }

       else {
         p.open = getAssetRecords(indice).get_data()[backtest_pos].value;
         p.size = nbc;
         p.stop = p.open - stop;
         p.set_vstop(p.stop);
         p.limit = p.open + limit;
         p.set_vlimit(p.limit);
       }

       p.pnl = 0;
       p.open_date = inmem_records[0].get_data()[backtest_pos].timestamp;
       tse_mm->addPosition(p);
      
    }

    else {
      logger->log("Position refused by moneymanager (" + tse_mm->resolveError(mm_answer) + ")", progress_tstamp);
    }
  }

  else if (order_params.at(0) == "closepos") {
    std::string dealid = order_params.at(1);

    //stores position to be removed to the positions history.
    quotek::core::position* pinv = tse_mm->getPositionByDealid(dealid);
    pinv->close_date = inmem_records[0].get_data()[backtest_pos].timestamp;
    positions_history.push_back(*pinv);

    tse_mm->remPosition(dealid);       
  }

  else if (order_params.at(0) == "closeallpos") {

    string indice = order_params.at(1);
    string way = order_params.at(2);

    vector<string> dealids = tse_mm->findPos(indice,way);

    for (int k=0;k<dealids.size();k++) {

      quotek::core::position* pinv = tse_mm->getPositionByDealid(dealids[k]);
      pinv->close_date = inmem_records[0].get_data()[backtest_pos].timestamp;
      positions_history.push_back(*pinv);
      
      tse_mm->remPosition(dealids[k]);
    }

  }

}


void hsbt::reset() {
  
  this->backtest_pos = 0;
  this->progress_tstamp = 0;
  this->backtest_progress = 0;

  this->positions_history.clear();

  this->tse_mm->reset();
  this->logger->reset();

  for ( int i=0; i<  this->inmem_records.Size() ; i++  )  {
       this->inmem_records[i].get_data().clear();   
  }

}

qateresult* hsbt::run() {

  qateresult* result = new qateresult();

  result->start = time(0);
  result->from = backtest_from; 
  result->to = backtest_to;

  int bpp = 0;
  float bt_realsize = backtest_inmem_records[0].size() - 1;
  int nb_assets = backtest_inmem_records.Size();
  int nb_algos = strategies.size();

  //benchmarking purposes
  auto tt0 = std::chrono::high_resolution_clock::now();

  //*** START OF CRITICAL SECTION LOOP **** //
  for(int i=0;i<bt_realsize;i++) {

    //move data from backtest_inmem to inmem
    //CAN BE HACKED/OPTIMIZED !!!
    for ( int j=0; j< nb_assets; j++  )  {
      this->inmem_records[j].append(this->backtest_inmem_records[j][i]);
    }

    this->progress_tstamp = this->backtest_inmem_records[0][this->backtest_pos].timestamp ;

    if ( btfilter ) {
      
      if ( this->progress_tstamp % btfilter_skip != 0 ) goto skipexec;

      int chour = quotek::core::time::timeint(this->progress_tstamp)[0];
      if ( chour < btfilter_from || chour >= btfilter_to ) {
        goto skipexec;
      }

    }

    for (int k=0;k< nb_algos;k++) {
      evaluate_(strategies[k]);
    }

    moneyman_();
    execute_();
 

skipexec: this->backtest_pos++;
    //Computes backtest Progress.
    this->backtest_progress =  this->backtest_pos / bt_realsize * 100;

    /* We want disable this section of code that is in the critical path. (at least optimize ) */
    if (this->backtest_progress % 10 == 0 && this->backtest_progress > bpp) {
      bpp = this->backtest_progress;
      logger->log("Backtest Progress: " + int2string(backtest_progress) + "%", progress_tstamp);
      std::cout << "Backtest Progress: " << int2string(backtest_progress) << "%" << std::endl;
    
    }

  }
  //*** END CRITICAL SECTION LOOP ***//

  //computes high res backtest end time, for benchmarking purposes.
  auto tt1 = std::chrono::high_resolution_clock::now();
  auto elapsed_t = tt1 - tt0;
  uint64_t elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_t).count();
  float elapsed_secs = elapsed / (float) 1000;

  //puts duration in result.
  result->duration = elapsed_secs;


  //Adds remaining pos to history
  quotek::data::cvector<quotek::core::position>& rpos = tse_mm->getPositions();
  result->remainingpos = rpos.size();

  for (int i=0;i<result->remainingpos;i++) {
    rpos.at(i).close_date = inmem_records[0][inmem_records[0].size() - 1].timestamp;
    positions_history.push_back(rpos.at(i));
  }

  //Adds trade stats to result
  addTradeStats(result);
  //Adds logs to result
  addLogStats(result);
  result->positions_history = positions_history; 

  cout << endl<< "* Backtest Finished in " << elapsed_secs << "s *" << endl;
 
  tse_mm->addStats(result);

  return result;

}


void hsbt::runNormal() {

  qateresult* result = this->run();
  this->qrh->entries.emplace_back(result);
  this->qrh->save();

}


void hsbt::runBatch() {

    qateresult* result;
    std::vector<std::string> representation;

    AssocArray<std::vector<quotek::data::any> > universe;
    
    //initalizes current iter
    this->current_iter = 0;

    //we must generate the universe before using it !
    this->createBatchUniverse(universe);

    std::vector<int> indices;
    
    //compute how many iters;
    int total_iters = 0;
    for (int i=0; i< universe.Size();i++  ){

      indices.push_back(0);

      if (total_iters == 0) total_iters = universe[i].size();
      else total_iters *= universe[i].size();
    }


    for (int i=0;i< total_iters;i++) {

      representation.clear();

      
      //Here we place the correct values inside the strats store entry
      //and compute batch representation
      for (int k = 0; k < indices.size(); k++) {
        tse_store[universe.GetItemName(k)] = universe[k][indices[k]];
        representation.emplace_back( this->batch2string(universe.GetItemName(k),universe[k][indices[k]]) );
      }

      std::cout << "Batch testing with [";
      for (auto repr_ : representation) std::cout << repr_ << ",";
      std::cout << "]\n";

      //HERE WE GO!!
      result = this->run();

      result->batch_repr = representation;

      //Then we save result (temporary advancement)
      this->qrh->entries.emplace_back(result);
      this->qrh->save();

      //then we reset hsbt for next iter
      this->reset();

      this->current_iter++;
      this->backtest_global_progress = ( (float) this->current_iter / (float) total_iters) * 100.0;

      indices[ indices.size() -1 ] ++;
      for ( int j= indices.size() -1; j > 0; j--  ) {
        if (  indices[j] == universe[j].size() ) {
          indices[j] = 0;
          indices[j-1] ++;
        }
      }

    }

    return;
}


void hsbt::runGenetics() {

  this->backtest_global_progress = 0;
  this->current_iter = 0;
  int total_iters = 0; 

  qateresult* result;

  //get //#gene directives and parse them.
  std::vector< std::vector<std::string> > gene_dirs = tse_strathandlers[0]->getGeneDirectives();
  for (auto gene_dir: gene_dirs) {
    tse_ge->parseGene(gene_dir);
  }

  std::cout << "Initializing first genetics population.." << std::endl;
  tse_ge->initPopulation();

  total_iters = tse_ge->getMaxGenerations() * tse_ge->population.size();
  std::cout << tse_ge->population.size() << "IVs generated." << std::endl;

  for (int i=0;i< tse_ge->getMaxGenerations();i++) {
    
    for (int j=0; j< tse_ge->population.size();j++) {

      individual* iv = &(tse_ge->population[j]);

      for (int k=0; k < iv->attributes.Size(); k++  ) {
        this->tse_store[ iv->attributes.GetItemName(k) ] = iv->attributes[k];
      }

      //HERE WE GO!!
      result = this->run();

      result->generation_id = i;
      result->individual_id = j;

      result->genes_repr = tse_ge->serializeIV(iv);

      iv->result = result->pnl;

      //Then we save result (temporary advancement)
      this->qrh->entries.emplace_back(result);
      this->qrh->save();

      //then we reset hsbt for next iter
      this->reset();

      this->current_iter++;
      this->backtest_global_progress = ( (float) this->current_iter / (float) total_iters) * 100.0;

      //we are OK !
      if (tse_ge->converges() ) { 
        backtest_global_progress = 100;
        return;
      }

    }

    std::cout << "Population " << i << " tested, generating new one !" << std::endl;
    tse_ge->newgen();

  }


  return;
}





int hsbt::getBacktestPos() {
  return backtest_pos;
}

int hsbt::getBacktestProgress() {
  return backtest_progress;
}


tradestats hsbt::compute_tradestats() {

  tradestats result;
  memset(&result,0x00,sizeof(result));

  float total_gains =0;
  float total_losses = 0;
  float cmax= -1000000000;
  float cmin = 1000000000;
  float ctpnl = 0;


  for (int i=0;i<positions_history.size();i++) {

    if ( positions_history[i].size > 0  ) result.nb_long++;
    else result.nb_short++;

    
    ctpnl += positions_history[i].pnl;
  
    if ( ctpnl > cmax ) {
      cmax = ctpnl;
      cmin = 10000000000;
    }
    if ( ctpnl < cmin ) cmin = ctpnl;

    if ( positions_history[i].pnl > 0 ) {
      total_gains += positions_history[i].pnl;
      result.winning++;
    }

    else if (positions_history[i].pnl <= 0 ) {
      total_losses += positions_history[i].pnl;
      result.losing++;
    }
  }

  //computed values are significant only if trades occured !
  if (positions_history.size() > 0) {
    result.max_drawdown = cmax - cmin ;
    result.profit_factor = total_gains / fabs(total_losses);
  }

  result.pnl = ctpnl;
  return result;

}



std::string hsbt::snapshot() {

  //makes a snapshot of the trade statistics
  tradestats ts1 = compute_tradestats();

  std::stringstream ss;

  //We send backtest progress if single BT
  if ( tse_mode == QATE_MODE_BACKTEST  ) ss << "{\"progress\":" << "\"" << backtest_progress << "\",";

  //Else we send backtest_global_progress
  else ss << "{\"progress\":" << "\"" << backtest_global_progress << "\",";

  ss << "\"tradestats\":" << tradestats2json(ts1);
  ss << ",\"pnl\": \"" << ts1.pnl << "\",";
  ss << "\"histgraph\":[";

  float ppnl = 0;
  for (int i=0;i< positions_history.size();i++) {

    ppnl += positions_history[i].pnl;
    ss << "[" << positions_history[i].close_date * 1000 << "," << ppnl << "]";
    if (i < positions_history.size() -1 ) ss << ",";
  
  }

  ss << "]";

  ss << "}";  


  return ss.str();
}

void hsbt::setBacktestPos(int bpos) {
  backtest_pos = bpos;
}

void hsbt::setBacktestProgress(int bprogress) {
  backtest_progress = bprogress;
}


void hsbt::addLogStats(qateresult* result) {
  vector<log_entry>* lentries = logger->getAllEntries();
  for (int i=0;i<lentries->size();i++) {
    result->loglines.push_back(lentries->at(i).entry);
  }
}

void hsbt::addTradeStats(qateresult* result) {
  tradestats ts1 = compute_tradestats() ;
  result->pnl = ts1.pnl;
  result->winning_trades = ts1.winning ;
  result->losing_trades = ts1.losing;
  result->profit_factor = ts1.profit_factor;
  result->max_drawdown = ts1.max_drawdown;

}


std::string hsbt::batch2string(std::string bname, quotek::data::any value) {
  
  stringstream result;
  
  std::vector< std::vector<std::string> > batch_dirs = tse_strathandlers[0]->getBatchDirectives();

  for (auto bdir: batch_dirs) {

    if ( bdir[1]  == bname ) {
      
      if ( bdir[0] == "int" ) {
        int tmp_ = value;
        result << bdir[1] << "=" << tmp_;
      }

      else if (bdir[0] == "float") {
        float tmp_ = value;
        result << bdir[1] << "=" << tmp_;
      }

      else if (bdir[0] == "string") {
        std::string tmp_ = value;
        result << bdir[1] << "=" << tmp_ ;
      }
      break;
    }
  }
  return result.str();
}

void hsbt::createBatchUniverse(AssocArray<std::vector<quotek::data::any> > &universe ) {

  std::vector< std::vector<std::string> > batch_dirs = tse_strathandlers[0]->getBatchDirectives();

  for ( int i= 0; i < batch_dirs.size(); i++  ) {
  
    if ( batch_dirs[i][0] == "int" ) {

      if ( batch_dirs[i].size() ==  5 ) {
        
        int a = std::stoi(batch_dirs[i][2]);
        int b = std::stoi(batch_dirs[i][3]);
        int c = std::stoi(batch_dirs[i][4]);

        for (int j= a; j < b; j= j+ c) {
          universe[batch_dirs[i][1]].emplace_back(j);
        }
      }

      else if (batch_dirs[i].size() == 4) {

        int a = std::stoi(batch_dirs[i][2]);
        int b = std::stoi(batch_dirs[i][3]);

        for (int j= a; j < b; j++) {
          universe[batch_dirs[i][1]].emplace_back(j);
        }

      }
  
      else if (batch_dirs[i].size() == 3) {

        std::vector<std::string> intlist = split(batch_dirs[i][2],',');

        for (auto lint_ : intlist) {
          universe[batch_dirs[i][1]].emplace_back(std::stoi(lint_)); 
        }

      }


    }

    else if ( batch_dirs[i][0] == "float" ) {

      if ( batch_dirs[i].size() ==  5 ) {
        
        float a = std::stof(batch_dirs[i][2]);
        float b = std::stof(batch_dirs[i][3]);
        float c = std::stof(batch_dirs[i][4]);

        for (float j= a; j < b; j= j+ c) {
          universe[batch_dirs[i][1]].emplace_back(j);
        }
      }

      else if (batch_dirs[i].size() == 3) {

        std::vector<std::string> floatlist = split(batch_dirs[i][2],',');

        for (auto lfloat_ : floatlist) {
          universe[batch_dirs[i][1]].emplace_back(std::stof(lfloat_)); 
        }

      }
    }

    else if ( batch_dirs[i][0] == "string" ) {

      if (batch_dirs[i].size() == 3) {

        std::vector<std::string> strlist = split(batch_dirs[i][2],',');

        for (auto lstring_ : strlist) {
          universe[batch_dirs[i][1]].emplace_back(lstring_);
        }

      }
    }

  }  


}
