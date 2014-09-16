#include "../../src/lib/records.h"
#include "../../src/lib/backends/influxdb.cpp"

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

  influxdb* idb = new influxdb();
  idb->init("127.0.0.1",8086, "root","root","adam");
  idb->store("CAC_MINI",&recs);

  records* recs2 = idb->query("SELECT value,spread FROM CAC_MINI order asc");
  cout << "RECS2_SIZE:" << recs2->size << endl;

  records* recs3 = idb->query("CAC_MINI",  recs2->data[0].timestamp , recs2->data[9].timestamp );
  cout << "RECS3_SIZE:" << recs3->size << endl;

  


  




  return 0;

}