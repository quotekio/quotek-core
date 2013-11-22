#ifndef IGML_H
#define IGML_H

#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "utils.h"


using namespace std;

typedef struct log_entry {
  int tstamp;
  string entry;
} log_entry;


class igmLogger {

  public:
    igmLogger();
    void log(string);
    ofstream* getStream();
    vector<log_entry> getLastEntries(int);
  private:
  ofstream out;
  vector<log_entry> entries;

  

  
};

#endif