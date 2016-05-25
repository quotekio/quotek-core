/*
Quotek Strategies SDK 3.0
Copyright 2013-2016 Quotek SAS
http://www.quotek.io
*/

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace quotek {

  namespace core {

  	#define T_S10 10
  	#define T_S30 30
  	#define T_S50 50
    #define T_m1 60
    #define T_m5 300
    #define T_m10 600
    #define T_m15 900
    #define T_H1 3600
    #define T_H2 7200
    #define T_H4 14400
    #define T_H10 36000
    #define T_D1 86400
    #define T_M1 2592000
    #define T_Y1 31104000
    
    #ifdef _WIN64
    #define TMPDIR "%%Temp%%"
    #else
    #define TMPDIR "/tmp/"
    #endif
    
    #define SDK_VERSION "Quotek SDK v3.0"


  }
}

#endif
