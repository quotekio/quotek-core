#ifndef BTENGINE_H
#define BTENGINE_H

#include "tsengine.h"
#include <map>

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
             strategy*,
             moneyManager*,
             genetics*,
             vector<string>);

    void evaluate_(string,void*);
    void moneyman_();
    void execute_();
    int loadHistory_();
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

    float backtest_speed;
    int tse_mode;
    int backtest_pos;
    int backtest_progress;
    int backtest_from;
    int backtest_to; 
    vector<position> positions_history;

    AssocArray<void*> eval_pointers; 

};

#endif
