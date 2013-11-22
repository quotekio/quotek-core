#include "aep.h"


std::string aep_answer(std::string data,tsEngine* t0) {

  //std::cout << "DATA:" << data << std::endl;
  
  std::string res = "";

  if (data == "corestats\r\n") {
    res = aep_corestats(t0);
  }

  else if (data == "poslist\r\n") {
    res = aep_poslist(t0);
  }

  else if (data == "btprogress\r\n") {
    res = aep_btprogress(t0);
  }


  else {

    data = trim(data);
    vector<string> query_args = split(data,' ');

    if (query_args.size() > 0 ) {
      if ( query_args[0] == "lastlogs" ) {
        int ne = atoi( query_args[1].c_str() ) ;
        res = aep_lastlogs(t0,ne);
      } 
    }
  }
  

  return res;
}

std::string aep_corestats(tsEngine* t0) {
  moneyManager* mm = t0->getMoneyManager();
  std::string ret;
  
  ret = "{";
  ret += "\"pnl\":\"" + float2string(mm->getCurResult())  + "\",";
  ret += "\"nbpos\":\"" + int2string( mm->getPositions()->size() ) + "\"";
  ret += "}"; 

  return ret;
}

std::string aep_poslist(tsEngine* t0) {

  moneyManager* mm = t0->getMoneyManager();
  std::string ret = "[";

  vector<position>* plist = mm->getPositions();

  for (int i=0;i<plist->size();i++) {

    ret += "{";

    ret += "\"indice\":\"" + plist->at(i).indice + "\",";
    ret += "\"epic\":\"" + plist->at(i).epic + "\",";
    ret += "\"size\":\"" + int2string(plist->at(i).size) + "\",";
    ret += "\"stop\":\"" + float2string(plist->at(i).stop) + "\",";
    ret += "\"pnl\":\""  + float2string(plist->at(i).pnl)  + "\",";
    ret += "\"dealid\":\"" + plist->at(i).dealid + "\"";
    
    ret += "}"; 

    if (i != plist->size() -1) ret += ",";
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
  ret = "{ \"btprogress\":\"" + int2string( t0->getBacktestProgress() ) + "\" }";
  return ret;
}



void* aep_handler(void* ptr) {

  aep_handle_io* aio = (aep_handle_io*) ptr;
  networkSession* nsession = aio->nsession;
  tsEngine* t0 = aio->t0;
 

  while(nsession->isAlive()) {

    std::string data = nsession->recv_data();
    //std::cout << "RECEIVED_DATA:" << data << endl;
    std::string answer = aep_answer(data,t0);

    if (answer != "") {
      nsession->send_data(answer + "\r\n\r\n");
    }
    nsession->close_connection();
  }
  
  return NULL;
}
