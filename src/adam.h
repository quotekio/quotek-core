#ifndef ADAM_H
#define ADAM_H
#include <stdio.h>
#include <stdlib.h>
#include "lib/tsengine.h"
#include "lib/btengine.h"
#include "lib/adamcfg.h"
#include <string>
#include <dlfcn.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <getopt.h>

#include "lib/broker.hpp"
#include "lib/strategyhandler.h"
#include "lib/moneymanager.h"
#include "lib/genetics.h"
#include "lib/network.h"
#include "lib/aep.h"

#include "lib/constants.h"

#ifndef ADAM_PREFIX
#define ADAM_PREFIX "/usr/local/adam"
#endif


#endif

create_t* load_broker(string);
create_be* load_backend(string);

void panic(const char *fmt, ...);
void init_signals(struct sigaction*);
void signal_callback_handler(int);
