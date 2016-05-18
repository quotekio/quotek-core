#ifndef TSEXPORT_H
#define TSEXPORT_H

#include "igmlogger.h"
#include "store.h"
#include <quotek/cvector.hpp>
#include <quotek/any.hpp>
#include <map>


/** tsexport is a class meant to expose some poarts 
 *  of TSEngine to external modules. */

class tsexport {

public:

  /** tsexport main constructor.
   *  @param mode: Qate running mode.
   *  @param logger: Pointer to Qate logger object.
   *  @param cur_pnl: Pointer to Qate Current PNL float variable.
   *  @param cumulative_pnl: Pointer to Qate Cumulative PNL float variable.
   *  @param positions: Pointer to Qate Positions list.
   *  @param s: Current Qate values store pointer. 
   */

  tsexport(int mode,
  	       igmLogger* logger,
  	       float* cur_pnl,
  	       float* cumulative_pnl,
  	       quotek::data::cvector<quotek::core::position>& positions,
  	       std::map<std::string,quotek::data::any>* s) : positions(positions) {

    this->mode = mode;
    this->logger = logger;
    this->cur_pnl = cur_pnl;
    this->s = s;

  }

public:

  int mode;
  igmLogger* logger;
  float* cur_pnl;
  float* cumulative_pnl;
  quotek::data::cvector<quotek::core::position>& positions;
  std::map<std::string, quotek::data::any>* s;
  void* eptr;

};

#endif
