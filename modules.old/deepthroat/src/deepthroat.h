#include <vector>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "assoc.h"

using namespace std;

class datasource {

  public:

    datasource(string,string,int);
    vector<string> fetch();
    vector<string> parseRSS(xmlNode*);
    vector<string> cleanData(vector<string>);
    string getLocation();
    

    
  private:

  	string type;
  	string location;
  	int history;
    
};



class information {

  public:

  	information(string,string,string);
    information();
    string toString();

  private:

  	string who;
  	string does;
  	string what;
    int weight;

};



class deepthroat {


  public:

    deepthroat();
    string findWho(string news_text,AssocArray<int> words_occ);
    information* summerize(string);
    int fetchAll();
    int readConf();

    static const string cfgpath;


  private:
    vector<datasource*> sources;
    vector<information*> infos;

};


