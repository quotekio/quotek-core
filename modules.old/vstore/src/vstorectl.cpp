#include "odbc.h"
#include <iostream>

using namespace std;

odbc* dbconn = new odbc();

void usage() {
  cout << "Usage: vstorectl <command [command_args]>" << endl;
  cout << "Available commands:\n\n\tclean: Removes invalid values inside vstore tables" << endl;
  cout << "\tflush: Removes all data from vstore tables but keeps the data schemes." << endl;
  exit(1);
}


int clean() {
  SQLHSTMT stmt;
  vector<string> tables;
  dbconn->getTables(&stmt,&tables);

  for (int i=0;i<tables.size();i++) {

    cout << "Cleaning table " << tables[i] << " ..." << endl;
    string clean_q = "DELETE FROM " + tables[i] + " WHERE value <= '0.0';";
    dbconn->query(&stmt,clean_q);

  }

  return 0;
}


int flush() {
 
  SQLHSTMT stmt;
  vector<string> tables;
  dbconn->getTables(&stmt,&tables);

  for (int i=0;i<tables.size();i++) {

    cout << "flushing table " << tables[i] << " ..." << endl;
    string clean_q = "DELETE FROM " + tables[i] + ";";
    dbconn->query(&stmt,clean_q);

  }
  return 0;
}


int main(int argc, char** argv) {
  cout << "[ ADAM VSTORE CONTROL UTILITY v1.0 ]\n" << endl;
  if (argc < 2) {
    usage();
  }

  dbconn->connect("DSN=vstore_sqlite;");

  if (dbconn->isConnected()) {
    cout << "Connected to vstore !" << endl;
  }
  else {
    cerr << "* ERROR: cannot connect to vstore ! *" << endl;
    exit(1);
  }

  string main_cmd = std::string(argv[1]);

  if ( main_cmd == "clean" ) {
    clean();
  }

  else if (main_cmd == "flush") {
    flush();
  }

}