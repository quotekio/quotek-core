#include "records.h"

void records_init(records* recs,int msize) {

  recs->data = (record*) malloc( sizeof(record) * msize );
  recs->size = 0;
  recs->msize = msize;

}

void records_clear(records* recs) {

  free(recs->data);
  recs->size=0;
  recs->msize=0;
  

}


void records_push(records* recs, record r) {

  if (!recs->data) {
    records_init(recs,100);
  }

  int pos = recs->size;
  if (pos == recs->msize - 1) {
    recs->data = (record*) realloc(recs->data, sizeof(record) * 2 * recs->msize);
    recs->msize = 2 * recs->msize;
  }

  recs->data[pos] = r;
  recs->size++;
}


record* records_last(records* recs) {
  if (recs->size > 0) return &(recs->data[recs->size-1]);
  else return NULL;
}

records* records_sample(records* recs, 
	                    const char* search_method, 
	                    uint32_t timestamp, 
	                    int tinf, 
	                    int tsup) {

  records* result = NULL;

  if (tinf < 0) tinf = timestamp + tinf;
  if (tsup <= 0) tsup = timestamp + tsup;
 
  if (strcmp(search_method,"dicho") == 0) {
     int inf_index = records_search_dicho(recs,tinf);
     int sup_index = records_search_dicho(recs,tsup);

     if ( inf_index == -1 || sup_index == -1 ) return result;

     result = (records*) malloc(sizeof(records)); 
     result->data = recs->data + inf_index;
     result->size = sup_index - inf_index; 
  }

  return result;

}

int records_search_dicho(records* recs ,uint32_t timestamp) {

  int first = 0;
  int last = recs->size -1;
  int middle = (first+last)/2;

  while( first <= last )
   {
      if ( recs->data[middle].timestamp < timestamp )
         first = middle + 1;    
      else if ( recs->data[middle].timestamp == timestamp ) 
      {
        return middle ;
      }
      else last = middle - 1;
      middle = (first + last)/2;
   }
  return -1;

}

/* Legacy functions to translate to old datastruct */
farray* records_values(records* recs) {

farray* values;
  int i;
  for (i=0;i<recs->size;i++)  {
    farray_push(values,recs->data[i].value);
  }

  return values;

}

iarray* records_timestamps(records* recs) {

  iarray* timestamps;
  int i;
  for (i=0;i<recs->size;i++)  {
    iarray_push(timestamps,recs->data[i].timestamp);
  }

  return timestamps;

}