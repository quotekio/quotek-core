#include "dump.h"

void* load_th(void* arg) {

  dump* d1 = (dump*) arg;
  char* dumpfile = d1->getDumpFile();
  iarray* tstamps = d1->getTStamps();
  AssocArray<farray>* values = d1->getValues();

  string line;
  ifstream fh0( dumpfile, ios::binary | ios::ate);
  int dumpsize = fh0.tellg();
  fh0.close();

  int readsize= 0;

  ifstream fh (dumpfile);

  if (fh.is_open() ) {
    
    while(fh.good()) {
      getline(fh,line);
      readsize+=line.length() +1;
      vector<string> largs = split(line,':');
      if (largs.size() == 2) {
       
        if (largs[0] == "T") {
          iarray_push(tstamps,atoi( largs[1].c_str() ) );
        }

        else {
          if ( ! values->IsItem(largs[0]) ) {
            farray_init(&(values->get(largs[0])) , 10000 );
            farray_push(&(values->get(largs[0])), atof( largs[1].c_str() ) );
          }

          else {
            farray_push(&(values->get(largs[0])), atof( largs[1].c_str() ) ); 
          }
        }
      }
      d1->setLoadProgress( (float) readsize / (float) dumpsize ); 
    }
  }
  
  return NULL;

}


dump::dump() {
  iarray_init(&tstamps,10000);
  load_progress = 0.1;
}

float dump::getLoadProgress() {
  return load_progress;
}

void dump::setLoadProgress(float lp) {
  load_progress = lp;
}


char* dump::getDumpFile() {
  return dumpfile;
}

AssocArray<farray>* dump::getValues() {
  return &values;
}

iarray* dump::getTStamps() {
  return &tstamps;
}


void dump::load_async(char* df) {
  dumpfile = df;
  pthread_t lasync;
  pthread_create(&lasync,NULL,load_th,this);
}

void dump::load(char* dumpfile) {

  string line;

  ifstream fh0( dumpfile, ios::binary | ios::ate);
  dumpsize = fh0.tellg();
  fh0.close();

  readsize= 0;

  ifstream fh (dumpfile);

  if (fh.is_open() ) {
    
    while(fh.good()) {
      getline(fh,line);
      readsize+=line.length() +1;
      vector<string> largs = split(line,':');
      if (largs.size() == 2) {
       
        if (largs[0] == "T") {
          iarray_push(&tstamps,atoi( largs[1].c_str() ) );
        }

        else {
          if ( ! values.IsItem(largs[0]) ) {
            farray_init( &(values[largs[0]]), 10000 );
            farray_push(&(values[largs[0]]), atof( largs[1].c_str() ) );
          }

          else {
            farray_push(&(values[largs[0]]), atof( largs[1].c_str() ) ); 
          }
        }
      }
      load_progress = (float) readsize / (float) dumpsize; 
    }
  }
}
