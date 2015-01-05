#include "store.h"

void* store_push(store* s,char* name, void* data)  {

  int exists = store_exists(s,name);

  if (exists == ERR) {
    s->data[s->size] = data;
    strncpy(s->name[s->size],name,MAX_STORENAME_LEN * sizeof(char));
    s->size++;
  }

  else {
    s->data[exists] = data;
  }
}


int store_exists(store* s,char* name) {

  int i;
  for (i=0;i<s->size;i++) {
    if (strcmp(s->name[i],name) == 0 ) {
      return i;
    } 
  }
  return ERR;
}


void* store_get(store* s,char* name) {

  int i;
  for (i=0;i<s->size;i++) {
    if (strcmp(s->name[i],name) == 0 ) {
      return s->data[i];
    }
  }
  return NULL;
}


void store_clean(store* s, char* name) {

  int i;
  for (i=0;i<s->size;i++) {
    if (strcmp(s->name[i],name) == 0 ) {
      int j = 0;
      for (j=0;j< MAX_STORENAME_LEN; j++) s->name[i][j] = 0x00;
      s->data[i] = 0;
    }
  }

}

void* store_init(store* s) {

  s->size = 0;
  s->msize = 128;

}

