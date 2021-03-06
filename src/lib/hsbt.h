#ifndef BTENGINE_H
#define BTENGINE_H

#include "tsengine.h"
#include "btf.hpp"
#include <map>
#include <quotek/timeutils.hpp>
#include <quotek/any.hpp>

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
  float sharpe_ratio;
  float pnl;

} tradestats;

std::string tradestats2json(tradestats& s);


/*
hsbt, for High Speed BackTester, is the Qate's Backtesting engine.
It inherits from tsEngine but simplifies it for more speed and efficiency.
*/

class hsbt: public tsEngine {

  public:
  	hsbt(qateCfg*,
             broker*,
             backend*,
             cache*,
             AssocArray<indice*>,
             std::vector<strategyHandler*> sh_list,
             moneyManager*,
             genetics*,
             vector<string>,
             igmLogger* logger);

    void init_finalize();

    inline int evaluate_(strategy* s);
    inline void moneyman_();
    inline void execute_();
    int loadBacktestData_();
    
    void reset();
    qateresult* run();

    void runNormal();
    void runGenetics();
    void runBatch();

    void createBatchUniverse(AssocArray<std::vector<quotek::data::any> > &universe );
    std::string batch2string(std::string bname, quotek::data::any value);

    float getSpeed();
    int getBacktestPos();
    int getBacktestProgress();
    void setBacktestPos(int);
    void setBacktestProgress(int);

    void addTradeStats(qateresult*);
    void addLogStats(qateresult*);

    /* Creates a snapshot of the trade statistics so far */
    tradestats compute_tradestats();

    std::string snapshot(bool add_logs);

    /* Computes the sharpe ratio of the BT */
    float sharpe_ratio();

  protected:

    int tse_mode;
    int backtest_pos;
    int backtest_progress;
    float backtest_global_progress;
    int current_iter;
    int progress_tstamp;
    int backtest_from;
    int backtest_to; 
    vector<quotek::core::position> positions_history;

    AssocArray<quotek::data::records> backtest_inmem_records; 
    std::vector<strategy*> strategies;
    std::vector<std::string> si;
    int si_size;

    qateResultsHandler* qrh;

    bool btfilter;
    uint8_t btfilter_from;
    uint8_t btfilter_to;
    uint8_t btfilter_skip;


};

#endif
