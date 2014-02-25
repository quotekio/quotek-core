#ifndef BTENGINE_H
#define BTENGINE_H

#include "tsengine.h"
#include <map>

/*
btEngine class defines the behaviour of adam while in backtest mode.
The threading model is given away for serial processing in a single loop.
btengine inherits from tsEngine.
*/

class btEngine: tsEngine {

  public:
  	btEngine(adamCfg*,
             broker*, 
             AssocArray<indice*>,
             strategy*,
             moneyManager*,
             genetics*,
             vector<string>);

    void evaluate_(string,void*);
    void moneyman_();
    void execute_();
    void run();
    void runGenetics();

    float getSpeed();
    int getBacktestPos();
    int getBacktestProgress();
    void setBacktestPos(int);
    void setBacktestProgress(int);

  protected:

    float backtest_speed;
    int tse_mode;
    string backtest_dump;
    int backtest_pos;
    int backtest_progress;

    std::map<string, void*> eval_pointers; 

};

#endif
