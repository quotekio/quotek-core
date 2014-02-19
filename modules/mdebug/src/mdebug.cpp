#include "../../../src/lib/adammodule.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../../src/lib/utils.h"
#include "../../../src/lib/constants.h"


void log_mem_addrs(module_io* mio,ofstream& out) {
  
  string cdt = currentDateTime();

  for (int i=0;i< mio->values.Size();i++ ) {
    farray* f = *(mio->values[i]);
    string idx = mio->values.GetItemName(i);
    out << "[" <<  cdt  << "]" << "Address for " << idx << "values array:" << static_cast<void*>(f) << endl;
    out << "[" <<  cdt << "]" << "Address for " << idx << "values->values array:" << static_cast<void*>(f->values) << endl;
  }
}

void log_last_values(module_io* mio,ofstream& out) {

  string cdt = currentDateTime();

  iarray* tstamps = *(mio->tstamps);
  if (tstamps == NULL) {
      cout << "*ERROR: Invalid tstamps pointer ! *" << endl;
      return ;
  }


  for (int i=0;i<mio->values.Size();i++) {

     farray* f = *(mio->values[i]);
     string idx = mio->values.GetItemName(i);

     out << "[" <<  cdt  << "]" << "Last values for " << idx << ":";

     if (f->size > 10 ) {
       out << "{";
       int start = f->size - 10;
       int stop = f->size;

       for (int j=start;j<stop;j++) {
          float v = f->values[j];
          out << v << ",";
       }
       out << "}" << endl;
     }

     else {
        out << "Values array size for " << idx << " is < 10" << endl;
     }

  }

}

void* module_fct(module_io mio) {

  ofstream dbgfh;
  dbgfh.open("/var/log/adam.mdebug.log",ios::out | ios::app);

  int nb_iter = 0;

  while(1) {
    nb_iter++;
    log_mem_addrs(&mio,dbgfh);
    log_last_values(&mio,dbgfh);

    dbgfh << "[" << currentDateTime() << "] Adam Uptime:" << ( nb_iter *10 ) << "seconds" << endl; 

    sleep(10);

  }

}


extern "C" void* module(module_io mio) {
  module_fct(mio);
  return NULL;

}
