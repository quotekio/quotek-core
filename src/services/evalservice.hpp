#ifndef EVALSERVICE_H
#define EVALSERVICE_H

#include <regex>
#include <n3rv/n3rvcommon.hpp>
#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvservicecontroller.hpp>
#include "../lib/qatecfg.h"
#include "../lib/strategyhandler.hpp"
#include <quotek/backend.hpp>
#include <quotek/strategy.hpp>
#include <quotek/cqueue.hpp>

typedef struct algov2
{
  std::thread *th;
  void *eval_ptr;
  string eval_name;
  string strategy;
  float pnl;
} algov2;

class evalservice : public n3rv::service
{
  using n3rv::service::service;

public:

  n3rv::qhandler *prices;
  n3rv::qhandler *moneyman;
  std::vector<strategy> strategies_pool;

  int initialize(const char *node_name, qateCfg *cfg)
  {

    this->cfg = cfg;
    //Sets a global identifier for service node.
    this->set_uid(("qate.eval." + std::string(node_name)).c_str());

    this->prices = this->connect("qate.broker.*.prices", ZMQ_SUB);
    this->attach(this->prices, pricesReceiveCallback);
  }

  void initStrats()
  {

    this->active_strats = this->cfg->getActiveStrats();
    for (int i = 0; i < this->active_strats.size(); i++)
    {
      this->strategy_handlers.emplace_back(new strategyHandler(this->cfg->getStratsPath(),
                                                               this->active_strats[i],
                                                               false));
    }
  }

  bool initAlgos()
  {

    std::stringstream ss;
    std::vector<std::string> evnames = iGetNames(this->cfg->getIndicesList());

    for (int j = 0; j < this->strategy_handlers.size(); j++)
    {

      
      void *strat_ptr = this->strategy_handlers[j]->getExportFct();
      std::regex asset_match(this->strategy_handlers[j]->getAssetMatch());

      for (int i = 0; i < evnames.size(); i++)
      {

        if (strat_ptr && std::regex_match(evnames.at(i), asset_match))
        {

          algov2 al;
          ss << "loading eval for indice " << evnames.at(i);
          this->ll->log(n3rv::LOGLV_INFO,ss);
          
          al.eval_ptr = strat_ptr;
          al.eval_name = evnames.at(i);
          al.strategy = this->strategy_handlers[j]->getName();
          al.pnl = 0;
          algos.emplace_back(al);
        }
      }
    }

    for (int i = 0; i < algos.size(); i++)
    {

      //function pointer to extern C create_st symbol.
      create_st *c_strat = (create_st *)algos[i].eval_ptr;
      strategy *st = c_strat();

      //st->recs = &this->getAssetRecords(algos[i].eval_name);

      //To Add later on.
      //st->portfolio = this->getMoneyManager()->getPositionsPtr();
      //st->back = tse_back;

      st->asset_name = algos[i].eval_name;
      st->identifier = algos[i].strategy + "@" + algos[i].eval_name;
      
      //st->store = &tse_store;

      //algos[i].th = new std::thread([st, this] { this->evaluate(st); });
      //algos[i].th->detach();
    }
  }

  bool compileStrats()
  {

    std::stringstream ss;
    for (int i = 0; i < strategy_handlers.size(); i++)
    {

      ss << "preparing strategy compilation for algo " << strategy_handlers[i]->getName();
      this->ll->log(n3rv::LOGLV_INFO, ss);

      int pres = strategy_handlers[i]->prepareCompile();
      int cerr = 0;

      if (pres == 0)
      {
        ss << "compiling algo " << strategy_handlers[i]->getName();
        this->ll->log(n3rv::LOGLV_INFO, ss);
        cerr = strategy_handlers[i]->compile(0);
      }
      else
      {
        ss << "Compile prepare failed, shutting down eval";
        this->ll->log(n3rv::LOGLV_CRIT, ss);
        return false;
      }

      if (cerr > 0)
      {
        ss << "Strategy " << strategy_handlers[i]->getName() << " failed to compile !";
        this->ll->log(n3rv::LOGLV_CRIT, ss);
        return false;
      }

      ss << "loading compiled algo " << strategy_handlers[i]->getName();
      this->ll->log(n3rv::LOGLV_INFO, ss);

      strategy_handlers[i]->dlibOpen(0);
    }

    return true;
  }

  void evaluate(strategy *s)
  {
 
    std::stringstream ss;

    //declares work variables
    std::string order;
    std::string logstr;
    std::string savestr;

    //fetch tsEngine objects to avoid too much function calls
    ticks_t ticks = this->cfg->getTicks();

    //starts strategy initialization
    s->initialize();

    //flushing of data to save to another, more general queue.
    while (s->save_queue.pop(savestr, false))
    {
      save_queue.push(savestr);
    }

    while (s->recs->size() == 0)
    {
      ss << "Waiting for data population..";
      this->ll->log(n3rv::LOGLV_INFO, ss);
      usleep(ticks.eval);
    }

    while (1)
    {
      //perf profiling
      auto tt0 = std::chrono::high_resolution_clock::now();

      //setting of evaluation context.
      quotek::data::record &last_rec = s->recs->last();
      s->value = last_rec.value;
      s->spread = last_rec.spread;
      s->t = last_rec.timestamp;

      //user algo tick evaluation.
      s->__evaluate__();

      while (s->orders_queue.pop(order, false))
      {
        this->orders_queue.push(order);
      }

      //flushing of logs
      s->flushlogs();

      while (s->log_queue.pop(logstr, false))
      {
        this->ll->log(n3rv::LOGLV_INFO,logstr);
      }

      //flushing of data to save to another, more general queue.
      while (s->save_queue.pop(savestr, false))
      {
        save_queue.push(savestr);
      }

      auto tt1 = std::chrono::high_resolution_clock::now();
      auto elapsed_t = tt1 - tt0;
      uint64_t elapsed = std::chrono::duration_cast<std::chrono::microseconds>(elapsed_t).count();

      if (elapsed < ticks.eval)
      {
        usleep(ticks.eval - elapsed);
      }
    }
  }

  /** Receives prices coming from broker and stores'em in database. */
  static void *pricesReceiveCallback(void *objref, zmq::message_t *zmsg)
  {
    evalservice *self = (evalservice *)objref;
    n3rv::message msg = n3rv::parse_msg(zmsg);
    quotek::data::record r;
    std::vector<std::string> splitp = split(msg.payload, ' ');
    self->ll->log(n3rv::LOGLV_INFO, "received price payload:" + msg.payload);

    std::string asset_name = msg.args[0];
    r.timestamp = atoi(splitp[0].c_str());
    r.value = atof(splitp[1].c_str());
    r.spread = atof(splitp[2].c_str());
    
    /*for (auto strat: self->strategies_pool) {

       if (strat.asset_name == asset_name) {

       }
    }*/

    //Launch eval for concerned strats

  }

protected:
  qateCfg *cfg;
  std::vector<std::string> active_strats;
  std::vector<strategyHandler *> strategy_handlers;
  std::vector<algov2> algos;

  quotek::data::cqueue<std::string> orders_queue;
  quotek::data::cqueue<std::string> save_queue;
  quotek::data::cqueue<std::string> log_queue;


};

#endif
