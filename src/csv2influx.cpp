#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include "lib/assoc.h"
#include "lib/utils.h"
#include "lib/backends/influxdb.cpp"


using namespace std;

AssocArray<int> findOffsets(vector<string> header) {

  AssocArray<int> offs;

  regex cac ("France40(.*)Sell");
  regex ita ("Italie40(.*)Sell");
  regex jap ("Japon225(.*)Sell");
  regex dow ("WallStreet(.*)Sell");
  regex dax ("Allemagne30(.*)Sell");
  regex gold ("Oraucomptant(.*)Sell");
 
  for(int i=0;i<header.size();i++) {

    if (  regex_match(header[i],cac) ) {
      offs["CAC_MINI"] = i;
    }

    if (  regex_match(header[i],jap) ) {
      offs["NIKKEI_MINI"] = i;
    }

    if (  regex_match(header[i],ita) ) {
      offs["ITALY40_MINI"] = i;
    }

    else if ( regex_match(header[i],dax) ) {
      offs["DAX_MINI"] = i;
    }
    
    else if ( regex_match(header[i],dow) ) {
      offs["DOW_MINI"] = i;
    }

    else if ( regex_match(header[i],gold) ) {
      offs["GOLD_MINI"] = i;
    }

  }

  return offs;
 
}


int main(int argc,char** argv) {


  AssocArray<int> offsets;
  AssocArray<records*> reclist;

  reclist["CAC_MINI"] = (records*) malloc(sizeof(records));
  reclist["NIKKEI_MINI"] = (records*) malloc(sizeof(records));
  reclist["ITALY40_MINI"] = (records*) malloc(sizeof(records));
  reclist["DAX_MINI"] = (records*) malloc(sizeof(records));
  reclist["DOW_MINI"] = (records*) malloc(sizeof(records));
  reclist["GOLD_MINI"] = (records*) malloc(sizeof(records));

  records_init(reclist["CAC_MINI"],150);
  records_init(reclist["NIKKEI_MINI"],150);
  records_init(reclist["ITALY40_MINI"],150);
  records_init(reclist["DAX_MINI"],150);
  records_init(reclist["DOW_MINI"],150);
  records_init(reclist["GOLD_MINI"],150);

  ifstream ifh  (argv[1]);
  influxdb* back = new influxdb();
  back->init(argv[2]);

  string line;

  if ( ! ifh.is_open()) {
    cerr << "Cannot open file" << endl;
    exit(1) ;    
  }

  getline(ifh,line);
  vector<string> fmap = split(line,';');

  offsets = findOffsets(fmap);
  
  while(ifh.good()) {

    cout << "." ;

    getline(ifh,line);

    vector<string> fields = split(line,';');

    if (fields.size() > 3 ) {

      string cdate = fields[1] + " " + fields[2];
      record rec;

      rec.timestamp = str2time2(cdate);

      for(int i=0;i<offsets.Size();i++ ) {
        int offset = offsets[i];
        float price_sell = atof(fields[offset].c_str());
        float price_buy = atof(fields[offset+1].c_str());
        float price =  (price_sell + price_buy) / (float) 2;

        rec.value = price;
        rec.spread = (price_buy - price_sell) / (float) 2;
        records_push(reclist[offsets.GetItemName(i)],rec);
      }
    }
  

    for (int j=0;j< reclist.Size();j++ ) {
      if (reclist[j]->size == 100) {
        back->store(reclist.GetItemName(j), reclist[j]); 
        records_clear(reclist[j]);
        records_init(reclist[j],150);
      }
    }
  }

  ifh.close();


}