#ifndef MM_H
#define MM_H

#include <iostream>
#include <vector>
#include <map>
#include <deque>
#include <cmath>
#include <string>
#include <algorithm>

#include "assoc.h"
#include "utils.h"
#include "indice.h"
#include "results.h"
#include "igmlogger.h"

#include <quotek/cvector.hpp>
#include <quotek/position.hpp>

#define MM_ERR_NBPOS 0x01
#define MM_ERR_TRADERISK_2HIGH 0x02
#define MM_ERR_NOSTOP 0x03
#define MM_ERR_NBPOS_PE 0x04
#define MM_ERR_REVERSE_POS_LOCK 0x05

#define MM_ERR_CRITICAL 0x10

#define MM_ERR_CRITICAL_STR "Critical state reached, No more positions taken until restart."
#define MM_ERR_NBPOS_STR "Max number of positions reached"
#define MM_ERR_NBPOS_PE_STR "Max number of positions reached for epic"
#define MM_ERR_TRADERISK_2HIGH_STR "VAR is too high for the trade"
#define MM_ERR_NOSTOP_STR "No stop provided"
#define MM_ERR_REVERSE_POS_LOCK_STR "An opposite position is already open"

#define CPNL_FILE "/tmp/qate/qate.cpnl"
#define POSCACHE_FILE "/tmp/qate/qate.poscache"


using namespace std;


#define POS_OPEN 0x00
#define POS_PENDING_CLOSE 0x01
#define POS_CLOSED 0x02

typedef struct pos_c {
  const char* indice;
  const char* epic;
  const char* dealid;
  const char* name;
  int size;
  float stop;
  float vstop;
  float vlimit;
  int nb_inc;
  float limit;
  float open;
  float pnl;
  int status;
} pos_c;



class moneyManager {

  public:

  	moneyManager(float capital,
                           int max_openpos, 
                           int max_openpos_per_epic, 
                           int reverse_pos_lock,
                           int reverse_pos_force_close,
                           float max_loss_percentage_per_trade,
                           float critical_loss_percentage,
                           float max_var,
                           AssocArray<indice*> indices_list,
                           igmLogger* logger );

  	float computeVAR();
  	void computePNLs(string,float);
    void computePNLs2(std::map<std::string, float>  current_values);

  	float computeWholePNL();
    float computeWholePNLNoSec();
    float getEndResult();
    int countPos(string indice_name);
    int ask(string,string,int,int);
    void smartAsk(int*,string);
    int addPosition(quotek::core::position p);
    vector<quotek::core::position>::iterator remPosition(vector<quotek::core::position>::iterator);
    void remPosition(string);
    bool hasPos(string);
    bool hasPos(string,string);
    quotek::data::cvector<quotek::core::position>& getPositions();
    quotek::data::cvector<quotek::core::position>* getPositionsPtr();
    quotek::data::cvector<quotek::core::position>* getPositionsHistory();
    quotek::core::position* getPositionByDealid(string);
    string cleanPositions(vector<string>);
    string resolveError(int);
    float* getCurPNL();
    float* getCumulativePNL();
    float getCurResult();
    float getUnrealizedPNL();

    int getReversePosForceClose();
    vector<string> findPos(string,string);


    void displayPositions();
    void getPositionsString(string*);
    void getStats(string*);
    void addStats(qateresult*);
    void displayStats();

    void clear();
    void reset();

    void loadCPNL();
    void saveCPNL();

    quotek::data::cvector<quotek::core::position>* loadPosCache();
    void savePosCache();
    void verifyPosCache(quotek::data::cvector<quotek::core::position>* pos_cache_list );

    /* heartbeat checks if the algos are not making foolish things
       and therefore losing shitloads of money. */
    bool heartbeat();

  private:
  	float capital;
    int max_openpos;
    int max_openpos_per_epic;
    float max_loss_percentage_per_trade;
    float critical_loss_percentage;
    float max_var;
  	quotek::data::cvector<quotek::core::position> positions;
    quotek::data::cvector<quotek::core::position> positions_history;
    AssocArray<indice*> indices_list;
    float var;
    float cur_pnl;
    float cumulative_pnl;
    int reverse_pos_lock;
    int reverse_pos_force_close;

    bool healthy;

    igmLogger* logger;

};

#endif
