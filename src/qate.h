#ifndef QATE_H
#define QATE_H
#include <stdio.h>
#include <stdlib.h>
#include "lib/tsengine.h"
#include "lib/hsbt.h"
#include "lib/qatecfg.h"
#include <string>
#include <dlfcn.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <getopt.h>

#include "lib/broker.hpp"
#include "lib/strategyhandler.hpp"
#include "lib/moneymanager.h"
#include "lib/genetics.h"
#include "lib/network.h"
#include "lib/aep.h"
#include "lib/aep_ws.hpp"

#include "lib/constants.h"

#ifndef QATE_PREFIX
#define QATE_PREFIX "/usr/local/qate"
#endif


#endif


create_t* load_broker(string);
create_be* load_backend(string);

void panic(const char *fmt, ...);
void init_signals(struct sigaction*);
void signal_callback_handler(int);
void init_finalize(qateCfg* c, aep_ws_server* ws1);
