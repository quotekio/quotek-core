#include "igmlogger.h"

igmLogger::igmLogger() {

  use_stdout = false;

  out.open("/var/log/qate.log",std::ofstream::out | std::ofstream::app);

  if ( ! out.is_open() ) {
    cout << "cannot open /var/log/qate.log for writing" << endl;
    exit(1);
  } 
 }

igmLogger::igmLogger(bool uout) {

  use_stdout = uout;
  out.open("/var/log/qate.log",std::ofstream::out | std::ofstream::app);

  if ( ! out.is_open() ) {
    cout << "cannot open /var/log/qate.log for writing" << endl;
    exit(1);
  }

}


void igmLogger::log(string logstr) {

  log_entry le;
  le.tstamp = time(NULL);
  le.entry = "[" + currentDateTime() + "] " +logstr;
  entries.push_back(le);
  out << "[" << currentDateTime() << "] " << logstr << endl;

  if (use_stdout) {
    cout << "[" << currentDateTime() << "] " << logstr << endl;
  }

}

void igmLogger::log(string logstr, int tstamp) {

  log_entry le;
  le.tstamp = time(NULL);
  le.entry = "[" + epochToDateTime(tstamp) + "] " +logstr;
  entries.push_back(le);
  out << "[" << epochToDateTime(tstamp) << "] " << logstr << endl;

  if (use_stdout) {
    cout << "[" << epochToDateTime(tstamp) << "] " << logstr << endl;
  }

}


vector<log_entry>* igmLogger::getAllEntries() {
  return &entries;
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

void igmLogger::reset() {
  clear();
}

void igmLogger::clear() {
  entries.clear();
}
