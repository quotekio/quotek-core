#ifndef RESULTS_H
#define RESULTS_H

#include <string>
#include <vector>
#include "utils.h"

using namespace std;

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

      string res = "{";
      res += "\"name\": " + name + ",\n";
      res += "\"open\": " + float2string(open) + ",\n";
      res += "\"close\": " + float2string(close) + ",\n";
      res += "\"pnl\": " + float2string(pnl) + ",\n";
      res += "\"opentime\": " + int2string(opentime) + ",\n";
      res += "\"closetime\": " + int2string(closetime) + ",\n";

      res += "}";
      return res;

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
      string res = "{";
      res += "\t\"name\": " + name + ",\n";
      res += "\t\"deviation\": " + float2string(deviation) + ",\n";
      res += "\t\"variation\": " + float2string(variation) + ",\n";
      res += "\t\"highest\": " + float2string(highest) + ",\n";
      res += "\t\"lowest\": " + float2string(lowest) + ",\n";
      res+= "}";
      return res;
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
    vector<snappos> positions;
    vector<assetstats*> astats;

  	string json_encode() {

      string res = "{";
      res += "\"start\": " + int2string(start) + ",\n";
      res += "\"stop\": " + int2string(stop) + ",\n";
      res += "\"from\": " + int2string(from) + ",\n";
      res += "\"to\": " + int2string(to) + ",\n";
      res += "\"pnl\": " + float2string(pnl) + ",\n";
      res += "\"remainingpos\": " + int2string(remainingpos) + ",\n";

      res+="\"positions\": [";
      for(int i=0;i<positions.size();i++) {
        res += positions[i].json_encode() + ",\n";
      }
      res +="],\n";

      res+="\"astats\": [";
      for(int i=0;i<astats.size();i++) {
        res += astats[i]->json_encode() + ",\n";
      }
      res +="]";

      
      res += "}";
      return res;

  	}
    
};

#endif
