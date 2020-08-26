#include "qate.h"
#include "lib/version.h"
#include "lib/broker.hpp"
#include <n3rv/n3rvlogger.hpp>
#include <n3rv/n3rvservicecontroller.hpp>
#include "services/brokerservice.hpp"
#include "services/backendservice.hpp"
#include "services/evalservice.hpp"

int so_iter = 0;

void parse_cmdline(qateCfg *conf, int argc, char **argv)
{

  int c;
  std::stringstream ss;

  static struct option long_options[] =
      {

          {"config", required_argument, 0, 'c'},
          {"backtest", no_argument, 0, 'b'},
          {"compile", no_argument, 0, 'd'},
          {"backtest-from", required_argument, 0, 'f'},
          {"backtest-to", required_argument, 0, 't'},
          {"backtest-result", required_argument, 0, 'r'},
          {"exit", no_argument, 0, 'e'},
          {"strategy", required_argument, 0, 's'},
          {"spath", required_argument, 0, 'x'},
          {"batch", no_argument, 0, 'z'},
          {"genetics", no_argument, 0, 'g'},
          {"eap-port", required_argument, 0, 'p'},

      };

  while (1)
  {
    int option_index = 0;
    c = getopt_long(argc, argv, "c:bedgs:x:t:p:r:",
                    long_options, &option_index);

    if (c == -1)
      break;

    switch (c)
    {

    case 'c':
      conf->chconf(optarg);
      ss.str("");ss.clear();
      ss << "Using config file " << optarg << ".." << endl;
      conf->ll->log(n3rv::LOGLV_INFO, ss.str());
      conf->read();
      break;

    case 'p':
      conf->setAEPPort(atoi(optarg));
      break;

    /* ** DEPRECATED ** /
    case 'b':
      conf->setMode(QATE_MODE_BACKTEST);
      cout << "Starting qate in Backtest mode.." << endl;
      break;
    
    case 'g':
      conf->setMode(QATE_MODE_GENETICS);
      cout << "Starting qate in Genetics mode.." << endl;
      break;

    case 'z':
      conf->setMode(QATE_MODE_BATCH);
      cout << "Starting qate in Batch mode.." << endl;
      break;
    */

    case 'e':
      conf->setBTExit(true);
      break;

    case 'd':
      conf->setMode(QATE_MODE_DRY);
      cout << "Starting Qate in dry run mode, as compilation test.." << endl;
      break;

    case 'x':
    {
      conf->setStratsPath(std::string(optarg));
    }

    case 's':
    {
      std::vector<std::string> st = split(std::string(optarg), ',');
      conf->setActiveStrats(st);
      break;
    }
    case 'f':
      conf->setBFrom(atoi(optarg));
      break;
    case 't':
      conf->setBTo(atoi(optarg));
      break;
    case 'r':
      conf->setBTResultFile(std::string(optarg));
      break;
    }
  }
}

int main(int argc, char **argv)
{

  std::cout << "QATE TRADING BOT " << QATE_VERSION << endl
            << "(c) 2013-2020 Clément Gamé" << endl;

  n3rv::logger *ll = new n3rv::logger(n3rv::LOGLV_INFO);
  ll->add_dest("stdout");
  ll->add_dest("file:///var/log/qatem5.log");

  //seeds prandom generator
  srand(time(NULL));
  chdir(QATE_PREFIX);

  ll->log(n3rv::LOGLV_INFO, "Loading configuration...");

  genetics *ge = NULL;
  genetics_params *gp = NULL;
  strategyHandler *sh;
  qateCfg *c = new qateCfg();
  c->ll = ll;

  //checks if command line has config a config file option.
  bool has_cf_option = false;
  for (int i = 1; i < argc; i++)
  {
    if (std::string(argv[i]) == "-c" || std::string(argv[i]) == "--config")
    {
      has_cf_option = true;
      break;
    }
  }

  if (!has_cf_option)
    c->read();

  parse_cmdline(c, argc, argv);

  //we start an hidden svc controller.
  n3rv::start_controller("0.0.0.0", 10001, ll);

  //**** START N3RV NODES HERE ****/
  brokerservice *bs1 = new brokerservice("127.0.0.1", 10001, ll);
  bs1->initialize("broker1", c);

  //starts prices fetching loop
  new std::thread([bs1] { brokerservice::fetchPrices((void *)bs1); });
  bs1->run_async();

  backendservice *back1 = new backendservice("127.0.0.1",10001,ll);
  back1->initialize("back1",c);
  back1->run_async();

  evalservice *ev1 = new evalservice("127.0.0.1",10001,ll);
  ev1->initialize("eval1",c);
  ev1->initStrats();
  ev1->compileStrats();

  ev1->initAlgos();
  ev1->run_async();



  while (1)
  {
    sleep(2);
  }
}
