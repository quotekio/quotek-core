#ifndef TSEMODULE_H
#define TSEMODULE_H

#include "tsexport.hpp"

/** tsemodule is a virtual class meant to provide a working template
 *  for Qate Modules.
 */

class tsemodule {

public:

  /** tsemodule main constructor.
   *  @param tse: Qate Exported TSEngine.
   */
  tsemodule(tsexport* tse)  {
    this->tse = tse;
  }
  
  /** Module Initializer */
  virtual int init();

  /** Module execution method, this is where you put your module execution code. */
  virtual int run();

protected:

  tsexport* tse;

};

#endif
