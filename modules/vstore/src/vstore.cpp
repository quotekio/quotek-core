#include "../../../src/lib/adammodule.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "odbc.h"

#include "../../../src/lib/utils.h"
#include "../../../src/lib/constants.h"

void* module_fct(module_io mio) {

  if (mio.mode == ADAM_MODE_BACKTEST || mio.mode == ADAM_MODE_GENETICS) {
    cout << "Adam is not in real mode, closing Vstore module..." << endl;
    return NULL;
  }

  SQLRETURN ret;

  cout << "Connecting to Vstore..." << endl;
  odbc* dbconn = new odbc();
  dbconn->connect("DSN=vstore_sqlite;");

  if (dbconn->isConnected() ) {
    cout << "Adam Successfully Connected to Vstore!" << endl;
  }
  else {
    cerr << " *ERROR: cannot connect to Vstore, terminating Vstore module*" << endl;
    return NULL;
  }

  vector<string> tables;
  SQLHSTMT stmt0;
  dbconn->getTables(&stmt0,&tables);

  for (int i=0;i<mio.values->keys().size();i++) {
      
      bool exists = false;
      string idx =  mio.values->keys().at(i);
      string create_vstore_table_query = "CREATE TABLE " + 
                                    idx + 
                                    "(tstamp INTEGER, value float);";
      cout << idx << endl; 
      for (int j=0;j<tables.size();j++) {
          
        if ( idx == tables[j] ) {
           exists = true;
           break;
        } 
      }

      if (exists) {
        cout << "Vstore table " << idx << " already exists, skipping..." << endl;         
      }

      else {
         cout << "Creating a Vstore Table for " << idx << endl;
         SQLHSTMT stmt1;
         dbconn->query(&stmt1,create_vstore_table_query);
         dbconn->freeStatement(&stmt1);
      }

  }
 
  while(1) {

    if (mio.mode == ADAM_MODE_BACKTEST || mio.mode == ADAM_MODE_GENETICS) {
      cout << "Adam is not in real mode, closing Vstore module..." << endl;
      return NULL;
    }

    iarray* tstamps = *(mio.tstamps);
    if (tstamps == NULL) {
      cout << "*ERROR: Invalid tstamps pointer ! *" << endl;
      return NULL;
    }

    if (tstamps->size >=10) {
      
         int start = tstamps->size - 10;
         int stop = tstamps->size; 

         for (int j=start;j<stop;j++) {
         for (int i=0; i< mio.values->keys().size();i++) {
            string idx = mio.values->keys().at(i);      
            
               SQLHSTMT stmt3;
               float v;
               if (mio.values->get(idx)->size >= 10) {
                  v = mio.values->get(idx)->values[j];
               }
               else v = 0;
               string insert_q = "INSERT INTO " + idx + 
                                 " VALUES ('" + 
                                  int2string(tstamps->values[j]) +
                                  "','" +
                                  float2string(v) +
                                  "');";
               dbconn->query(&stmt3,insert_q);
               dbconn->freeStatement(&stmt3);

            }
         }
      }
        sleep(10);
    }
  }


extern "C" void* module(module_io mio) {
  module_fct(mio);
  return NULL;

}
