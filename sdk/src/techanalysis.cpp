/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
http://www.quotek.io
*/

#include "techanalysis.hpp"

namespace quotek {

  
  namespace ta {

     std::vector<quotek::data::records> moving_average_convergeance_divergeance(quotek::data::records& recs,
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

      std::vector<float> sigline = quotek::quant::EMA(result[0],9);

      quotek::data::records r2(sigline);
      result.emplace_back(r2);
      return result;

    }

    quotek::data::records trix(quotek::data::records& recs,
                            int periods) {

      quotek::data::records result;

      //ema
      std::vector<float> ema1 = quotek::quant::EMA(recs, periods);
      quotek::data::records recs1 = quotek::data::records(ema1);

      //ema of ema
      std::vector<float> ema2 = quotek::quant::EMA(recs1, periods);
      quotek::data::records recs2 = quotek::data::records(ema2);

      //ema of ema of ema
      std::vector<float> ema3 = quotek::quant::EMA(recs2, periods);
      quotek::data::records recs3 = quotek::data::records(ema3);

      for ( int i=1;i< ema3.size();i++ ) {
         result.append( ( ema3[i] - ema3[i-1]) / ema3[i-1] );
       }

      return result;     
    }

    quotek::quant::fibo_ret fibo_retrace(quotek::data::records& recs) {

      quotek::quant::fibo_ret result;

      float max = quotek::quant::max(recs);
      float min = quotek::quant::min(recs);
      float delta = max - min;

      result.p0 = min;
      result.p23 = min + (23.6 * delta / 100 );
      result.p38 = min + (38.2 * delta / 100);
      result.p50 = min + (delta / 2 );
      result.p61 = min + (61.8 * delta / 100 );
      result.p100 = max;

      return result;
    }
    
    quotek::quant::fibo_ext fibo_extension(quotek::data::records& recs) {

      quotek::quant::fibo_ext result;

      float max = quotek::quant::max(recs);
      float min = quotek::quant::min(recs);
      float delta = max - min;

      result.p100 = max;
      result.p127 = min + (127.2 * delta / 100);
      result.p161 = min + (161.8 * delta / 100 );
      result.p200 = 2 * max;
      result.p261 = min + (261.8 * delta / 100 );

      return result;
    }

    quotek::quant::pivot pivots(float high, float low, float close) {

      quotek::quant::pivot result;

      result.pivot = (high + low + close ) / 3;
      result.s1 = 2 * result.pivot - high;
      result.s2 = result.pivot - ( high - low );
      result.s3 = result.pivot - 2 * (high - low);
      
      result.r1 = 2 * result.pivot - low;
      result.r2 = result.pivot + (high - low);
      result.r3 = result.pivot + 2 * (high - low);

      return result;

    }

    quotek::quant::pivot pivots(quotek::data::records& recs) {

      float high = quotek::quant::max(recs);
      float low = quotek::quant::min(recs);
      float close = recs[recs.size()-1].value;

      return pivots(high,low,close);
      
    }

  }

}
