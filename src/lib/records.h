#ifndef RECORDS_H
#define RECORDS_H

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "narrays.h"

typedef struct __record {
  uint32_t timestamp;
  float value;
  float spread;
} record;

typedef struct __records {
  record* data;
  int size;
  int msize;
} records;

void records_init(records*,int);
void records_clear(records*);
void records_push(records*,record);
record* records_last(records*);
records* records_sample(records*, const char*, uint32_t, int, int);
int records_search_dicho(records* ,uint32_t);


/* Legacy functions to translate to old datastruct */
farray* records_values(records*);
iarray* records_timestamps(records*);

#endif

