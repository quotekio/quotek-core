#include "store.h"

void* store_push(store* s,char* name, void* data)  {

  pthread_mutex_lock( &s->m_mutex );

  int exists = store_exists(s,name);

  if (exists == ERR) {
    s->data[s->size] = data;
    strncpy(s->name[s->size],name,MAX_STORENAME_LEN * sizeof(char));
    s->size++;
  }

  else {
    s->data[exists] = data;
  }

  pthread_mutex_unlock( &s->m_mutex );

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


void store_delete(store* s, char* name) {

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

void store_clear(store* s) {
  int i=0;
  for (i=0;i<s->size;i++) {
    strcpy(s->name[i],"\0");
    s->data[i] = 0x00;
  }
}

char* store_item_at(store* s,int index) {
  if (index < s->size ) return s->name[index];
  else return NULL;
}


void* store_value_at(store*s,int index) {

 if (index < s->size ) return s->data[index];
 else return 0;

}


