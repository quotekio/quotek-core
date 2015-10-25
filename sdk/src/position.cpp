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
      
      ss << "ASSET:" << asset_name << std::endl;
      ss << "PNL:" << pnl << std::endl;
      ss << "OPEN:" << open << std::endl;
      ss << "STOP:" << stop << std::endl;
      ss << "LIMIT:" << limit << std::endl;
      ss << "OPEN_DATE:" << open_date << std::endl;
      ss << "CLOSE_DATE:" << close_date << std::endl;
      
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

  }
}
