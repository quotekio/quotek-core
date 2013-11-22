#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include "lib/assoc.h"
#include "lib/utils.h"

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


  AssocArray< vector<string> > mapping;
  AssocArray<int> offsets;

  mapping["CAC_MINI"].push_back("");
  mapping["NIKKEI_MINI"].push_back("");
  mapping["ITALY40_MINI"].push_back("");
  mapping["DAX_MINI"].push_back("");
  mapping["DOW_MINI"].push_back("");
  mapping["GOLD_MINI"].push_back("");


  ifstream ifh  (argv[1]);
  ofstream ofh (argv[2]);

  string line;

  if ( ! ifh.is_open() ||  ! ofh.is_open()) {
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

      ofh << "T:" << str2time2(cdate) << endl;

      for(int i=0;i<offsets.Size();i++ ) {
        int offset = offsets[i];
        float price_sell = atof(fields[offset].c_str());
        float price_buy = atof(fields[offset+1].c_str());
        float price =  (price_sell + price_buy) / (float) 2;
        ofh << offsets.GetItemName(i) << ":" << price << endl;

      }
    }
  }

  ofh.close();


}