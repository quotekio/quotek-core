#ifndef TSEXPORT_H
#define TSEXPORT_H

#include "igmlogger.h"
#include "store.h"
#include <quotek/cvector.hpp>


/** tsexport is a class meant to expose some poarts 
 *  of TSEngine to external modules. */

class tsexport {

public:

  /** tsexport main constructor.
   *  @param mode: Adam running mode.
   *  @param logger: Pointer to Adam logger object.
   *  @param cur_pnl: Pointer to Adam Current PNL float variable.
   *  @param cumulative_pnl: Pointer to Adam Cumulative PNL float variable.
   *  @param positions: Pointer to Adam Positions list.
   *  @param s: Current Adam values store pointer. 
   */

  tsexport(int mode,
  	       igmLogger* logger,
  	       float* cur_pnl,
  	       float* cumulative_pnl,
  	       quotek::data::cvector<position>* positions,
  	       store* s) {

    this->mode = mode;
    this->logger = logger;
    this->cur_pnl = cur_pnl;
    this->positions = positions;
    this->s = s;

  }

public:

  int mode;
  igmLogger* logger;
  float* cur_pnl;
  float* cumulative_pnl;
  quotek::data::cvector<position>* positions;
  store* s;
  void* eptr;

};

#endif