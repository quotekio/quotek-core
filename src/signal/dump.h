#ifndef DUMP_H
#define DUMP_H

#include "../lib/narrays.h"
#include "../lib/assoc.h"
#include "../lib/utils.h"
#include <iostream>
#include <fstream>
#include <pthread.h>

class dump {

  public:
    dump();
    void load(char*);
    void load_async(char*);
    float getLoadProgress();
    void setLoadProgress(float);
    AssocArray<farray>* getValues();
    iarray* getTStamps();
    char* getDumpFile();

  private:
  	char* dumpfile;
  	int dumpsize;
  	int readsize;
    float load_progress;
    iarray tstamps;
    AssocArray <farray> values; 
};

#endif