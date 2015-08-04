/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS 
http://www.quotek.io
*/

#ifndef TECHANALYSIS_HPP
#define TECHANALYSIS_HPP

#include "quant.hpp"
#include "record.hpp"

namespace quotek {

  /** ta namespace contains a set of functions meant to facilitate automated technical
   *  analysis.
   */
  namespace ta {

    #define MACD(_a,_b,_c,_d) moving_average_convergeance_divergeance(_a,_b,_c,_d)


    /** Computes MACD */

    void moving_average_convergeance_divergeance(std::vector<quotek::data::record>& recs,
                                                 int periods_ema1, 
                                                 int periods_ema2,
                                                 int periods_signal_line);




  }
}

#endif
