/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace quotek {

  namespace core {
  	#define S10 10
  	#define S30 30
  	#define S50 50
    #define m1 60
    #define m5 300
    #define m10 600
    #define m15 900
    #define H1 3600
    #define H2 7200
    #define H4 14400
    #define H10 36000
    #define D1 86400
    #define M1 2592000
    #define Y1 31104000
    
    #ifdef _WIN64
    #define TMPDIR "%%Temp%%"
    #else
    #define TMPDIR "/tmp/"
    #endif
    
    #define SDK_VERSION "Quotek SDK v2.2.2"


  }
}

#endif
