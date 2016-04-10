#ifndef AEP_H
#define AEP_H

#include "network.h"
#include "tsengine.h"
#include "hsbt.h"
#include "constants.h"
#include <quotek/cvector.hpp>

#include <string>
#include <iostream>


typedef struct aep_handle_io {
    tsEngine* t0;
    networkSession* nsession;
} aep_handle_io;


typedef struct ws_token {
  int timestamp;
  std::string client_ip;
} ws_token;

void* aep_handler(void*);
std::string aep_answer(networkSession*, std::string,tsEngine*);
std::string aep_corestats(tsEngine*);
std::string aep_order(tsEngine*,std::vector<string>);
std::string aep_poslist(tsEngine*);
std::string aep_lastlogs(tsEngine*,int);
std::string aep_btprogress(tsEngine*);
std::string aep_wstoken(networkSession*, tsEngine*);
std::string aep_btsnap(tsEngine*);
std::string aep_version(tsEngine*);
std::string aep_algos(tsEngine*);

#endif
