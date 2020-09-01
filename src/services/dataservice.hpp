#ifndef DATASERVICE_H
#define DATASERVICE_H

#include <string>
#include <cstdio>
#include <cstdlib>

#include <n3rv/n3rvcommon.hpp>
#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvservicecontroller.hpp>
#include <quotek/backend.hpp>
#include "../lib/qatecfg.h"
#include "../lib/utils.h"
#include <dlfcn.h>

class dataservice : public n3rv::service
{
  using n3rv::service::service;

public:
  n3rv::qhandler *prices;
  n3rv::qhandler *data;

  int initialize(const char *node_name, qateCfg *cfg)
  {

    this->cfg = cfg;

    this->back = (backend *)this->load_backend(this->cfg->getBackend());
    if (this->cfg->getBackend() != "none" && this->cfg->getBackend() != "")
    {
      this->back = load_backend(this->cfg->getBackend())();
    }

    else
    {
      this->ll->log(n3rv::LOGLV_INFO, "No backend is configured: cannot save history, nor run backtests. ");
      return -1;
    }

    if (back != NULL)
    {
      this->ll->log(n3rv::LOGLV_INFO, "Initializing Data Service Connection..");
      back->init(this->cfg->getBackendParams());
      back->connect();
    }

    //Sets a global identifier for service node.
    this->set_uid(("qate.data." + std::string(node_name)).c_str());

    //instanciates broker object
    this->prices = this->connect("qate.broker.*.prices", ZMQ_SUB);
    this->data = this->bind("data", "127.0.0.1", ZMQ_REP);
    //this->attach(this->orders, process_orders);
    this->attach(this->prices, prices_receive);
  }

  /** Distributes data to requester service */
  static void *dist_data(void *objref, zmq::message_t *zmsg)
  {
    dataservice *self = (dataservice *)objref;
    n3rv::message msg = n3rv::parse_msg(zmsg);
  }

  /** Receives prices coming from broker and stores'em in database. */
  static void *prices_receive(void *objref, zmq::message_t *zmsg)
  {
    dataservice *self = (dataservice *)objref;
    n3rv::message msg = n3rv::parse_msg(zmsg);

    quotek::data::record r;
    std::vector<std::string> splitp = split(msg.payload, ' ');

    self->ll->log(n3rv::LOGLV_DEBUG, "received price payload:" + msg.payload);

    r.timestamp = atoi(splitp[0].c_str());
    r.value = atof(splitp[1].c_str());
    r.spread = atof(splitp[2].c_str());
    self->back->store(msg.args[0], r);

  }

protected:
  backend *back;
  qateCfg *cfg;

  create_be *load_backend(std::string bename)
  {

    std::cout << "loading backend module..." << std::endl;
    std::string lib_backend = "lib" + bename + ".so";
    void *handle = dlopen(lib_backend.c_str(), RTLD_LAZY);

    if (handle == NULL)
    {
      std::cerr << dlerror() << std::endl;
      exit(1);
    }

    create_be *create_backend = (create_be *)dlsym(handle, "create");
    const char *dlsym_error = dlerror();
    if (dlsym_error)
    {
      std::cerr << "Cannot load symbol create: " << dlsym_error << std::endl;
      exit(1);
    }
    return create_backend;
  }
};

#endif
