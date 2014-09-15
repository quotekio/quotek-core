#ifndef STRATS_H
#define STRATS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "narrays.h"
#include "quant.h"
#include "finmath.h"
#include "store.h"
#include "queue_c.h"
#include "modules.h"
#include "position.h"
#include <string.h>
#include <time.h>

//!MH_APPEND!

typedef struct tradelife_io {
  char* ans;
  char* log_s;
  store* s;
  store* genes;
} tradelife_io;


typedef struct evaluate_io {
  const char* indice_name;
  farray* values;
  iarray* tstamps;
  char* ans;
  char* log_s;
  store* s;
  store* genes;
  struct evmio_array* evmio_a;
} evaluate_io;


typedef struct vpos {

  float open;
  float pnl;
  int pnl_pp;
  int size;

} vpos;


void* __order(const char*,const char*,const char*,int,int,int, char*);
uint8_t __is_time(const char* timestr,uint32_t t);
uint8_t __trade_hours(const char* h1,const char* h2,uint32_t t);
int __resolvT(int tinf,int tsup,uint32_t t,farray* values,iarray* tstamps,farray* result);
farray* __sampleT(int tinf,int tsup,uint32_t t,farray* values,iarray* tstamps);
int __counter(store* s,char* cname,int wait);
int ___log (const char* log_input, char* log_s);

#define SIGNAL_NONE 0x00
#define SIGNAL_BUY 0x01
#define SIGNAL_SELL 0x02


#define order(act,idc,w,n,stp,lm) (__order(act,idc,w,n,stp,lm,io->ans))
#define buy(idx,n,stp,lm) (__order("openpos",idx,"buy",n,stp,lm,io->ans))
#define sell(idx,n,stp,lm) (__order("openpos",idx,"sell",n,stp,lm,io->ans))

#define smartbuy(idx,lm) (__order("smartpos",idx,"buy",0,0,lm,io->ans) )
#define smartsell(idx,lm) (__order("smartpos",idx,"sell",0,0,lm,io->ans) )


#define close_pos(_dealid) (__order("closepos",_dealid,"",0,0,0,io->ans))


#define trade_hours(h1_str,h2_str) {  if (__trade_hours(h1_str,h2_str,t) == 0) { return NULL ; } }
#define is_time(_tstr) (__is_time(_tstr,t))

#define week_day() ( __week_day(t) )
#define month_day() (__month_day(t))


#define absolute(_fval) ( ( _fval * _fval ) / _fval )

#define sup(_fval) ( v > _fval )
#define inf(_fval) ( v < _fval )

#define supeq(_fval) ( v >= _fval )
#define infeq(_fval) ( v <= _fval )
#define eq(_fval)  ( v == _fval )

#define sample(_tinf,_tsup) (  __sampleT(_tinf,_tsup,t,io->values,io->tstamps) )

#define store(_a,_d) ( store_push(io->s,_a,_d) )
#define get(_a,_b) ( (_b) store_get(io->s,_a) )
#define stored(_a) ( (store_get(io->s,_a) == ERR ) ? 0 : 1 ) 

#define genetics(_a,_b) ( (_b) store_get(io->genes,_a) )  

#define counter(_a,_b) ( __counter(io->s,_a,_b) )
#define log(_log_input) ( ___log(_log_input,io->log_s) )

#endif
