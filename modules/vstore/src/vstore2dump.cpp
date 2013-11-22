#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include "../../../src/lib/assoc.h"
#include "../../../src/lib/utils.h"
#include "odbc.h"

using namespace std;

void usage(char* cmd) {
  cout << "Usage: " << cmd << " <start> <end> <output> <VALUE_1> [VALUE_2] [VALUE_N]" << endl;
  exit(1);
}

typedef struct vprocess {
  string query;
  SQLHSTMT stmt;
  vector<  vector<string>  > data;
} vprocess;


void writeToFile(ofstream* ofh,AssocArray<vprocess>* vproc) {

  cout << "+";

  for (int i=0;i<vproc->at(0).data.size();i++) {
     *ofh << "T:" << vproc->at(0).data[i][0] << endl;
  }

  for (int i=0;i<vproc->Size();i++) {
    for (int j=0;j< vproc->at(i).data.size();j++ ) {
       *ofh << vproc->GetItemName(i) << ":" << vproc->at(i).data[j][1] << endl;

    }
    vproc->at(i).data.clear();
  }

}



int main(int argc,char** argv) {
  if (argc < 5) {
    usage(argv[0]);
  } 

  AssocArray<vprocess> vproc;

  vector<string> values;

  string tt_inf = argv[1];
  string tt_sup = argv[2];
  ofstream ofh (argv[3]);
  
  if (! ofh.is_open()) {
    cerr << "* ERROR: cannot open " << argv[3] << "for writing" << endl;
    exit(1); 
  }

  for (int i=4;i<argc;i++) {
    //values.push_back(string(argv[i]));
    
    vproc[argv[i]].query = "SELECT * FROM " + 
                           string(argv[i]) + 
                           " WHERE tstamp >= " + 
                           tt_inf + 
                           " AND tstamp <= " + 
                           tt_sup + 
                           " ORDER BY tstamp ASC;";
  }

  odbc* dbconn = new odbc();
  dbconn->connect("DSN=vstore_sqlite;");

  if (dbconn->isConnected()) {
    cout << "Connected to vstore !" << endl;
  }
  else {
    cerr << "* ERROR: cannot connect to vstore ! *" << endl;
    exit(1);
  }

  for (int i=0;i<vproc.Size();i++) {
    dbconn->query( &(vproc[i].stmt),vproc[i].query);    
  }

  while(1) {

    if ( vproc[0].data.size() == 50 ) writeToFile(&ofh, &vproc);

    for (int i=0;i<vproc.Size();i++) {

      vector<string> ans;
      int retval = dbconn->fetch(&(vproc[i].stmt),&ans);
      if (retval == 1) {
         vproc[i].data.push_back(ans);
         ans.clear();
      }
      else {
        ofh.close();
        exit(0);
      }
    }     
  }
  ofh.close();
}