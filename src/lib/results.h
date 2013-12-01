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
      res += "\"name\": " + name + ",";
      res += "\"open\": " + float2string(open) + ",";
      res += "\"close\": " + float2string(close) + ",";
      res += "\"pnl\": " + float2string(pnl) + ",";
      res += "\"opentime\": " + int2string(opentime) + ",";
      res += "\"closetime\": " + int2string(closetime) + ",";

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
      res += "\"name\": " + name + ",";
      res += "\"deviation\": " + float2string(deviation) + ",";
      res += "\"variation\": " + float2string(variation) + ",";
      res += "\"highest\": " + float2string(highest) + ",";
      res += "\"lowest\": " + float2string(lowest) + ",";
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
    vector<assetstats> astats;

  	string json_encode() {

      string res = "{";
      res += "\"start\": " + int2string(start) + ",";
      res += "\"stop\": " + int2string(stop) + ",";
      res += "\"from\": " + int2string(from) + ",";
      res += "\"to\": " + int2string(to) + ",";
      res += "\"pnl\": " + float2string(pnl) + ",";
      res += "\"remainingpos\": " + int2string(remainingpos) + ",";

      res+="\"positions\": [";
      for(int i=0;i<positions.size();i++) {
        res += positions[i].json_encode() + ",";
      }
      res +="]";

      res+="\"positions\": [";
      for(int i=0;i<astats.size();i++) {
        res += astats[i].json_encode() + ",";
      }
      res +="]";

      
      res += "}";
      return res;

  	}
    
};