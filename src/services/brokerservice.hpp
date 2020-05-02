#ifndef BROKERSERVICE_H
#define BROKERSERVICE_H

#include <quotek/record.hpp>
#include <n3rv/n3rvcommon.hpp>
#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvservicecontroller.hpp>
#include "../lib/qatecfg.h"
#include "../lib/broker.hpp"
#include <dlfcn.h>

class brokerservice : public n3rv::service
{
  using n3rv::service::service;

public:
  n3rv::qhandler *prices;
  n3rv::qhandler *orders;
  AssocArray<indice *> ilist;
  ticks_t ticks;

  int initialize(const char *node_name, qateCfg *cfg)
  {

    this->cfg = cfg;

    this->ilist = cfg->getIndicesList();
    this->ticks = cfg->getTicks();

    //Sets a global identifier for service node.
    this->set_uid(("qate.broker." + std::string(node_name)).c_str());

    //instanciates broker object
    this->broker_ = this->load_broker(this->cfg->getBroker())();

    //initializes and connect to broker;
    this->broker_->initialize(this->cfg->getBrokerParams(),
                              true,
                              true,
                              this->cfg->getBrokerMode());

    if (this->broker_->requiresIndicesList() == 1)
    {
      this->broker_->setIndicesList(iGetEpics(this->ilist));
    }
    int conn_status = this->broker_->connect();

    if (conn_status != 0)
    {
      this->ll->log(n3rv::LOGLV_CRIT, "Can't connect to broker, please check your credentials/ API availability");
      exit(1);
    }
    else
    {
      this->ll->log(n3rv::LOGLV_INFO, "Connection to Broker successful");
    }

    this->prices = this->bind("prices", "127.0.0.1", ZMQ_PUB);
    this->orders = this->bind("orders", "127.0.0.1", ZMQ_REP);

    this->attach(this->orders, process_orders);
  }

  void hkloop()
  {
  }

  static void *fetchPrices(void *objref)
  {

    brokerservice *self = (brokerservice *)objref;
    vector<bvex> values;

    uint32_t time_ms;
    string epic;

    float buy;
    float sell;
    float spread;

    while (1)
    {
      //perf profiling
      auto tthis = std::chrono::high_resolution_clock::now();
      values = self->broker_->getValues();
      time_ms = time(0);

      if (values.size() != 0)
      {

        for (int i = 0; i < values.size(); i++)
        {

          epic = values[i].epic;

          indice *idx = iResolve(self->ilist, epic);

          float unit_coef = 1.0 / idx->pip_value;

          quotek::data::record r;
          buy = values[i].offer;
          sell = values[i].bid;

          r.timestamp = time_ms;
          r.value = (buy + sell) / 2;
          r.spread = roundfloat((buy - sell) * unit_coef, 0.1);

          if (idx != NULL)
          {

            n3rv::message msg;
            std::stringstream ss;
            msg.action = "PRICE";
            msg.args.emplace_back(idx->name);
            msg.args.emplace_back(epic);

            ss << r.timestamp << " " << r.value << " " << r.spread;
            msg.payload = ss.str();
            self->send(self->prices, msg, 0);
          }
        }
      }

      auto tt1 = std::chrono::high_resolution_clock::now();
      auto elapsed_t = tt1 - tthis;
      uint64_t elapsed = std::chrono::duration_cast<std::chrono::microseconds>(elapsed_t).count();

      if (elapsed < self->ticks.getval)
      {
        usleep(self->ticks.getval - elapsed);
      }
    }
  }

  static void *process_orders(void *objref, zmq::message_t *zmsg)
  {

    brokerservice *self = (brokerservice *)objref;
    n3rv::message msg = n3rv::parse_msg(zmsg);

    if (msg.action == "OPEN")
    {
       
       std::string id = msg.args[0];
       std::string epic = msg.args[1];
       std::string way = msg.args[2];
       int nbc = atoi(msg.args[3].c_str());
       int stop = atoi(msg.args[4].c_str());
       int limit = atoi(msg.args[4].c_str());

       self->openPosition(id,epic,way,nbc,stop,limit);

    }

    else if (msg.action == "CLOSE")
    {
      std::string dealid = msg.args[0];
      int size = atoi(msg.args[1].c_str());
      self->closePosition(dealid,size);
    }
  }

  void closePosition(string dealid, int size) {

    //API Performance profiling.
    auto tt0 = std::chrono::high_resolution_clock::now();

    string result = this->broker_->closePos(dealid, size);

    if (result == "ACCEPTED:SUCCESS")
    {

      auto tt1 = std::chrono::high_resolution_clock::now();
      auto elapsed_t = tt1 - tt0;
      uint64_t elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_t).count();
      this->ll->log(n3rv::LOGLV_INFO, "Position Closed: " + dealid + " (" + int2string(elapsed) + "ms)");
    }

    else
    {
      this->ll->log(n3rv::LOGLV_INFO, "[broker] Error closing position:" + result);
    }
  }

    quotek::core::position openPosition(string id, string epic, string way, int nbc, int stop, int limit)
  {

    //API Performance profiling.
    auto tt0 = std::chrono::high_resolution_clock::now();

    bpex ex1;
    indice *idx = iResolve(this->ilist, epic);
    ex1 = this->broker_->openPos(epic, way, nbc, stop, limit);

    if (ex1.status == "ACCEPTED")
    {

      quotek::core::position p;
      p.identifier = id;
      p.asset_id = epic;
      p.asset_name = idx->name;
      p.ticket_id = ex1.dealid;
      p.open = ex1.open;
      p.stop = ex1.stop;
      p.set_vstop(ex1.stop);
      p.set_vlimit(ex1.limit);

      p.limit = ex1.limit;
      p.size = ex1.size;
      p.pnl = 0;
      //p.status = POS_OPEN;
      p.open_date = time(0);

      auto tt1 = std::chrono::high_resolution_clock::now();
      auto elapsed_t = tt1 - tt0;
      uint64_t elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_t).count();

      this->ll->log(n3rv::LOGLV_INFO, "New Position Opened: " + ex1.dealid + " (" + int2string(elapsed) + "ms)");
    }

    else
    {
      this->ll->log(n3rv::LOGLV_ERR, "[broker] Error opening position:" + ex1.reason);
    }
  }

protected:
  qateCfg *cfg;
  broker *broker_;

  create_t *load_broker(string bname)
  {

    this->ll->log(n3rv::LOGLV_DEBUG, "loading broker module...");
    string lib_broker = "lib" + bname + ".so";
    void *handle = dlopen(lib_broker.c_str(), RTLD_LAZY);

    if (handle == NULL)
    {
      this->ll->log(n3rv::LOGLV_CRIT, dlerror());
      exit(1);
    }

    create_t *create_broker = (create_t *)dlsym(handle, "create");

    const char *dlsym_error = dlerror();
    if (dlsym_error)
    {
      std::stringstream ss;
      ss << "Cannot load symbol create: " << dlsym_error;
      this->ll->log(n3rv::LOGLV_CRIT, ss.str());
      exit(1);
    }
    return create_broker;
  }
};

#endif