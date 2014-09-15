#include "../../src/lib/records.h"

#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main(int argc, char** argv) {
 
  records recs;
  record r; 
  records_init(&recs,100);

  int i =0;

  uint32_t t = time(0);

  for (i=0;i<10;i++)  {
    r.value = i;
    r.spread = i;
    r.timestamp = t + i;
    records_push(&recs,r);
    sleep(1);
    
  }

  t = time(0);
  records* recs2 = records_sample(&recs,"dicho",t,-6,-5);
  cout << "RECS2_SIZE:" << recs2->size << endl;
  record* r2 = records_last(recs2);
  cout << "RECS2_LAST_ELEMENT:" << r2->value << endl;
  return 0;

}