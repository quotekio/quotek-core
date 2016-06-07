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


void genetics::parseGene(std::vector<std::string> &gargs) {

  gene g1; 
  int parse_err = 0;

  if (gargs[0] == "randint" && gargs.size() == 4) {
    g1.dataType = gargs[0];
    g1.name = gargs[1];
    g1.ilimit_inf = std::stoi(gargs[2]);
    g1.ilimit_sup = std::stoi(gargs[3]);

  }

  else if (gargs[0] == "randfloat" && gargs.size() == 4) {
    g1.dataType = gargs[0];
    g1.name = gargs[1];
    g1.flimit_inf = std::stof(gargs[2]);
    g1.flimit_sup = std::stof(gargs[3]);
  }

  else parse_err = 1;

  if (parse_err) {
    cerr << "* UNABLE TO LOAD GENE " << gargs[1] << " : PARSING ERROR *" << endl;
    return;
  }
  

  this->genes.emplace_back(g1);

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

    for(int j=0;j< this->genes.size();j++) {
      
      if ( genes[j].dataType == "randint" ) {
        int f = randint(genes[j].ilimit_inf,genes[j].ilimit_sup);
        iv.attributes[genes[j].name] = f;
      }

      else if (genes[j].dataType == "randfloat") {
        iv.attributes[genes[j].name] = randfloat(genes[j].flimit_inf,genes[j].flimit_sup);
      }
    }
    population.push_back(iv);

  }

}


void genetics::newgen() {

  vector <individual> newpop;
  select(&newpop);

  reproduce(&newpop);
  mutate(&newpop);
  
  population = newpop;
  genPopulation(  population_size - newpop.size() );
  generation++;

}

void genetics::select(vector<individual>* p) {

  survivors_indices.clear();

  //here we select <genetics_survivors> from the best
  //individuals.
  for(int i=0;i<survivors;i++) {
    int s_idx = getCurMaxResult();
    p->push_back( population[s_idx]);
  }

  std::cout << "Remaining Individuals:";
  for (auto suri_: survivors_indices) {
    std::cout << suri_ << "," ;
  }
  std::cout << std::endl;

}


void genetics::reproduce(vector<individual>* p) {


  for (int i=0;i<children;i++) {

    individual iv;
    iv.result = 0;
    individual* giver;
    //store_init(&(iv.attributes));

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
      iv.attributes[j] = giver->attributes[j];


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

  if (gene_ref != NULL) {

    if (gene_ref->dataType == "randint") {
  	  iv->attributes[gene_ref->name] = randint(gene_ref->ilimit_inf,gene_ref->ilimit_sup);
    }  

    else if (gene_ref->dataType == "randfloat") {
     iv->attributes[gene_ref->name] = randfloat(gene_ref->flimit_inf,gene_ref->flimit_sup);
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


vector <string> genetics::serializeIV(individual* iv) {

  vector <string> result;

  gene* linked_gene;
  for(int i=0;i<iv->attributes.Size();i++) {
   
    stringstream ss;

    linked_gene = getGene( iv->attributes.GetItemName(i) );

    if ( linked_gene->dataType == "randint"  ) {
      int f = iv->attributes[i];
      ss << linked_gene->name << "=" << f;
    }

    else if (linked_gene->dataType == "randfloat") {
      float f = iv->attributes[i];
      ss << linked_gene->name << "=" << f;
    }
    result.emplace_back(ss.str());
  }

  return result;
}

