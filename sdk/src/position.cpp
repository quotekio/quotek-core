/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include "position.hpp"

namespace quotek {

  namespace core {
    
    position::position() {
      stats = new posstats();
      this->vstop = 0;
      this->vlimit = 0;
    }

    position::~position() {
      
    }

    void position::set_vstop(int new_vstop) {

      if (new_vstop > this->vstop) this->stats->vstop_increments++;
      else this->stats->vstop_decrements++;
      this->vstop = new_vstop;
    }

    void position::set_vlimit(int new_vlimit) {
      if (new_vlimit > this->vlimit) this->stats->vlimit_increments++;
      else this->stats->vlimit_decrements++;
      this->vlimit = new_vlimit;
    }

    void position::vshift(int offset) {

      this->vstop += offset;
      if (this->limit != 0) this->limit += offset;
      if (offset < 0) {
        this->stats->vstop_increments++;
        this->stats->vlimit_decrements++;
      }

      else {
        this->stats->vstop_decrements++;
        this->stats->vlimit_increments++;
      }

    }


    float position::get_vstop() {
      return this->vstop;
    }

    float position::get_vlimit() {
      return this->vlimit;
    }

    
    std::string position::str() {

      std::stringstream ss;

      ss << "{";
      ss << "\"asset_name\":" << "\"" << asset_name << "\",";

      ss << "\"size\":" << size << ",";      

      ss << "\"ticket_id\":" << "\"" << ticket_id << "\",";
      ss << "\"asset_id\":" << "\"" << asset_id << "\",";
      ss << "\"identifier\":" << "\"" << identifier << "\",";

      ss << "\"pnl\":" << pnl << ",";
      ss << "\"open\":" << open << ",";
      ss << "\"stop\":" << stop << ",";
      ss << "\"limit\":" << limit << ",";


      ss << "\"open_date\":" << open_date << ",";
      ss << "\"close_date\":" << close_date << ",";

      ss << "\"stats\":" << stats->str();

      ss << "}";
      return ss.str();

    }


    posstats::posstats() {
      this->pnl_peak = -100000000;
      this->vstop_increments = 0;
      this->vstop_decrements = 0;
      this->vlimit_increments = 0;
      this->vlimit_decrements = 0;
    }

    posstats::~posstats() {
      
    }

    std::string posstats::str() {

      std::stringstream ss;

      ss << "{";
      
      ss << "\"pnl_peak\":" << pnl_peak << ",";  
      ss << "\"vstop_increments\":" << vstop_increments << ",";
      ss << "\"vstop_decrements\":" << vstop_decrements << ",";
      ss << "\"vlimit_increments\":" << vlimit_increments << ",";
      ss << "\"vlimit_decrements\":" << vlimit_decrements ;

      ss << "}";
      return ss.str();

    }

  }
}
