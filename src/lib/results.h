#ifndef RESULTS_H
#define RESULTS_H

#include <string>
#include <vector>
#include "utils.h"
#include <fstream>

#include <quotek/position.hpp>

using namespace std;


class qateresult {

  public:
  	qateresult() {

      //initializing of member values;

      this->start = 0;
      this->stop = 0;
      this->from = 0;
      this->to = 0;
      this->pnl = 0;
      this->remainingpos = 0;
      this->max_drawdown = 0;
      this->profit_factor = 0;
      this->winning_trades = 0;
      this->losing_trades = 0;
      this->returns_percent = 0;
      this->duration = 0;
      this->generation_id = 0;
      this->individual_id = 0;

  	}


  	int start;
  	int stop;
  	int from;  	
  	int to;
    float pnl;
    int remainingpos;

    float max_drawdown;
    float profit_factor;

    int winning_trades;
    int losing_trades;
    
    float returns_percent;
    float duration;

    /* for genetic results */
    int generation_id;
    int individual_id;
    vector<string> genes_repr;
    
    vector<string> loglines;
    vector<quotek::core::position> positions_history;

  	string json_encode() {

      stringstream ss;

      ss <<  "{";
      ss <<  "\"generation\": " << generation_id << ",\n";
      ss <<  "\"individual\": " << individual_id << ",\n";
      ss <<  "\"start\": " << start << ",\n";
      ss <<  "\"duration\": " << duration << ",\n";
      ss <<  "\"from\": " << from << ",\n";
      ss <<  "\"to\": " << to << ",\n";
      ss << "\"pnl\": " << pnl << ",\n";
      ss << "\"returns_percent\": " << returns_percent << ",\n";
      ss <<  "\"max_drawdown\": " << max_drawdown << ",\n";
      ss <<  "\"profit_factor\": " << profit_factor << ",\n";
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

      ss << "\"trades\": {";
      ss << "\"winning\":" << winning_trades << ",\n";
      ss << "\"losing\":" << losing_trades << ",\n";
      ss << "\"list\":[\n";

      for(int i=0;i<positions_history.size();i++) {
        ss << positions_history[i].str();
        if (i < positions_history.size()-1) ss << ",\n";
        else ss << "\n";
      }
      ss << "]},\n";

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


//class made to store all the iterations.
class qateResultsHandler {
  public:
    std::vector<qateresult*> entries;
    std::string output_file;
    int bt_type;

    qateResultsHandler(std::string output_file, int bt_type) {
      this->output_file = output_file;
      this->bt_type = bt_type;
    }


    string json_encode() {
      stringstream res;

      res << "{ \"type\":" << this->bt_type ;
      res << "\", results\": [";

      for (int i=0;i<entries.size();i++) {
        res << entries[i]->json_encode();
        if (i < entries.size() - 1 ) res << ",";
      }

      res << "]";

      res << "}";

      return res.str();

    }

    int save() {

      ofstream ofh (this->output_file.c_str());

      if (! ofh.good()) return -1;
      
      ofh << json_encode();
      ofh.close();

      return 0;
    }

};



#endif
