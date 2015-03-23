#include "moneymanager.h"

moneyManager::moneyManager(float cap,
                           int max_open, 
                           int max_open_pe, 
                           int rpl,
                           int rpfc,
                           float mlppt,
                           float clp,
                           float mvar,
                           AssocArray<indice*> ilist ) {
  
  capital = cap;
  max_openpos = max_open;
  max_openpos_per_epic = max_open_pe;
  max_loss_percentage_per_trade = mlppt;
  critical_loss_percentage = clp;
  max_var = mvar;

  reverse_pos_lock = rpl;
  reverse_pos_force_close = rpfc; 


  indices_list = ilist;
  cur_pnl = 0;
  var = 0;

  cumulative_pnl = 0;

}

void moneyManager::clear() {

  cur_pnl = 0;
  cumulative_pnl = 0;
  var = 0;
  positions.clear();

}

//Special compute_pnl fct without stop security,
// for backtest purposes

float moneyManager::computeWholePNLNoSec() {

  cur_pnl = 0;
  for (int i=0;i<positions.size();i++) {
    cur_pnl = cur_pnl + positions.at(i).pnl;
  }
  return cur_pnl;
}


float moneyManager::computeWholePNL() {
  cur_pnl = 0;
  for (int i=0;i<positions.size();i++) {
    cur_pnl = cur_pnl + positions.at(i).pnl;
  }

  //*** SAFE ESCAPE IN CASE OF CRITICAL LOSS */
  if (  cumulative_pnl + cur_pnl  <=  -1 * (  (critical_loss_percentage/(float) 100) * capital ) ) {
    cerr << "*CRTITICAL SHUTDOWN, LOSS IS TOO HIGH: " <<   (cumulative_pnl + cur_pnl)  << "*" << endl;
    exit(2);
  }

  return cur_pnl;
}

void moneyManager::computePNLs(string indice_name,float cur_value) {

  indice* idx = iResolve(indices_list,indice_name);
  
  int pnl_coef = idx->pnl_pp;
  int unit_coef = (idx->unit == "pip") ? 10000 : 1;

  for (int i=0;i<positions.size();i++) {

    if (positions.at(i).indice == indice_name) {
      position* p = &(positions.at(i));
      p->pnl = (cur_value - p->open) * pnl_coef * unit_coef * p->size;
      

    }
  }
}


bool moneyManager::hasPos(string dealid) {

  for(int i =0;i<positions.size();i++) {
    if (positions.at(i).dealid == dealid) {
      return true;
    }
  }
  return false;
} 


bool moneyManager::hasPos(string indice,string way) {

  if (way == "sell") {
    for(int i =0;i<positions.size();i++) {
      if (positions.at(i).indice == indice && positions.at(i).size < 0 ) {
        return true;
      }
    }
  }

  else if (way == "buy") {
    for(int i =0;i<positions.size();i++) {
      if (positions.at(i).indice == indice && positions.at(i).size > 0 ) {
        return true;
      }
    }
  }
  return false;
} 

vector<string> moneyManager::findPos(string indice ,string way) {

  vector<string> result;

  if (way == "sell") {
    for(int i =0;i<positions.size();i++) {
      if (positions.at(i).indice == indice && positions.at(i).size < 0 ) {
        result.push_back(positions.at(i).dealid);
      }
    }
  }

  else if (way == "buy") {
    for(int i =0;i<positions.size();i++) {
      if (positions.at(i).indice == indice && positions.at(i).size > 0 ) {
        result.push_back(positions.at(i).dealid);
      }
    }
  }

  return result;

}



int moneyManager::addPosition(position p) {
  positions.push_back(p);
  return 0;
}

string moneyManager::cleanPositions(vector<string> alive_pos) {

  string response = "";

  for (posList::iterator iter =  positions.begin() ;iter != positions.end();++iter) {

    position *p = &*iter;
    if ( ! contains(alive_pos,p->dealid) ) {
      response = response + p->dealid + ";";
      cumulative_pnl += p->pnl;
      iter = positions.erase(iter);
      if (iter == positions.end()) break;
    }
  }

  return response;
}


vector<position>::iterator moneyManager::remPosition(vector<position>::iterator iter) {
  
      position* p = &*iter;
      cumulative_pnl += p->pnl;
      return positions.erase(iter);

}

void moneyManager::remPosition(string dealid) {
  
  for (posList::iterator iter =  positions.begin() ;iter != positions.end();++iter) {
    position *p = &*iter;
    if (p->dealid == dealid) {
      cumulative_pnl += p->pnl;
      iter = positions.erase(iter);
      if (iter == positions.end()) break;
    }
  }
}



vector<position>* moneyManager::getPositions() {
  return &positions;
}


position* moneyManager::getPositionByDealid(string dealid) {

  for(int i=0;i< positions.size() ;i++) {

    if (positions.at(i).dealid == dealid) {
      return &(positions.at(i));
    }
    
  }
  return NULL;
}



float moneyManager::computeVAR() {

  var = 0;

  for(int i=0; i< positions.size();i++) {

    position* p = &(positions.at(i));
    indice* idx = iResolve(indices_list,p->indice);
    int pnl_coef = idx->pnl_pp; 
    int unit_coef = (idx->unit == "pip") ? 10000 : 1;


    float stop = ( p->stop - p->open );
    stop = pow(stop,2) / stop;
    var += stop * pnl_coef * unit_coef * (pow(p->size,2)/p->size);

  }

  return var;

}


int moneyManager::countPos(string indice_name) {

  int nbpos = 0;

  for(int i=0;i<positions.size();i++) {
    if ( positions[i].indice == indice_name ) nbpos++;
  }

  return nbpos;

}

