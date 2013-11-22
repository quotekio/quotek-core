#include "igmlogger.h"

igmLogger::igmLogger() {

  out.open("/var/log/adam.log",std::ofstream::out | std::ofstream::app);

  if ( ! out.is_open() ) {
    cout << "cannot open /var/log/adam.log for writing" << endl;
    exit(1);
  } 
 }

void igmLogger::log(string logstr) {

  log_entry le;
  le.tstamp = time(NULL);
  le.entry = "[" + currentDateTime() + "] " +logstr;
  entries.push_back(le);
  out << "[" << currentDateTime() << "] " << logstr << endl;
}


vector<log_entry> igmLogger::getLastEntries(int nb_entries) {

  int start = 0;
  vector<log_entry> res;

  if (entries.size() < nb_entries) start = 0;
  else start = entries.size() - nb_entries;

  for(int i=start;i<entries.size();i++) { 
    res.push_back( entries[i]);
  }

  return res;

}


ofstream* igmLogger::getStream() {
  return &out;
}

