#ifndef BTENGINE_H
#define BTENGINE_H

#include "tsengine.cpp"
/*
btEngine class defines the behaviour of adam while in backtest mode.
The threading model is given away for serial processing in a single loop.
btengine inherits from tsEngine.

*/

class btEngine: public tsEngine {

  public:
  	btEngine(adamCfg*,
             broker*, 
             AssocArray<indice*>,
             strategy*,
             moneyManager*,
             genetics*,
             vector<string>);

    float getSpeed();
    int getBacktestPos();
    int getBacktestProgress();
    void setBacktestPos(int);
    void setBacktestProgress(int);

  private:

    float backtest_speed;
    int tse_mode;
    string backtest_dump;
    int backtest_pos;
    int backtest_progress;

};

#endif
