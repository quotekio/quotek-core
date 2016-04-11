#include "adam.h"

tsEngine* tse;
hsbt* bte;
int so_iter = 0;

void init_signals(struct sigaction* sigact){

  cout << "Initializing signals.." << endl;

  sigact->sa_handler = signal_callback_handler;
  sigemptyset(&(sigact->sa_mask));
  sigact->sa_flags = 0;
  sigaction(SIGINT, sigact, (struct sigaction *)NULL);

  sigaddset(&(sigact->sa_mask), SIGSEGV);
  sigaction(SIGSEGV, sigact, (struct sigaction *)NULL);

  sigaddset(&(sigact->sa_mask), SIGBUS);
  sigaction(SIGBUS, sigact, (struct sigaction *)NULL);

  sigaddset(&(sigact->sa_mask), SIGQUIT);
  sigaction(SIGQUIT, sigact, (struct sigaction *)NULL);

  sigaddset(&(sigact->sa_mask), SIGHUP);
  sigaction(SIGHUP, sigact, (struct sigaction *)NULL);

  sigaddset(&(sigact->sa_mask), SIGKILL);
  sigaction(SIGKILL, sigact, (struct sigaction *)NULL);
}

void signal_callback_handler(int signum) {
  

  if (signum == SIGINT) {
       std::cout << "Gracefully Closing.." << std::endl;
       
       exit(signum);
  }

  

  else if (signum == SIGHUP && tse != NULL) {

    int i;
    string iname;

    so_iter++;

    chdir(ADAM_PREFIX);

    std::vector<strategyHandler*> sh_list = tse->getStratHandlers();

    for (int i=0;i<sh_list.size();i++) {

      int pres = sh_list[i]->prepareCompile();
      int cerr = 0;
      
      if (pres == 0) {
        cout << "recompiling algo " << sh_list[i]->getName() << endl;
        cerr = sh_list[i]->compile(so_iter);
      }
      else {
        std::cout << "[CRITICAL] Compile prepare failed, shutting down robot" << std::endl;
        exit(1);
      }

      cout << "reloading compiled algo " << sh_list[i]->getName() << endl; 
      sh_list[i]->dlibOpen(so_iter);
    }
  }

}

