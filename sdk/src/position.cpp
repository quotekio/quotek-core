/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#include "position.hpp"

namespace quotek {

  namespace core {
    
    position::position() {
      stats = new posstats();
    }

    position::~position() {
      delete stats;
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

    posstats::posstats() {
      this->pnl_peak = -100000000;
      this->vstop_increments = 0;
      this->vstop_decrements = 0;
      this->vlimit_increments = 0;
      this->vlimit_decrements = 0;
    }

  }
}
