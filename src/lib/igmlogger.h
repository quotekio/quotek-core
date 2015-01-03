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
    igmLogger(bool);
    void log(string);
    void log(string, int);
    ofstream* getStream();
    vector<log_entry> getLastEntries(int);
    vector<log_entry>* getAllEntries();
    void clear();
  private:
  bool use_stdout;
  ofstream out;
  vector<log_entry> entries;

  

  
};

#endif