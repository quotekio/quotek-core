#ifndef AEP_H
#define AEP_H

#include "network.h"
#include "tsengine.h"
#include "btengine.h"
#include "constants.h"

#include <string>
#include <iostream>


typedef struct aep_handle_io {
    tsEngine* t0;
    networkSession* nsession;
} aep_handle_io;

void* aep_handler(void*);
std::string aep_answer(std::string,tsEngine*);
std::string aep_corestats(tsEngine*);
std::string aep_poslist(tsEngine*);
std::string aep_lastlogs(tsEngine*,int);
std::string aep_btprogress(tsEngine*);
std::string aep_version(tsEngine*);



#endif
