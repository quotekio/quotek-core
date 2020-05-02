

/*

  cache* cc_ = NULL;

  if (c->getRedisHost() != "") {
    cc_ = load_cache("rediscache")();
    cc_->init(c->getRedisHost(), c->getRedisPort());
    cc_->connect();
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


  */