#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include "../../../src/lib/assoc.h"
#include "../../../src/lib/utils.h"
#include "odbc.h"

#define BUFF_ENTRIES_SIZE  50


typedef struct ventry {

  string tstamp;
  string value;

} ventry;


using namespace std;


void usage(char* cmd) {
  cout << "Usage: " << cmd << " <input>" << endl;
  exit(1);
}


void insert(odbc* dbconn,string tstamp,string idx,string v) {

  cout << "INSERT (" + tstamp + "," + v + ") INTO " +  idx << endl;

  string q = "INSERT INTO " + idx + " VALUES ('" + 
             tstamp + 
             "','" +
             v +
             "');";
          
  SQLHSTMT stmt;
  dbconn->query(&stmt,q);
  dbconn->freeStatement(&stmt);

}


void bulkInsert(odbc* dbconn,string idx, vector<ventry>* ve) {

   string q = "INSERT INTO " + idx + " VALUES ";
   for (int i=0;i<ve->size();i++) {

     q+= "('" + ve->at(i).tstamp + "','" +  ve->at(i).value + "'),";
   }

   q[q.length()-1] = ';';
  
   SQLHSTMT stmt;
   dbconn->query(&stmt,q);
   dbconn->freeStatement(&stmt);
   ve->clear();
}



void createTable(odbc* dbconn,string idx) {

  string q = "CREATE TABLE " + idx + 
             " (tstamp INTEGER, value FLOAT);";

  SQLHSTMT stmt;
  dbconn->query(&stmt,q);
  dbconn->freeStatement(&stmt);

} 

int main(int argc,char** argv) {
  if (argc < 2) {
    usage(argv[0]);
  } 

  ifstream ifh;
  ifh.open(argv[1]);
  if (! ifh.is_open()) {
    cerr << "* ERROR: cannot open input dump file*" << endl;
    exit(1);
  }

  odbc* dbconn = new odbc();
  dbconn->connect("DSN=vstore_sqlite;");

  if (!dbconn->isConnected()) {
    cerr << "* ERROR: cannot connect to vstore *" << endl;
    exit(1);
  }
  
  else {
    cout << "Connected to vstore !" << endl;
  }
    
  vector<string> tables;
  SQLHSTMT stmt;
  dbconn->getTables(&stmt,&tables);
  string line;

  string tstamp = "123";

  AssocArray< vector<ventry> > buff;

  while(ifh.good()){

    for (int i=0;i<buff.Size();i++) {
       if (  buff[i].size() == BUFF_ENTRIES_SIZE ) {
          bulkInsert(dbconn, buff.GetItemName(i), &(buff[i]) );
          cout << "+";
       }
    }

    vector<string> args;
    getline(ifh,line);
    //cout << line << endl;
    args = split(line,':');
    if (args.size() == 2) {
      if (args[0] == "T") {
        tstamp = args[1];
      }

      else {
        ventry v;
        v.tstamp = tstamp;
        v.value = args[1];
        if ( ! vector_in_str(&tables,args[0]) ) {
          createTable(dbconn,args[0]);
          tables.push_back(args[0]);
        }
        buff[args[0]].push_back(v);
      }
    }
  }

}