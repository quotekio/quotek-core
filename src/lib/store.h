#ifndef STORE_HPP
#define STORE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>

#define MAX_STORENAME_LEN 128
#define ERR 0xffffffff

typedef struct store {

  char** name;
  uint32_t data[128];
  int size;
  int msize;
  pthread_mutex_t m_mutex;
  pthread_cond_t  m_condition;
  
} store;


void store_push(store* s, char* name,uint32_t data);
uint32_t store_get(store* s, char* name);
void store_init(store* s,int size);
int store_exists(store* s,char* name);

char* store_item_at(store* s,int index);
uint32_t store_value_at(store*s,int index);

void store_clear(store* s);

#endif
