#include "aep.h"
#include "version.h"

#include <quotek/core.hpp>

std::map<std::string, ws_token> ws_tokens_list;

std::string aep_answer(networkSession* nsession, std::string data,tsEngine* t0) {

  //std::cout << "DATA:" << data << std::endl;
  
  std::string res = "";

  if (data == "corestats\r\n") {
    res = aep_corestats(t0);
  }

  else if ( data == "algos\r\n") {
    res = aep_algos(t0);
  }

  else if (data == "poslist\r\n") {
    res = aep_poslist(t0);
  }
  
  else if (data == "btprogress\r\n") {
    res = aep_btprogress(t0);
  }

  else if (data == "version\r\n") {
    res = aep_version(t0);
  }

  else if (data == "wstoken\r\n" ) {
    res = aep_wstoken(nsession, t0);
  }

  else {

    data = trim(data);
    vector<string> query_args = split(data,' ');

    if (query_args.size() > 0 ) {
      if ( query_args[0] == "lastlogs" ) {
        int ne = atoi( query_args[1].c_str() ) ;
        res = aep_lastlogs(t0,ne);
      }

      else if (query_args[0] == "order") {
        res = aep_order(t0,query_args);
      }

    }
  }
  

  return res;
}

std::string aep_corestats(tsEngine* t0) {
  
  std::string ret;
  moneyManager* mm; 

  if (!t0) {
    cerr << "*CRITICAL: cannot get tsEngine, pointer is NULL!" << endl;
    return ret; 
  }

  mm = t0->getMoneyManager();

  if (!mm) {
    cerr << "*CRITICAL: cannot get moneyManager!" << endl;
    return ret;
  }

  ret = "{";
  ret += "\"unrealized_pnl\":\"" + float2string(mm->getUnrealizedPNL())  + "\",";
  ret += "\"nbpos\":\"" + int2string( mm->getPositions().size() ) + "\"";
  ret += "}"; 
  
  return ret;
}

std::string aep_version(tsEngine* t0) {
  
  std::string ret;

  ret = "{";
  ret += "\"version\":\"" +  std::string(QATE_VERSION)  +  ", " + quotek::core::version() + "\"";
  ret += "}"; 

  return ret;

}

std::string aep_algos(tsEngine* t0) {

  moneyManager* mm = t0->getMoneyManager();

  stringstream ret;
  std::vector<algo> running_algos = t0->getAlgos();

  //computes algos pnl;
  quotek::data::cvector<quotek::core::position>& plist = mm->getPositions();

  for (int i=0;i<plist.size();i++) {

    for ( int j=0;j<running_algos.size();j++ ) {
  
      std::string algo_id = running_algos[j].strategy + "@" + running_algos[j].eval_name;

      if ( plist[i].identifier == algo_id ) {
        running_algos[j].pnl += plist[i].pnl;
      }
    }
  }

  ret << "[" ;

  for (int i = 0; i< running_algos.size();i++) {
    
    ret << "{\"identifier\":\"" << running_algos[i].strategy << "@" <<  running_algos[i].eval_name << "\",";
    ret << "\"pnl\":\"" << running_algos[i].pnl << "\"}";

    if ( i < running_algos.size() - 1 ) ret << ",";

  }

  ret << "]";
  return ret.str();

}

std::string aep_order(tsEngine* t0, std::vector<string> args) {

  string order = "";
  for (int i=1;i<args.size();i++) {
    order += args[i] + " ";
  }
  order = trim(order);
  t0->getOrdersQueue()->push(order);

  return "OK";

}

std::string aep_wstoken(networkSession* nsession, tsEngine* t0) {

  

  return "";

}


std::string aep_poslist(tsEngine* t0) {

  moneyManager* mm = t0->getMoneyManager();
  std::string ret = "[";

  quotek::data::cvector<quotek::core::position>& plist = mm->getPositions();

  for (int i=0;i<plist.size();i++) {

    ret += "{";

    ret += "\"indice\":\"" + plist.at(i).asset_name + "\",";
    ret += "\"epic\":\"" + plist.at(i).asset_id + "\",";
    ret += "\"size\":\"" + int2string(plist.at(i).size) + "\",";
    ret += "\"open\":\"" + float2string(plist.at(i).open) + "\",";
    ret += "\"stop\":\"" + float2string(plist.at(i).stop) + "\",";
    ret += "\"limit\":\"" + float2string(plist.at(i).limit) + "\",";
    ret += "\"pnl\":\""  + float2string(plist.at(i).pnl)  + "\",";
    ret += "\"dealid\":\"" + plist.at(i).ticket_id + "\"";
    
    ret += "}"; 

    if (i != plist.size() -1) ret += ",";
  } 

  ret += "]";
  return ret;
}


std::string aep_lastlogs(tsEngine* t0,int nb_entries) {

  igmLogger* logger = t0->getLogger();
  std::string ret;

  vector<log_entry> le = logger->getLastEntries(nb_entries);
  
  ret = "[";

  for (int i=0;i<le.size();i++) {

    ret +=  "{ \"tstamp\":\"" + int2string(le[i].tstamp) + "\",";
    ret +=  "\"entry\":\"" + le[i].entry + "\"";
    ret += "}";

    if (i!= le.size() -1) ret += ",";

  }

  ret += "]";

  return ret;  


}


std::string aep_btprogress(tsEngine* t0) {
  std::string ret;
  hsbt* bt0 = (hsbt*) t0;
  ret = "{ \"btprogress\":\"" + int2string( bt0->getBacktestProgress() ) + "\" }";
  return ret;
}

std::string aep_btsnap(hsbt* t0, bool add_logs) {

  std::string ret;
  ret = t0->snapshot(add_logs);
  
  return ret;
  
}


void* aep_handler(void* ptr) {

  aep_handle_io* aio = (aep_handle_io*) ptr;
  networkSession* nsession = aio->nsession;

  tsEngine* t0 = aio->t0 ;
  
  if (!t0) {
    cerr << "*CRITICAL: tsEngine provided to AEP handler is null" << endl;
    return NULL;
  }

  while(nsession->isAlive()) {

    std::string data = nsession->recv_data();
    //std::cout << "RECEIVED_DATA:" << data << endl;
    std::string answer = aep_answer(nsession, data, t0);

    if (answer != "") {
      nsession->send_data(answer + "\r\n\r\n");
    }
    nsession->close_connection();
  }
  
  return NULL;
}
