#ifndef INDICE_H
#define INDICE_H

#include <string>
#include <vector>
#include "assoc.h"

using namespace std;

class indice {

  public:

    string name;
    int pnl_pp;
    int min_stop;
    string bmapping;
  	string unit;
    string start_hour;
    string end_hour;
    indice(string, int,int, string, string,string,string);

};

indice* iResolve(AssocArray<indice*>,string s);
indice* iResolveByName(AssocArray<indice*>,string s);
indice* iResolveByEpic(AssocArray<indice*>,string s);

vector<string> iGetNames(AssocArray<indice*> indices);
vector<string> iGetEpics(AssocArray<indice*> indices);


#endif