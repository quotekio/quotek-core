#include <quotek/quotek.hpp>
#include <quotek/any.hpp>
#include <map>
#include <vector>
#include "../../src/lib/assoc.h"


int itersum(std::map<std::string, int> iters) {
  int sum_ = 0;
  for (auto i: iters) sum_ += i.second;
  return sum_;
}


int main(int argc, char** argv) {

  //construct a map of vectors
  AssocArray<std::vector<quotek::data::any> > universe;

  universe["foo"].emplace_back(0);
  universe["foo"].emplace_back(1);
  universe["foo"].emplace_back(2);
  universe["foo"].emplace_back(3);

  universe["bar"].emplace_back(0);
  universe["bar"].emplace_back(1);
  universe["bar"].emplace_back(2);
  universe["bar"].emplace_back(3);

  universe["kiki"].emplace_back(8);
  universe["kiki"].emplace_back(9);

  //universe["caca"].emplace_back("toto");
  //universe["caca"].emplace_back("titi");

 
  std::vector<int> indices;
  
  //compute how many iters;
  int total_iters = 0;
  for (int i=0; i< universe.Size();i++  ){

    indices.push_back(0);

    if (total_iters == 0) total_iters = universe[i].size();
    else total_iters *= universe[i].size();
  }

  for (int i=0;i< total_iters;i++) {

    std::cout << "(";

    for (int k = 0; k < indices.size(); k++) {

      int foo =  universe[k][indices[k]];
      
      std::cout << foo << ",";


    }
    std::cout << ")" << std::endl;


    indices[ indices.size() -1 ] ++;

    for ( int j= indices.size() -1; j > 0; j--  ) {

      if (  indices[j] == universe[j].size() ) {
        indices[j] = 0;
        indices[j-1] ++;
      }

    }

  }

  return 0;

}