void moneyManager::smartAsk(int* values,string indice_name) {

  int nbc =0;
  indice* idx = iResolve(indices_list,indice_name);
  int pnl_coef = idx->pnl_pp;
  int stop = idx->min_stop;
  int i = 0;

  while( ( (stop * nbc * pnl_coef * 100) / capital ) < max_loss_percentage_per_trade ) {

    cout << "cur_mloss:" << (stop * nbc * pnl_coef * 100) / capital << endl;

    if (nbc < stop) nbc++;

    else {

      if (i % 2 == 0) nbc++;
      else stop++;
      i++;
    }

  }

  nbc--;

  while( ( (stop * nbc * pnl_coef * 100) / capital ) < max_loss_percentage_per_trade ) {
    stop++;
  }
  
  stop--;

  values[0] = nbc;
  values[1] = stop;

}


int moneyManager::ask(string indice_name,string way,int nbc,int stop) {

  indice* idx = iResolve(indices_list,indice_name);

  int pnl_coef = idx->pnl_pp;

  float max_loss = stop * pnl_coef * nbc; 
  float max_loss_percentage = max_loss * 100 / capital;  

  if ( stop <= 0) return MM_ERR_NOSTOP; 

  //Checks maximum loss by percentage of capital.
  if (  max_loss_percentage  >= max_loss_percentage_per_trade  ) return MM_ERR_TRADERISK_2HIGH;

  //Checks maximum number of positions that can be opened simultanously
  if (positions.size() >= max_openpos) return MM_ERR_NBPOS;
  
  //Checks maximum number of positions that can be opened simultanously, per epic
  if ( max_openpos_per_epic != 0 && countPos(indice_name) >= max_openpos_per_epic ) {
    return MM_ERR_NBPOS_PE;
  }

  //checks if already have a pos which is reverse from the one we're trying to take, and apply rule
  if ( reverse_pos_lock && hasPos(indice_name,way) ) {
    return MM_ERR_REVERSE_POS_LOCK;
  }

  return 0;


}


string moneyManager::resolveError(int errnum) {

  switch(errnum) {
    case MM_ERR_NBPOS:
      return std::string(MM_ERR_NBPOS_STR);
    case MM_ERR_NBPOS_PE:
      return std::string(MM_ERR_NBPOS_PE_STR);
    case MM_ERR_TRADERISK_2HIGH:
      return std::string(MM_ERR_TRADERISK_2HIGH_STR);
    case MM_ERR_NOSTOP:
      return std::string(MM_ERR_NOSTOP_STR);
    case MM_ERR_REVERSE_POS_LOCK:
      return std::string(MM_ERR_REVERSE_POS_LOCK_STR);
  }

  return "NULL";

}


float* moneyManager::getCurPNL() {
  return &cur_pnl;
}

float* moneyManager::getCumulativePNL() {
  return &cumulative_pnl;
}

float moneyManager::getCurResult() {
  return cur_pnl + cumulative_pnl;
}


int moneyManager::getReversePosForceClose() {
  return reverse_pos_force_close;
}


void moneyManager::getPositionsString(string* s) {

  for (int i=0;i<positions.size();i++) {
    *s += "Indice:" + positions[i].indice + "\n";
    *s += "Size:" + int2string(positions[i].size) + "\n";
    *s += "Open:" + float2string(positions[i].open) + "\n";
    *s += "Stop:" + float2string(positions[i].stop) + "\n";
    *s += "Limit:" + float2string(positions[i].limit) + "\n";
    *s += "Pnl:" + float2string(positions[i].pnl) + "\n";
    *s += "+++++++++++++\n";

  }

}

void moneyManager::displayPositions() {

  for (int i=0;i<positions.size();i++) {

    cout << "Indice:" << positions[i].indice << endl;
    cout << "Size:" << positions[i].size << endl;
    cout << "Open:" << positions[i].open << endl;
    cout << "Stop:" << positions[i].stop << endl;
    cout << "Limit:" << positions[i].limit << endl;
    cout << "Pnl:" << positions[i].pnl << endl;

    cout << "+++++++++++++" << endl;

  }

}


float moneyManager::getEndResult() {
  computeWholePNLNoSec();
  return (cumulative_pnl + cur_pnl);
}



void moneyManager::addStats(adamresult* result) {

  computeWholePNLNoSec();
  result->pnl = cumulative_pnl + cur_pnl;
  result->remainingpos = positions.size();
  
}

void moneyManager::getStats(string* s) {

  computeWholePNLNoSec();

  *s = "TOTAL PNL OVER SIMULATION: " + float2string(cumulative_pnl + cur_pnl) + "\n";
  *s += "REMAINING OPEN POSITIONS:"  + int2string(positions.size()) + "\n";

  if (positions.size() > 0) {

    *s += "REMAINING POS INFOS:\n";
    *s += "--------------------\n";

    getPositionsString(s);

  }

}


void moneyManager::displayStats() {

  computeWholePNLNoSec();

  cout << "TOTAL PNL OVER SIMULATION: " << float2string(cumulative_pnl + cur_pnl) << endl;
  cout << "REMAINING OPEN POSITIONS:" << positions.size() << endl;

  if (positions.size() > 0) {

    cout << "REMAINING POS INFOS:" << endl;
    cout << "--------------------" << endl;

    displayPositions();

  }

}

//loads a previous state of Cumulative PNL
void moneyManager::loadCPNL() {

  ifstream fh (CPNL_FILE);
  string data;
  if (fh.is_open()) {
    getline(fh,data);
    if (data != "") {
      cumulative_pnl = atof(data.c_str() );
    }
  }

  fh.close();

}

//saves current state of cumulative PNL for later use
void moneyManager::saveCPNL() {

  ofstream fh (CPNL_FILE);
  if (fh.is_open()) {
    fh << cumulative_pnl;
    fh.close();
  }

}
