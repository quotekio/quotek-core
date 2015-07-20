#ifndef RESULTS_H
#define RESULTS_H

#include <string>
#include <vector>
#include "utils.h"
#include <fstream>

#include <quotek/position.hpp>

using namespace std;

inline string pos2json(quotek::core::position* p) {

  stringstream ss;

  ss << "{";
  ss << "\"asset\":\"" << p->asset_name << "\",\n";
  ss << "\"broker_id\":\"" << p->asset_id << "\",\n";
  ss << "\"ticket_id\":\"" << p->ticket_id << "\",\n";
  
  if (  p->size > 0 ) {
    ss << "\"way\":\"buy\",\n"; 
  }
  else ss << "\"way\":\"sell\",\n"; 

  ss << "\"nbc\":" << p->size * p->size / p->size << ",\n";

  ss << "\"stop\":" << p->stop << ",\n";
  ss << "\"limit\":" << p->limit << ",\n";
  ss << "\"pnl\":" << p->pnl << ",\n";
  ss << "\"open_date\":" << p->open_date << ",\n";
  ss << "\"close_date\":" << p->close_date << "\n";
  ss << "}";

  return ss.str();
}


class snappos {

  public:
    string name;
    float open;
    float close;
    float pnl;
    int opentime;
    int closetime;
    snappos() {}
    string json_encode() {

      stringstream ss;
      ss << "{";
      ss << "\"name\": \"" << name << "\",\n";
      ss << "\"open\": " << open << ",\n";
      ss << "\"close\": " << close << ",\n";
      ss << "\"pnl\": " << pnl << ",\n";
      ss << "\"opentime\": " << opentime << ",\n";
      ss << "\"closetime\": " << closetime << "\n";
      ss << "}";

      return ss.str();

    }

};

class assetstats {

  public:
    string name;
    float deviation;
    float variation;
    float highest;
    float lowest;
    assetstats() {}

    string json_encode() {
      stringstream ss;
      ss << "{";
      ss << "\t\"name\": \"" << name << "\",\n";
      ss << "\t\"deviation\": " << deviation << ",\n";
      ss << "\t\"variation\": " << variation << ",\n";
      ss << "\t\"highest\": " << highest << ",\n";
      ss << "\t\"lowest\": " << lowest << "\n";
      ss << "}";
      return ss.str();
    }


};


class adamresult {

  public:
  	adamresult() {
  	}
  	int start;
  	int stop;
  	int from;  	
  	int to;
    float pnl;
    int remainingpos;

    /* for genetic results */
    int generation_id;
    int individual_id;
    vector<string> genes_repr;
    
    vector<assetstats*> astats;
    vector<string> loglines;
    vector<quotek::core::position> positions_history;

  	string json_encode() {

      stringstream ss;

      ss <<  "{";
      ss <<  "\"generation\": " << generation_id << ",\n";
      ss <<  "\"individual\": " << individual_id << ",\n";
      ss <<  "\"start\": " << start << ",\n";
      ss <<  "\"stop\": " << stop << ",\n";
      ss <<  "\"from\": " << from << ",\n";
      ss <<  "\"to\": " << to << ",\n";
      ss <<  "\"pnl\": " << pnl << ",\n";
      ss <<  "\"remainingpos\": " << remainingpos << ",\n";

    
      if ( genes_repr.size() > 0 ) {
        
        ss << "\"genes\": [";
        for(int i=0;i < genes_repr.size();i++) {
          ss <<  "\"" << genes_repr[i] << "\"" ;
          if (i < genes_repr.size()-1) ss << ",\n";
          else ss << "\n";
        }
        ss << "],\n";

      }

      ss << "\"positions\": [";
      for(int i=0;i<positions_history.size();i++) {
        ss <<  pos2json(&positions_history[i]);
        if (i < positions_history.size()-1) ss << ",\n";
        else ss << "\n";
      }
      ss << "],\n";

      ss << "\"astats\": [";
      for(int i=0;i<astats.size();i++) {
        ss <<  astats[i]->json_encode();
        if (i< astats.size()-1) ss << ",\n";
        else ss << "\n";
      }
      ss << "],";

      ss << "\"logs\" : [";

      for (int i=0;i<loglines.size();i++) {
        ss << "\"" << json_escape(loglines[i]) << "\"";
        if (i< loglines.size()-1) ss << ",\n";
        else ss << "\n";
      }

      ss << "]";
      ss <<  "}";
      return ss.str();

  	}
   
    int saveToFile(string fname) {

      ofstream ofh (fname.c_str());
      
      if (! ofh.good()) return -1;
      
      ofh << json_encode();
      ofh.close();

      return 0;
    }

};


//class made to store all the iterations.
class adamGeneticsResult {
  public:
    vector<adamresult*> entries;

    string json_encode() {
      stringstream res;
      res << "[";

      for (int i=0;i<entries.size();i++) {
        res << entries[i]->json_encode();
        if (i < entries.size() - 1 ) res << ",";
      }

      res << "]";

      return res.str();

    }

    int saveToFile(string fname) {

      ofstream ofh (fname.c_str());

      if (! ofh.good()) return -1;
      
      ofh << json_encode();
      ofh.close();

      return 0;
    }

};



#endif
