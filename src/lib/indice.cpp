#include "indice.h"

indice::indice(string n, int p, int ms, string b, float pv,string sh,string eh) {
      name = n;
      pnl_pp = p;
      min_stop = ms;
      bmapping = b;
      pip_value = pv;
      start_hour = sh;
      end_hour = eh;
    };


indice* iResolve(AssocArray<indice*> indices,string s) {
  indice* i = iResolveByEpic(indices,s);
  if (i != NULL) return i;
  else return iResolveByName(indices,s);
}

indice* iResolveByEpic(AssocArray<indice*> indices,string s) {
  if ( indices.IsItem(s) ) return indices[s];
  return NULL;
}


indice* iResolveByName(AssocArray<indice*> indices,string s) {
  for (int i=0;i<indices.Size();i++) {
    if (indices[i]->name == s ) return indices[i];
  }
  return NULL;
}

vector<string> iGetNames(AssocArray<indice*> indices) {

  vector<string> res;

  for(int i=0;i<indices.Size();i++) {
     res.push_back( indices[i]->name );
  }
  return res;
}

vector<string> iGetEpics(AssocArray<indice*> indices) {

  vector<string> res;
  for(int i=0;i<indices.Size();i++) {
     res.push_back( indices.GetItemName(i) );
  }
  return res;
}