/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS 
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
    #define FIBO_RETR(_a) fibo_retrace(_a)
    #define FIBO_EXT(_a) fibo_extension(_a)
    

    /** MACD is an early trend indicator. It is composed of 2 graph lines. The first line, MACD, is a difference between an EMA small period and an EMA long period.
     *  the second line, the signal line, is usually a 9 periods EMA.
     *  When the MACD line is going away from 0, then it susually means the current momentum is gaining strength.
     *  On the other side when it's nearing 0, then the momentum is losing strength.
     *  Also When the MACD crosses the signal line from above, it's a good insight for a sell signal.
     *  When the MACD crosses the signal line from below, it's a good insight for a buy signal.
     *  Note: the standard values for MACD periods is 12,26,9 and this is what we suggest to use.

     *  @param recs dataset to draw MACD for.
     *  @param periods_short_ema perdiod of the short moving average.
     *  @param periods_long_ema period of the long moving average.
     *  @param periods_signal_line period of the signal line EMA.
     *  @return the 2 MACD lines as a vector of quotek::data::records.
     */
    std::vector<quotek::data::records> moving_average_convergeance_divergeance(quotek::data::records& recs,
                                                 int periods_short_ema, 
                                                 int periods_long_ema,
                                                 int periods_signal_line);

    /** trix is an oscilator created to detect oversold or overbought markets.
     *  values returned by the trix indicator oscilate around a zero-line: If the
     *  value crosses above the zero-line it is a buy signal, if the value crosses below
     *  it is a sell signal.
     *  @param periods number of periods to compute trix for.
     *  @return the trix oscilation values for the provided datasets.
     */

    quotek::data::records trix(quotek::data::records& recs,
                            int periods);

    /** fibo_retrace takes a time-series and computes the different fibonacci 
     *  retracement levels and puts it in a quotek::quant::fibo_ret structure.
     *  @param recs dataset to compute fibonacci levels for.
     *  @return a quotek::quant::fibo_ret structure 
     */
     
    quotek::quant::fibo_ret fibo_retrace(quotek::data::records& recs);

    /** fibo_extension takes a time-series, computes the different fibonacci 
     *  extension levels and puts it in a quotek::quant::fibo_ext structure.
     *  @param recs dataset to compute fibonacci levels for.
     *  @return a quotek::quant::fibo_ext structure 
     */

    quotek::quant::fibo_ext fibo_extension(quotek::data::records& recs);

    /** pivots computes the pivot point as well as the four supports/resistances 
        for a given history.
        @param recs dataset to compute pivot points for.
        @return a quotek::quant::pivot structure
    */

    quotek::quant::pivot pivots(quotek::data::records& recs);

    /** same as previous, but takes (high,low,close) values instead of full 
     *  data history for computing.
     *  @return a quotek::quant::pivot structure
     */

    quotek::quant::pivot pivots(float high, float low, float close);
    


  }
}

#endif
