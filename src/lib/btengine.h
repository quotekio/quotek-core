#ifndef BTENGINE_H
#define BTENGINE_H

#include "tsengine.h"
#include <map>

#define REMPOS_STOP 0x01
#define REMPOS_VSTOP 0x02
#define REMPOS_LIMIT 0x03
#define REMPOS_VLIMIT 0x04

/*
btEngine class defines the behaviour of adam while in backtest mode.
The threading model is given away for serial processing in a single loop.
btengine inherits from tsEngine.
*/

class btEngine: public tsEngine {

  public:
  	btEngine(adamCfg*,
             broker*,
             backend*,
             AssocArray<indice*>,
             strategyHandler*,
             moneyManager*,
             genetics*,
             vector<string>);

    int evaluate_(string,void*, int cstate);
    void moneyman_();
    void execute_();
    int loadBacktestData_();
    adamresult* run();
    adamGeneticsResult* runGenetics();

    float getSpeed();
    int getBacktestPos();
    int getBacktestProgress();
    void setBacktestPos(int);
    void setBacktestProgress(int);
    void addAStats(adamresult*);
    void addLogStats(adamresult*);
    
  protected:

    int tse_mode;
    int backtest_pos;
    int backtest_progress;
    int progress_tstamp;
    int backtest_from;
    int backtest_to; 
    vector<quotek::core::position> positions_history;

    AssocArray<quotek::data::records> backtest_inmem_records;

    AssocArray<void*> eval_pointers; 

};

#endif
