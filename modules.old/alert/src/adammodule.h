#include "assoc.h"
#include "narrays.h"
#include "queue_c.h"

typedef struct module_io {
  AssocArray<farray*>* values;
  float* cur_pnl;
  float* cumulative_pnl;
  Queue_c* input;
  Queue_c* output;
} module_io;

typedef struct eval_module_io {
  char mname[50];
  Queue_c* input;
  Queue_c* output;
} eval_module_io;
