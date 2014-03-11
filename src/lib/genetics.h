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


#define GDUMP_DIR "/tmp/adam/genetics"

using namespace std;

typedef struct gene {
    string name;
    string dataType;
    float limit_inf;
    float limit_sup;
    int ilimit_inf;
    int ilimit_sup;
} gene;

typedef struct individual {
  store attributes;
  float result;
} individual;



class genetics {

  public:

  	genetics(int,int,int,int,int,float,int,int);

    int getMaxGenerations();
    float getConvergeThold();

  	void parseGene(string);
    void newgen();
    void initPopulation();
  	void genPopulation(int);
    void select(vector<individual>*);
    gene* getGene(string);
    void mutateGene(individual*,gene*);

  	void mutate(vector<individual>*);
  	void reproduce(vector<individual>*);
    void savePopulation();

    int getPopulationSize();
    individual* getIndividualFromPopulation(int);

    bool converges();
    bool mustCompute(individual*);


    individual* getWinner();
    

    string serializeIV(individual*);

    
    
 private:
  
   int getCurMaxResult();

   vector<individual> population;
   vector<gene> genes;
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

   vector <int> survivors_indices;

   string ddir;

};

#endif