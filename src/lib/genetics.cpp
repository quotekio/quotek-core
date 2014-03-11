#include "genetics.h"


genetics::genetics(int p,int s,int c,int n,int ma,float ct,int mg,int rwin) {
  population_size = p;
  survivors = s;
  children = c;
  newcomers = n;
  mutable_attributes = ma;
  converge_thold = ct;
  max_generations = mg;
  recompute_winners = rwin;
  generation = 0;

  ddir = std::string(GDUMP_DIR) + "/" +  int2string(time(NULL)) + "/" ;
  mkdir(ddir.c_str(),S_IRWXU);
}


int genetics::getMaxGenerations() {
  return max_generations;
}

float genetics::getConvergeThold() {
  return converge_thold;
}

int genetics::getPopulationSize() {
  return population.size();
}

bool genetics::converges() {

  for (int i=0;i< population.size();i++) {
    if (  population[i].result >= converge_thold  ) return true;
  }

  return false;

}

bool genetics::mustCompute(individual* iv) {
  if (recompute_winners == 0 && iv->result != 0) return false;
  else return true;
}

individual* genetics::getIndividualFromPopulation(int inum) {
  return &( population[inum] );
}


void genetics::parseGene(string gene_str) {

  vector<string> args = split(gene_str,0x20);
  gene g1; 
  int parse_err = 0;

  if (args.size() >= 3) {
    g1.name = args[1];  
    g1.dataType = args[2];

    if (args[2] == "int" || args[2] == "float") {
      if (args.size() >= 5) {
        g1.ilimit_inf = atoi( args[3].c_str() ); 
        g1.ilimit_sup = atoi( args[4].c_str() );
      } 
      else parse_err = 1;
    }

    if (parse_err) {
      cerr << "* UNABLE TO LOAD GENE " << gene_str << " : PARSING ERROR *" << endl;
      return;
    }

    genes.push_back(g1);

  }
}


void genetics::savePopulation() {

}


void genetics::initPopulation() {
  genPopulation(-1);
}


void genetics::genPopulation(int size) {

  if (size == -1) size = population_size;

  for (int i=0;i<size;i++) {

    individual iv;
    iv.result=0;

    store_init(&(iv.attributes),genes.size());
    char gname[512];

    for(int j=0;j< genes.size();j++) {

      strncpy(gname,genes[j].name.c_str(),512*sizeof(char));

      if ( genes[j].dataType == "int" ) {
        int val = genes[j].ilimit_inf + rand() % genes[j].ilimit_sup ;
        store_push( &(iv.attributes), gname, val);
      }

      else if (genes[j].dataType == "float") {
        float val =  genes[j].limit_inf + (float) rand() / ((float)RAND_MAX/(genes[j].limit_sup - genes[j].limit_inf ));
        store_push( &(iv.attributes), gname, val);
      }
    }
    population.push_back(iv);

  }
}


void genetics::newgen() {

  vector <individual> newpop;

  cout << "1ST INDIVIDUAL RESULT (PPRE SELECT): " << population[0].result << endl; 

  select(&newpop);

  cout << "1ST INDIVIDUAL RESULT (POST SELECT): " << newpop[0].result << endl; 

  reproduce(&newpop);
  cout << "1ST INDIVIDUAL RESULT (POST REPRODUCE): " << newpop[0].result << endl; 

  mutate(&newpop);
  cout << "1ST INDIVIDUAL RESULT (POST MUTATE): " << newpop[0].result << endl; 
  
  population = newpop;

  cout << "1ST INDIVIDUAL RESULT (POST REASSIGN): " << population[0].result << endl;

  genPopulation(newcomers);

  cout << "1ST INDIVIDUAL RESULT (POST NEWCOMERS): " << population[0].result << endl;  

  generation++;

}



//Probleme ICI
int genetics::getCurMaxResult() {

  float max = -1000000;
  int max_idx = -1;

  for (int i=0;i<population.size();i++) {
    if ( population.at(i).result >= max && ! vector_in(&survivors_indices,i) ) {
      max = population.at(i).result;
      max_idx = i;
    }
  }

  survivors_indices.push_back(max_idx);
  return max_idx;
}


void genetics::select(vector<individual>* p) {

  survivors_indices.clear();

  for(int i=0;i<survivors;i++) {
    int s_idx = getCurMaxResult();
    p->push_back( population[s_idx]);
    cout << "SELECTED INDIVIDUAL RESULT:" << population[s_idx].result << endl;
  }
}


