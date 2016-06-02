#include "qate.h"
#include "lib/version.h"

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

    chdir(QATE_PREFIX);

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

void parse_cmdline(qateCfg* conf,int argc,char** argv) {

   int c;

   static struct option long_options[] =
             {
               
               {"config", required_argument,0,'c'},
               {"backtest", no_argument,0,'b'},
               {"compile", no_argument,0,'d'},
               {"backtest-from",required_argument ,0 ,'f'},
               {"backtest-to", required_argument, 0, 't'},
               {"backtest-result",required_argument,0,'r'},
               {"exit", no_argument,0,'e'},
               {"strategy", required_argument,0, 's'},
               {"spath", required_argument,0,'x'},
               {"batch", no_argument,0,'z'},
               {"genetics", no_argument,0,'g'},
               {"eap-port",required_argument,0,'p'},

             };


  while(1) {
    int option_index = 0;
    c = getopt_long (argc, argv, "c:bedgs:x:t:p:r:",
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

      case 'e':
        conf->setBTExit(true);
        break;

      case 'd':
        conf->setMode(QATE_MODE_DRY);
        cout << "Starting Qate in dry run mode, as compilation test.." << endl;
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


create_cc* load_cache(string cachename) {

  cout << "loading cache module..." << endl;
  string lib_cache = "lib" + cachename + ".so";
  void* handle = dlopen(lib_cache.c_str(),RTLD_LAZY);

  if(handle == NULL){
    cerr << dlerror() << endl;
    exit(1);
  }

  create_cc* cc_ = (create_cc*) dlsym(handle, "create");

  const char* dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol create: " << dlsym_error << endl;
        exit(1);
  }

  return cc_;

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


void ws_broadcast_real(tsEngine* tse, aep_ws_server* ws1 ) {

  std::cout << "Starting AEP Websocket broadcasting routine." << std::endl;

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
      ws1->broadcast("poslist", ws_plist);
      ws_prev_plist = ws_plist;  
    }
    
    if ( ws_prev_logs != ws_logs  ) {
      ws1->broadcast("lastlogs", ws_logs);
      ws_prev_logs = ws_logs;
    }

    if ( ws_cstats != ws_prev_cstats ) {
      ws1->broadcast("corestats", ws_cstats);
      ws_prev_cstats = ws_cstats;
    }

    if (ws_algos != ws_prev_algos) {
      ws1->broadcast("algos", ws_algos);
      ws_prev_algos = ws_algos;
    }

    usleep(500000);

  }
}

void ws_broadcast_bt(hsbt* bte,aep_ws_server* ws1) {

  std::cout << "Starting AEP Websocket broadcasting routine. (backtests)" << std::endl;

  while(1) {

    std::string btsnap = aep_btsnap(bte);

    ws1->broadcast("btsnap", btsnap);
    usleep(1000000);

  }



}



int main(int argc,char** argv) {

  extern tsEngine* t;

  cout << "QATE TRADING BOT " << QATE_VERSION << endl << "(c) 2013-2016 Clément Gamé" << endl;

  //init signals
  struct sigaction sigact;
  init_signals(&sigact);
  
  //seeds prandom generator
  srand(time(NULL));
  
  chdir(QATE_PREFIX);

  cout << "Loading configuration..." << endl;

  genetics* ge = NULL;
  strategyHandler* sh;
  qateCfg* c = new qateCfg();

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

  if ( c->getMode() == QATE_MODE_GENETICS  ) {

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

  cache* cc_ = NULL;

  if (c->getRedisHost() != "") {
    cc_ = load_cache("rediscache")();
    cc_->init(c->getRedisHost(), c->getRedisPort());
    cc_->connect();
  }


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

  switch (c->getMode()) {

    case QATE_MODE_DRY:
      cout << "Compilation was succesfull, closing qate..";
      exit(0);
      break;

    case QATE_MODE_REAL:
      cout << "starting Engine in real mode.." << endl;
      tse = new tsEngine(c,b,back,NULL,ilist,sh_list,mm,ge,mlist);
      break;
    case QATE_MODE_BACKTEST:
      cout << "starting Engine in backtest mode.." << endl;
      bte = new hsbt(c,b,back,cc_,ilist,sh_list,mm,ge,mlist);
      break;

    case QATE_MODE_GENETICS:
      cout << "starting Engine in genetics mode.." << endl;
      bte = new hsbt(c,b,back,cc_,ilist,sh_list,mm,ge,mlist);
      break;

    default:
      cerr << "*CRITICAL: Unknown/unsupported mode for QATE, closing program" << endl;
      exit(1);
      break;
      
  }

  std::thread* th_aep_s1;
  std::thread* th_aep_ws1;
  std::thread* bc1;

  aep_ws_server* ws1;

  aep_params* aepp = c->getAEPP();

  //if AEP, we start service + attached Websocket !
  if (aepp->enable) {

    th_aep_s1 = new std::thread (aep_loop, aepp->listen_addr, aepp->listen_port );

    ws1 = new aep_ws_server(aepp->listen_port + 1);

    th_aep_ws1 = new std::thread( [ws1] { ws1->run(); } );
    
    if ( c->getMode() == QATE_MODE_REAL) {
      bc1 = new std::thread(ws_broadcast_real, tse, ws1);
    }

    else if (c->getMode() == QATE_MODE_BACKTEST || c->getMode() == QATE_MODE_GENETICS) {
      bc1 = new std::thread(ws_broadcast_bt, bte, ws1);
    }
  }

  init_finalize(c, ws1);

}

void init_finalize(qateCfg* c, aep_ws_server* ws1) {

  
  std::cout << "Continuating with Init" << std::endl;
  
  //We finish TSE initialization and start the algos.
  if ( c->getMode() == QATE_MODE_REAL ) {
    tse->init_finalize(c);
  }

  //We finish BT initialization and start the run
  else if ( c->getMode() == QATE_MODE_BACKTEST ) {
    bte->init_finalize();
    bte->runNormal();

    //we send last snap to AEP/WS
    if ( ws1 != nullptr ) ws1->broadcast("btsnap", aep_btsnap(bte) );
    if (c->getBTExit()) exit(0);
  }

  else if ( c->getMode() == QATE_MODE_GENETICS  ) {
    bte->init_finalize();
    bte->runGenetics();

    //we send last snap to AEP/WS
    if ( ws1 != nullptr ) ws1->broadcast("btsnap", aep_btsnap(bte) );
    if (c->getBTExit()) exit(0);
  }

  else if ( c->getMode() == QATE_MODE_BATCH  ) {
    bte->init_finalize();
    bte->runBatch();

    //we send last snap to AEP/WS
    if ( ws1 != nullptr ) ws1->broadcast("btsnap", aep_btsnap(bte) );
    if (c->getBTExit()) exit(0);
  }

  


  while(1) {
    sleep(2);
  }

}
