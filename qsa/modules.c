#include "modules.h"

eval_module_io* resolveIO( struct evmio_array* evmio_a, char* name) {

  int i;

  for (i=0;i<evmio_a->size;i++) {
    if (  strcmp(evmio_a->evmio[i].mname,name) == 0  ) {
      return &(evmio_a->evmio[i]);
    }
  }

  return NULL;

}

int __module_input(struct evmio_array* evmio_a, char* name,char* input) {
  eval_module_io* cio = resolveIO(evmio_a,name);
  if (cio != NULL) {

    if (cio->input != NULL) {
      Enqueue(input,*(cio->input) );
    }
  }
}