void parse_cmdline(adamCfg* conf,int argc,char** argv) {

   int c;

   static struct option long_options[] =
             {
               
               {"config", required_argument,0,'c'},
               {"backtest", no_argument,0,'b'},
               {"compile", no_argument,0,'d'},
               {"backtest-from",required_argument ,0 ,'f'},
               {"backtest-to", required_argument, 0, 't'},
               {"backtest-result",required_argument,0,'r'},
               {"strategy", required_argument,0, 's'},
               {"spath", required_argument,0,'x'},
               {"genetics", no_argument,0,'g'},
               {"eap-port",required_argument,0,'p'},

             };


  while(1) {
    int option_index = 0;
    c = getopt_long (argc, argv, "c:bdgs:x:t:p:r:",
                      long_options, &option_index);

    if (c == -1) break;

    switch(c) {

      case 'c':
        conf->chconf(optarg);
        cout << "Using config file " << optarg << ".." << endl;
        conf->read();
        break;

      case 'p':
        conf->setAEPPort(atoi(optarg));
        break;

      case 'b':
        conf->setMode(ADAM_MODE_BACKTEST);
        cout << "Starting Adam in Backtest mode.." << endl;
        break;

      case 'd':
        conf->setMode(ADAM_MODE_DRY);
        cout << "Starting Adam in dry run mode, as compilation test.." << endl;
        break;

      case 'g':
        conf->setMode(ADAM_MODE_GENETICS);
        cout << "Starting Adam in Genetics mode.." << endl;
        break;

      case 'x': {
        conf->setStratsPath(std::string(optarg));
      }

      case 's': {
        std::vector<std::string> st = split(std::string(optarg),',');
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


create_t* load_broker(string bname)  {

  cout << "loading broker module..." << endl;
  string lib_broker = "lib" + bname + ".so";
  void* handle = dlopen(lib_broker.c_str(),RTLD_LAZY);

  if(handle == NULL){
    cerr << dlerror() << endl;
    exit(1);
  }

  create_t* create_broker = (create_t*) dlsym(handle, "create");

  const char* dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol create: " << dlsym_error << endl;
        exit(1);
  }

  return create_broker;
}


create_be* load_backend(string bename) {

  cout << "loading backend module..." << endl;
  string lib_backend = "lib" + bename + ".so";
  void* handle = dlopen(lib_backend.c_str(),RTLD_LAZY);

  if(handle == NULL){
    cerr << dlerror() << endl;
    exit(1);
  }

  create_be* create_backend = (create_be*) dlsym(handle, "create");

  const char* dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol create: " << dlsym_error << endl;
        exit(1);
  }

  return create_backend;
}


void aep_loop(std::string laddr, int port) {
  
  network* net = new network();
  net->server(laddr,port);    
 
  networkSession* nsession;

  while (1) {

    nsession = net->server_accept();
    if ( nsession != NULL ) {
      pthread_t cth;
      int presult;
      aep_handle_io aio;
      if (tse != NULL) aio.t0 = tse;
      else aio.t0 = bte;
        
      aio.nsession = nsession;
      if ((presult = pthread_create(&cth,NULL,aep_handler,(void*)&aio)) == 0) {
        pthread_detach(cth);
      }
      else {
        cerr << "* ERROR: Cannot create Additional Thread: error code " << presult << endl;
        nsession->close_connection();
      }
    }
  }
}


int main(int argc,char** argv) {

  extern tsEngine* t;

  cout << "ADAM TRADING BOT " << ADAM_VERSION << endl << "(c) 2013-2016 Clément Gamé" << endl;

  //init signals
  struct sigaction sigact;
  init_signals(&sigact);
  
  //seeds prandom generator
  srand(time(NULL));
  
  chdir(ADAM_PREFIX);

  cout << "Loading configuration..." << endl;

  genetics* ge = NULL;
  strategyHandler* sh;
  adamCfg* c = new adamCfg();

  //checks if command line has config a config file option.
  bool has_cf_option = false;
  for (int i=1; i< argc;i++) {
    if ( std::string(argv[i]) == "-c" || std::string(argv[i]) == "--config" ) {
      has_cf_option = true;
      break;
    }
  }

  if (! has_cf_option ) c->read();
  
  parse_cmdline(c,argc,argv);

  
  AssocArray<indice*> ilist = c->getIndicesList();


  //moneymanager
  mm_params* mmp = c->getMMP();
  moneyManager* mm = new moneyManager(mmp->mm_capital,
                                      mmp->mm_max_openpos, 
                                      mmp->mm_max_openpos_per_epic, 
                                      mmp->mm_reverse_pos_lock, 
                                      mmp->mm_reverse_pos_force_close, 
                                      mmp->mm_max_loss_percentage_per_trade,
                                      mmp->mm_critical_loss_percentage, 
                                      mmp->mm_max_var,
                                      ilist);

  //getting Extra Modules List
  vector<string> mlist = c->getModulesList();
  
  //getting active strats
  std::vector<std::string> astrats = c->getActiveStrats();

  std::vector<strategyHandler*> sh_list;

  if ( c->getMode() == ADAM_MODE_GENETICS  ) {

    genetics_params* gp = c->getGP();

    ge = new genetics(gp->genetics_population,
                                gp->genetics_survivors,
                                gp->genetics_children,
                                gp->genetics_newcomers,
                                gp->genetics_mutable_genes,
                                gp->genetics_converge_thold,
                                gp->genetics_max_generations,
                                gp->genetics_recompute_winners);

    sh_list.emplace_back( new strategyHandler(c->getStratsPath(), astrats[0], ge) );

  }

  else {

    for (int i=0;i<astrats.size();i++) {
      sh_list.emplace_back (new strategyHandler(c->getStratsPath(), astrats[i]) );
    }

  }

  broker* b = load_broker(c->getBroker())();

  backend* back;

  if (c->getBackend() != "none" && c->getBackend() != "" ) {
    back = load_backend(c->getBackend())();
  }

  else  {
    cout << "* WARNING: No backend is configured: cannot save history, cannot perform backtests ! *" << endl;
    back = NULL;
  }

  if (back != NULL) {
    cout << "initializing backend connection.." << endl;
    back->init(c->getBackendParams());
    back->connect();
  }

  for (int i=0;i<sh_list.size();i++) {

    cout << "preparing strategy compilation for algo " << sh_list[i]->getName() << endl;
    int pres = sh_list[i]->prepareCompile();
    int cerr = 0;

    if (pres == 0) {
      cout << "compiling algo " << sh_list[i]->getName() << endl;
      cerr = sh_list[i]->compile(0);
    }
    else {
      std::cout << "[CRITICAL] Compile prepare failed, shutting down robot" << std::endl;
      exit(1);
    }


    if (cerr > 0) {
      std::cout << "[CRITICAL] Strategy "<< sh_list[i]->getName() << " failed to compile !" << std::endl;
      exit(1);
    }

    cout << "loading compiled algo " << sh_list[i]->getName() << endl;
    sh_list[i]->dlibOpen(0);

  }

  adamGeneticsResult* gres;
  adamresult* res;

  switch (c->getMode()) {

    case ADAM_MODE_DRY:
      cout << "Compilation was succesfull, closing adam..";
      exit(0);
      break;

    case ADAM_MODE_REAL:
      cout << "starting Engine in real mode.." << endl;
      tse = new tsEngine(c,b,back,ilist,sh_list,mm,ge,mlist);
      break;
    case ADAM_MODE_BACKTEST:
      cout << "starting Engine in backtest mode.." << endl;
      bte = new hsbt(c,b,back,ilist,sh_list,mm,ge,mlist);
      res = bte->run();

      if ( c->getBTResultFile() != "" ) {
        res->saveToFile(c->getBTResultFile());
      }

      break;

    case ADAM_MODE_GENETICS:
      cout << "starting Engine in genetics mode.." << endl;
      bte = new hsbt(c,b,back,ilist,sh_list,mm,ge,mlist);
      gres = bte->runGenetics();

      if ( c->getBTResultFile() != "" ) {
        gres->saveToFile(c->getBTResultFile());
      }

      break;

    default:
      cerr << "*CRITICAL: Unknown/unsupported mode for ADAM, closing program" << endl;
      exit(1);
      break;
      
  }

  aep_params* aepp = c->getAEPP();

  //if AEP, we start service + attached Websocket !
  if (aepp->enable) {

    std::thread th_aep_s1(aep_loop, aepp->listen_addr, aepp->listen_port );
    aep_ws_server ws1(aepp->listen_port + 1);

    std::thread th_aep_ws1 ( [&ws1] { ws1.run(); } );
    
    //Websocket broadcasting routine
    std::thread th_ws_bcast ( [&ws1, &tse] {

      std::string ws_plist = "";
      std::string ws_prev_plist = "";

      std::string ws_logs = "";
      std::string ws_prev_logs = "";

      std::string ws_cstats = "";
      std::string ws_prev_cstats = "";

      std::string ws_algos = "";
      std::string ws_prev_algos = "";

      while(1) {

        ws_plist = aep_poslist(tse);

        /*Opti à faire içi 
        (resend complet des 100 dernieres lignes en cas de change.) */

        ws_logs = aep_lastlogs(tse,100);

        ws_cstats = aep_corestats(tse);

        ws_algos = aep_algos(tse);

        if ( ws_prev_plist != ws_plist) {
          ws1.broadcast("poslist", ws_plist);
          ws_prev_plist = ws_plist;  
        }
        
        if ( ws_prev_logs != ws_logs  ) {
          ws1.broadcast("lastlogs", ws_logs);
          ws_prev_logs = ws_logs;
        }

        if ( ws_cstats != ws_prev_cstats ) {
          ws1.broadcast("corestats", ws_cstats);
          ws_prev_cstats = ws_cstats;
        }

        if (ws_algos != ws_prev_algos) {
          ws1.broadcast("algos", ws_algos);
          ws_prev_algos = ws_algos;
        }

        usleep(500000);

      }
    });
    
    while(1) {
      sleep(2);
    }

  }

  else {
    while(1) {
      sleep(2);
    }
  }

}

