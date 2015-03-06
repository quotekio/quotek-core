#include "strats.h"


void* __order(const char* action,const char* indice,const char* way, int nbc, int stop,int limit, Queue_c* orders ) {

  char* order = (char*) malloc(1024*sizeof(char));

  if (strcmp(action,"openpos") == 0 ) {
    sprintf(order,"openpos:%s:%s:%d:%d:%d", indice, way, nbc,stop,limit);
  }

  else if (strcmp(action,"smartpos") == 0 ) {
    sprintf(order,"smartpos:%s:%s:%d", indice, way,limit);
  }
  

  else if (strcmp(action,"closepos") == 0 ) {
    sprintf(order,"closepos:%s", indice);
  }

  else if (strcmp(action,"closeallpos") == 0 ) {
    sprintf(order,"closeallpos:%s:%s", indice,way);
  }
  
  Enqueue(order, *orders);

}


uint8_t __is_time(const char* timestr,uint32_t t) {

  int hour,minute,second;

  const time_t curtime = (const time_t) t;
  struct tm c_time ;
  localtime_r(&curtime,&c_time);



  int i =0;
  int nb_sep=0;

  for (i=0;i<strlen(timestr);i++) {
 
    if (timestr[i] == ':') nb_sep++;
  }

  if (nb_sep == 0) return 0;  
  else if (nb_sep == 2) {
    sscanf(timestr,"%d:%d:%d",&hour,&minute,&second);
    if (  c_time.tm_hour == hour && c_time.tm_min == minute && c_time.tm_sec == second ) return 1;
    return 0;
  }
  else if (nb_sep == 1) {
    sscanf(timestr,"%d:%d",&hour,&minute);
    if (  c_time.tm_hour == hour && c_time.tm_min == minute ) return 1;
    return 0;
  }

}

const char* __week_day(int t) {

  const char* weekdays[] = { "monday", "tuesday","weddnesday","thursday","friday","saturday","sunday" };
  int cweekday = 1;
  time_t curtime = (time_t) t;
  cweekday = localtime(&curtime)->tm_wday;
  return weekdays[cweekday];
}

uint8_t __month_day(int t) {
  int cmday = 1;
  time_t curtime = (time_t) t;
  cmday = localtime(&curtime)->tm_mday;
  return cmday;
}

uint8_t __trade_hours(const char* h1,const char* h2,uint32_t t) {

  t+=3600;

  struct tm* c_time; 
  char datestr[32];
  char datestr_h1[64];
  char datestr_h2[64];

  struct tm tm_h1;
  struct tm tm_h2;

  time_t t_h1;
  time_t t_h2;

  c_time = localtime((const time_t*) &t);   
  sprintf(datestr,
        "%d-%02d-%02d ",
        c_time->tm_year+1900,
        c_time->tm_mon+1,
        c_time->tm_mday
        );

  strcpy(datestr_h1,datestr);
  strcat(datestr_h1,h1);

  strcpy(datestr_h2,datestr);
  strcat(datestr_h2,h2);

  strptime(datestr_h1,"%Y-%m-%d %H:%M:%S",&tm_h1);
  t_h1 = mktime(&tm_h1);

  strptime(datestr_h2,"%Y-%m-%d %H:%M:%S",&tm_h2);
  t_h2 = mktime(&tm_h2);

  if (t >= t_h1 && t <= t_h2 ) return 1;
  else return 0;

 }


farray* __sampleT(int tinf,int tsup,uint32_t t,farray* values,iarray* tstamps) {

  farray* result = (farray*) malloc(1*sizeof(farray));

  int indice_inf = 0;
  int indice_sup = 0;
  int i = 0;

  if (tinf < 0) {
    tinf = t + tinf;
  }   

  if (tsup <= 0) {
    tsup = t + tsup;
  }

  indice_inf = iarray_dicho_search(tstamps,tinf);
  indice_sup = iarray_dicho_search(tstamps,tsup);
  
  if (indice_inf == -1 || indice_sup == -1) {
    return NULL;
  }

  result->values = values->values + indice_inf;
  result->size = indice_sup - indice_inf;

  return result;
 }


 int __counter(store* s,char* cname,int wait) {

  if (store_get(s,cname) != NULL ) {
    int* i = (int*) store_get(s, cname);
    *i = *i + 1;

    if (*i == wait) {
      *i=1;
      store_push(s,cname,i);
      return 1;
    }

    else {
      store_push(s,cname,i);
      return 0;
    }
  }

  else {
    int* new_i = (int*) malloc(sizeof(int));
    *new_i=1;
    store_push(s,cname,new_i);
    return 0;
  }

}


int ___log (const char* log_input, Queue_c* logs) {
  if (strlen(log_input) > 1024 ) return -1;

  char* log = (char*) malloc( sizeof(char) * 1024 );
  Enqueue(log, *logs);
  
  return 0;
}

