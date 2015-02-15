#include "adammodule.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rapidjson/document.h"
#include "narrays.h"
#include "store.h"
#include "assoc.h"
#include "utils.h"

const string edta_eventfile = "/tmp/adam/edta.events.json";

void* module_fct(module_io mio) {

  ifstream fh;
  std::string json_content = "";
  std::string line;
  rapidjson::Document d;
  AssocArray<iarray*> timings; 

  cout << "Loading Event Driven Trading Algorithm Module v0.1.." << endl;
  
  //opens and reads json file
  fh.open(edta_eventfile.c_str());

  if (fh.is_open() ) {
    while(fh.good() ){
      getline(fh,line);
      json_content += line;
    }
  }  

  else {
    cout << "Cannot open EDTA File, leaving Module.." << endl;
    return NULL;
  }

  //parses json
  d.Parse<0>(json_content.c_str());

  if ( d.IsArray() ) {
    //first loop, initializes timings assoc
    for (int i=0;i<d.Size();i++) {
      std::string vname = d[i]["value"].GetString();
      timings[vname] = (iarray*) malloc(sizeof(iarray));
      iarray_init(timings[vname],10);
    }

    for (int i=0;i<d.Size();i++) { 
      std::string vname = d[i]["value"].GetString();
      iarray_push(timings[vname],d[i]["time"].GetInt()); 
    }

  }

  else {
    cout << "Invalid EDTA File, leaving Module.." << endl;
    return NULL;
  }

  //puts back parsed data inside store
  for (int i=0;i<timings.Size();i++) {
    char sname[128];
    strcpy(sname,timings.GetItemName(i).c_str());
    strcat(sname,".EDTA");

    if (mio.s == NULL) {
      cout << "EDTA Error: store pointer is NULL!" << endl;
      return NULL;
    }

    store_push(mio.s,sname,timings[i]);
  }

  return NULL;

}

extern "C" void* module(module_io mio) {
  module_fct(mio);
  return NULL;

}
