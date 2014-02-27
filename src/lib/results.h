#ifndef RESULTS_H
#define RESULTS_H

#include <string>
#include <vector>
#include "utils.h"
#include "position.h"

using namespace std;

inline string pos2json(position* p) {

  stringstream ss;

  ss << "{";
  ss << "\"asset\":\"" << p->indice << "\",\n";
  ss << "\"epic\":\"" << p->epic << "\",\n";
  ss << "\"dealid\":\"" << p->dealid << "\",\n";
  ss << "\"name\":\"" << p->name << "\",\n";
  
  if (  p->size > 0 ) {
    ss << "\"way\":\"buy\",\n"; 
  }
  else ss << "\"way\":\"sell\",\n"; 

  ss << "\"nbc\":" << p->size * p->size / p->size << ",\n";

  ss << "\"stop\":" << p->stop << ",\n";
  ss << "\"limit\":" << p->limit << ",\n";
  ss << "\"pnl\":" << p->pnl << ",\n";
  ss << "\"open_time\":" << p->open_time << ",\n";
  ss << "\"close_time\":" << p->close_time << "\n";
  ss << "}";

  return ss.str();
}



class geneticresult {

  public:
    geneticresult() {

    }
    
};

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
  	vector<geneticresult> genetics;
    vector<assetstats*> astats;
    vector<string> loglines;
    vector<position> positions_history;

  	string json_encode() {

      stringstream ss;

      ss << "{";
      ss <<  "\"start\": " << start << ",\n";
      ss <<  "\"stop\": " << stop << ",\n";
      ss <<  "\"from\": " << from << ",\n";
      ss <<  "\"to\": " << to << ",\n";
      ss <<  "\"pnl\": " << pnl << ",\n";
      ss <<  "\"remainingpos\": " << remainingpos << ",\n";

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
    
};

#endif
