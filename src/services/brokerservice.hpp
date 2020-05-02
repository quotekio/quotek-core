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
    // msg.
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