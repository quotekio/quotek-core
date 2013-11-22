#include "strats.h"


void* __order(const char* action,const char* indice,const char* way, int nbc, int stop,int limit, char* ans ) {


  if (strcmp(action,"openpos") == 0 ) {
    sprintf(ans,"openpos:%s:%s:%d:%d:%d", indice, way, nbc,stop,limit);
  }

  else if (strcmp(action,"smartpos") == 0 ) {
    sprintf(ans,"smartpos:%s:%s:%d", indice, way,limit);
  }
  

  else if (strcmp(action,"closepos") == 0 ) {
    sprintf(ans,"closepos:%s", indice);
  }

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


float __genericT(int tinf,int tsup,uint32_t t,farray* values,iarray* tstamps,float (*mathfct)(farray*) ) {

  farray res;
  farray_init(&res,10);

  if (__resolvT(tinf,tsup,t,values,tstamps,&res) != -1) {
  	farray_destroy(&res);
    return mathfct(&res);
  }  

  farray_destroy(&res);
  return -1;
}


 int __resolvT(int tinf,int tsup,uint32_t t,farray* values,iarray* tstamps,farray* result) {

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
    return -1;
  }

  for (i=indice_inf;i<=indice_sup;i++) {
     farray_push(result,values->values[i]);     
  }
  
  return 0;
 }


farray* __sampleT(int tinf,int tsup,uint32_t t,farray* values,iarray* tstamps) {

  farray* result = (farray*) malloc(1*sizeof(farray));
  farray_init(result,10);

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

  for (i=indice_inf;i<=indice_sup;i++) {
     farray_push(result,values->values[i]);     
  }
  
  return result;
 }


 int __counter(store* s,char* cname,int wait) {

  if (store_exists(s,cname) != ERR ) {
    int i = store_get(s,cname);
    i++;

    if (i == wait) {
      i=1;
      store_push(s,cname,i);
      return 1;
    }

    else {
      store_push(s,cname,i);
      return 0;
    }
  }

  else {
    int i = 1;
    store_push(s,cname,i);
    return 0;
  }

}





//#### CONVENIENCY FUNCTIONS FOR PLOTTING


int fstack_init(farray_stack* fs,int size) {

  fs->stack = (farray**) malloc(size * sizeof(farray*) );
  fs->size = 0;
  fs->msize = size;

}

int fstack_add(farray_stack* fs,farray* f) {

  if (fs->size == fs->msize ) {
    fs->stack = (farray**) realloc(fs->stack,(fs->msize+10) * sizeof(farray*) );
    fs->msize+=10;
  }

  fs->stack[fs->size] = f;
  fs->size++;

}


int fstack_destroy(farray_stack* fs) {
  free(fs->stack);
  fs->size=0;
  fs->msize=0;
}


int __fplot(farray_stack* allvalues,int tinf,int tsup,uint32_t t) {

  iarray xval;
  iarray_init(&xval,10);
  int i,j;
  char line[4096];
  char buff[256];

  if (tinf < 0) {
    tinf = t + tinf + 7200;
  }   

  if (tsup <= 0) {
    tsup = t + tsup + 7200;
  }

  for (i=tinf;i<=tsup;i++) {
    iarray_push(&xval,i);
  }

  FILE* gfh = fopen("adam.gnuplot","w");
  FILE* dfh = fopen("data","w");


  fputs("set term dumb 100,30\n \
set xdata time\n \
set timefmt \"%s\"\n",gfh);

  if (tsup - tinf < 60) {
    fputs("set format x \"%H:%M:%S\"\n",gfh);
  }

  else if (tsup - tinf < 3600 ) {
    fputs("set format x \"%H:%M\"\n",gfh); 
  }
   
  else {
    fputs("set format x \"%H\"\n",gfh); 
  }

  fputs("plot 'data' using 1:2 with lines\n",gfh);


  for(i=3;i<=allvalues->size+1;i++) {
    sprintf(line,"replot 'data' using 1:%d with lines\n",i);
    fputs(line,gfh);	
  }

  for(i=0;i<xval.size;i++) {
    sprintf(line,"%d ",xval.values[i] );
    for (j=0;j<allvalues->size;j++) {
      if (i < allvalues->stack[j]->size  ) {
        sprintf(buff,"%f ",allvalues->stack[j]->values[i]);
        strcat(line,buff);
      }
    }
    strcat(line,"\n");
    fputs(line,dfh);
  }

  fclose(dfh);
  fclose(gfh);

  system("gnuplot adam.gnuplot");
}

int ___log (const char* log_input, char* log_s) {
  if (strlen(log_input) > 1024 ) return -1;
  strncpy(log_s,log_input,sizeof(char)* 1024);
  return 0;
}

