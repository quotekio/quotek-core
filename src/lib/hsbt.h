#ifndef BTENGINE_H
#define BTENGINE_H

#include "tsengine.h"
#include <map>

#define REMPOS_STOP 0x01
#define REMPOS_VSTOP 0x02
#define REMPOS_LIMIT 0x03
#define REMPOS_VLIMIT 0x04

typedef struct tradestats {
  int nb_long;
  int nb_short;
  int losing;
  int winning;
  float max_drawdown;
  float profit_factor;

} tradestats;

std::string tradestats2json(tradestats& s);


/*
hsbt, for High Speed BackTester, is the Adam's Backtesting engine.
It inherits from tsEngine but simplifies it for more speed and efficiency.
*/

class hsbt: public tsEngine {

  public:
  	hsbt(adamCfg*,
             broker*,
             backend*,
             AssocArray<indice*>,
             std::vector<strategyHandler*> sh_list,
             moneyManager*,
             genetics*,
             vector<string>);

    void init_finalize();

    inline int evaluate_(strategy* s);
    inline void moneyman_();
    inline void execute_();
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

    /* Creates a snapshot of the trade statistics so far */
    tradestats compute_tradestats();

    std::string snapshot();

    
  protected:

    int tse_mode;
    int backtest_pos;
    int backtest_progress;
    int progress_tstamp;
    int backtest_from;
    int backtest_to; 
    vector<quotek::core::position> positions_history;

    AssocArray<quotek::data::records> backtest_inmem_records; 
    std::vector<strategy*> strategies;
    std::vector<std::string> si;
    int si_size;

};

#endif
