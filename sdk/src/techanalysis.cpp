/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include "techanalysis.hpp"

namespace quotek {

  /** ta namespace contains a set of functions meant to facilitate automated technical
   *  analysis.
   */
  namespace ta {

    void moving_average_convergeance_divergeance(std::vector<quotek::data::record>& recs,
                                                 int periods_ema1, 
                                                 int periods_ema2,
                                                 int periods_signal_line) {

      std::vector<float> ema1 = quotek::quant::EMA(recs,periods_ema1);
      std::vector<float> ema2 = quotek::quant::EMA(recs,periods_ema2);

      std::vector<float> trendline = quotek::quant::EMA(recs,periods_signal_line);

      for (int i=0;i < ema1.size();i++) {
        
      }

    }
    
  }
}
