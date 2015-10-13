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
    #define FIBO(_a) fibo_retrace(_a)

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

    /** fibo_retrace takes a time-series and computes the different fibonacci 
     *  retrace levels and puts it in a quotek::quant::fibo_levels structure.
     *  @param recs dataset to compute fibonacci levels for.
     *  @return a quotek::quant::fibo_levels structure 
     */

    quotek::quant::fibo_levels fibo_retrace(quotek::data::records& recs);

  }
}

#endif