void genetics::reproduce(vector<individual>* p) {


  for (int i=0;i<children;i++) {

    individual iv;
    iv.result = 0;
    individual* giver;
    store_init(&(iv.attributes),genes.size());

    //randomly pick 2 individuals inside survivors group for mating
    int p1_idx = rand() % survivors;
    int p2_idx = rand() % survivors;

    /* makes sure the 2 selected individuals are differents 
    (sorry little guys, fapping is forbidden here ! ) */
    while(p2_idx == p1_idx) {
      p2_idx = rand() % survivors;
    }

    individual p1 = p->at(p1_idx);
    individual p2 = p->at(p2_idx);
    

    //now decide who gives what
    for (int j=0;j<genes.size();j++) {
 
      if ( rand() % 2 == 0) giver = &p1;
      else giver = &p2;

      //mmhh, pr0n !
      store_push( &(iv.attributes), 
        	      store_item_at(&(giver->attributes), j),
        	      store_value_at(&(giver->attributes), j) );

    }      
   p->push_back(iv);
  }
}



gene* genetics::getGene(string gname) {
  for (int i=0;i<genes.size();i++) {
    if ( genes[i].name == gname ) {
      return &(genes[i]);
    }
  }

  return NULL;

}

void genetics::mutateGene(individual* iv,gene* g) {

  gene* gene_ref = g;

  char gname[512];


  if (gene_ref != NULL) {

  	strncpy(gname,gene_ref->name.c_str(), 512*sizeof(char) );

    if (gene_ref->dataType == "int") {
  	  int val = gene_ref->ilimit_inf + rand() % gene_ref->ilimit_sup ;
  	  store_push ( &(iv->attributes) , gname , val );
    }  

    else if (gene_ref->dataType == "float") {
      float val =  gene_ref->limit_inf + (float) rand() / ((float)RAND_MAX/(gene_ref->limit_sup - gene_ref->limit_inf ));
      store_push ( &(iv->attributes) , gname , (uint32_t) val );
    }
  }
}


void genetics::mutate(vector<individual>* p) {

  if (mutable_attributes == 0) return; 
  if (mutable_attributes > genes.size() ) mutable_attributes = genes.size();

  for (int i=survivors;i<p->size();i++) {

    vector<int> muted_idx;

    for (int j = 0;j<mutable_attributes;j++) {

      //pick which gene to mutate
      int mut_r = rand() % genes.size();
      if (! vector_in( &(muted_idx), mut_r ) ) {
        mutateGene( &(p->at(i)), &(genes[mut_r]) );
        muted_idx.push_back( mut_r );     
      }

    }

  }

}

individual* genetics::getWinner() {

  //dirty, do not work in any case
  float max_result = -1000000;
  int w_idx = 0;

  for (int i=0;i<population.size();i++) {
    if (population[i].result >= max_result ) {
      max_result = population[i].result;
      w_idx = i;
    }
  }
  return &( population[w_idx] );  
}


string genetics::serializeIV(individual* iv) {

  string result = "";
  gene* linked_gene;
  for(int i=0;i<iv->attributes.size;i++) {
    char* gname = store_item_at( &(iv->attributes), i);
    uint32_t gval = store_value_at( &(iv->attributes),i);
    linked_gene = getGene( std::string(gname) );

    if ( linked_gene->dataType == "int"  ) {
      result += linked_gene->name + " int " + int2string(gval) + "\n";
    }

    else if (linked_gene->dataType == "float") {
      result += linked_gene->name + " float " + float2string((float)gval) + "\n"; 
    }
  }

  return result;
}


int genetics::dumpWinner() {

  string fpath = ddir + "winner.agf";
  ofstream of (fpath.c_str());
  if ( ! of.good() ) { 
    cerr << "* CRITICAL: Cannot dump genetics winner, file error *" << endl;
    return 0;
  }

  individual* winner = getWinner();

  of << "INDIVIDUAL 0" << endl;
  of << "RESULT " << winner->result << endl;
  of << serializeIV(winner);
  of.close();

  return 1;
}

int genetics::dumpPopulation() {

  
  string fpath = ddir + "generation_" + int2string(generation) + ".agf";
  ofstream of (fpath.c_str());
  if ( ! of.good() ) { 
    cerr << "* CRITICAL: Cannot dump genetics population, file error *" << endl;
    return 0;
  }
  
  for (int i=0;i<population.size();i++) {
    individual* iv = &(population[i]);
    of << "INDIVIDUAL " << i << endl;
    of << "RESULT " << iv->result << endl;
    of << serializeIV(iv);
    of << endl;
  }

  of.close();
  return 1;
}
