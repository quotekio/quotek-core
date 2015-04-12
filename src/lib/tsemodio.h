/** tsemodio is a class meant to 
 *  enable communication between TSEngine and its external modules.
 */

class tsemodio {

  /** tsemodio main constructor.
   *  @param mode: Adam running mode.
   *  @param logger: Pointer to Adam logger object.
   *  @param cur_pnl: Pointer to Adam Current PNL float variable.
   *  @param cumulative_pnl: Pointer to Adam Cumulative PNL float variable.
   *  @param positions: Pointer to Adam Positions list.
   *  @param s: Current Adam values store pointer. 
   */
   
  tsemodio(int mode,
  	       igmLogger* logger,
  	       float* cur_pnl,
  	       float* cumulative_pnl,
  	       vector<position>* positions,
  	       store* s);

  /** tsemodio alternate constructor. */
  tsemodio(void* eptr);

  


};