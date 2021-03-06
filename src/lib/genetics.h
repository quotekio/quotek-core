#ifndef GENETICS_H
#define GENETICS_H

#include "store.h"
#include "narrays.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "utils.h"
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

#include "assoc.h"
#include <quotek/any.hpp>


#define GDUMP_DIR "/tmp/qate/genetics"

using namespace std;

typedef struct gene {
    std::string name;
    std::string dataType;
    float flimit_inf;
    float flimit_sup;
    int ilimit_inf;
    int ilimit_sup;
} gene;

typedef struct gene_constraint {
  std::string gene1;
  std::string operator_;
  std::string gene2;
} gene_constraint;


typedef struct individual {

  AssocArray<quotek::data::any> attributes;

  float result;
} individual;



class genetics {

  public:

  	genetics(int,int,int,int,int,float,int,int);

    int getMaxGenerations();
    float getConvergeThold();

  	void parseGene(std::vector<string>&);
    void appendConstraint(std::vector<string>& gconst);
    void newgen();
    void initPopulation();
  	void genPopulation(int);
    void select(vector<individual>*);
    gene* getGene(string);
    void mutateGene(individual*,gene*);

    bool checkConstraints(individual& iv);

  	void mutate(vector<individual>*);
  	void reproduce(vector<individual>*);
    void savePopulation();

    bool converges();
    bool mustCompute(individual*);
    
    std::vector <string> serializeIV(individual*);

    std::vector<individual> population;

    
    
 private:
  
   int getCurMaxResult();

   farray results;
   int population_size;
   int survivors;
   int children;
   int newcomers; 
   int generation;
   int mutable_attributes;
   float converge_thold;
   int max_generations;
   int recompute_winners;

   std::vector<gene> genes;
   std::vector<gene_constraint> gene_constraints;
   vector <int> survivors_indices;

   string ddir;

};

#endif
