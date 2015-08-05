/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include "techanalysis.hpp"

namespace quotek {

  
  namespace ta {

     std::vector<quotek::data::records> moving_average_convergeance_divergeance(std::vector<quotek::data::record>& recs,
                                                 int periods_short_ema, 
                                                 int periods_long_ema,
                                                 int periods_signal_line) {

      std::vector<quotek::data::records> result;

      quotek::data::records r1;
      result.emplace_back(r1);

      std::vector<float> ema1 = quotek::quant::EMA(recs,periods_short_ema);
      std::vector<float> ema2 = quotek::quant::EMA(recs,periods_long_ema);

      int sdiff_offset = ema1.size() - ema2.size();
      for (int i=0;i < ema2.size();i++) {
        float cmacd = ema1[i+sdiff_offset] - ema2[i];
        result[0].append( i, cmacd, 0 );
      }

      std::vector<float> sigline = quotek::quant::EMA(result[0].get_data(),9);

      quotek::data::records r2(sigline);
      result.emplace_back(r2);
      return result;

    }
    
  }
}
