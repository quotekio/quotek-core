#include "adammodule.h"
#include <curl/curl.h>
#include "deepthroat.h"
#include "utils.h"
#include <fstream>
#include <vector>



#define DEEPTHROAT_VERSION 0.1


size_t curl_wh(void *ptr, size_t size, size_t nmemb, string stream)
{

    if (ptr != NULL) {

      string temp(static_cast<const char*>(ptr), size * nmemb);
      stream += temp;
    }

    return size*nmemb;
}


const string deepthroat::cfgpath = "/etc/adam/deepthroat.conf";



information::information() {

}

information::information(string w,string d,string wh ) {

  who = w;
  does = d;
  what = wh;

}




datasource::datasource(string t,string loc,int hist) {
  type = t;
  location = loc;
  history = hist;
}


vector<string> datasource::cleanData(vector<string> fetched_data) {

  for (int i=0;i<fetched_data.size();i++) {
    fetched_data[i] = stripHTML(fetched_data[i]);
  }

  return fetched_data;
}


vector<string> datasource::parseRSS(xmlNode* root) {

  vector<string> result;
  int nb_news = 0;

  root = root->children;
  xmlNodePtr cur_node = root->children;
  xmlNodePtr item_node = NULL;


  while(cur_node != NULL) {
  

    if ( xmlStrcmp(cur_node->name, (const xmlChar *)"item" ) == 0   ) {
        item_node = cur_node->children;
        while( item_node != NULL ) {

          if ( xmlStrcmp(item_node->name, (const xmlChar *)"description" ) == 0   ) {
            result.push_back( std::string( (char*) xmlNodeGetContent(item_node) ) );
            nb_news ++;
            if (nb_news >= history) return result;

          }
          item_node = item_node->next;

        }
    }
  
    cur_node = cur_node->next;

  }
  return result;

}


vector<string> datasource::fetch() {

  vector<string> result;

  string temp = "";

  if (type == "rss") {

    CURL* ch;
    xmlDocPtr doc;
    xmlNode *root = NULL;

    ch = curl_easy_init();
    curl_easy_setopt(ch,CURLOPT_URL,location.c_str());
    curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_wh);
    curl_easy_setopt(ch,CURLOPT_WRITEDATA,&temp);
    curl_easy_perform(ch);
    curl_easy_cleanup(ch);

    if (temp != "") {
      
        doc = xmlReadMemory(temp.c_str(),temp.size(), "noname.xml", NULL, 0);
        if (doc == NULL) {
          cerr << "Unable to parse rss feed" << endl;
        }
      
       else {
       
          root = xmlDocGetRootElement(doc);
          result =  cleanData(parseRSS(root));

          xmlFreeDoc(doc);

       }
    }
  }

  else if (type == "twitter") {
   



  }

  return result;

} 

string datasource::getLocation() {
  return location;
}




deepthroat::deepthroat() {

}

int deepthroat::readConf() {

  string line;
  vector<string> sline;
  string param;
  string arg1,arg2,arg3;

  ifstream dfh (deepthroat::cfgpath.c_str());
  if (!dfh.is_open() ) return 1;

  while(dfh.good()) {

    getline(dfh,line);

	sline = split(line,' ');

	if (sline.size() == 4) {
	  param = trim(sline[0]);
	  arg1 = trim(sline[1]);
	  arg2 = trim(sline[2]);
	  arg3 = trim(sline[3]);

	  if (param == "data_source") {
          sources.push_back( new datasource(arg1,arg2,atoi(arg3.c_str()) ));
      }
    }
  }

  return 0;

}

int deepthroat::fetchAll() {

  vector<string> data;


  for(int i=0;i<sources.size();i++) {
     data = sources[i]->fetch();

     for(int j=0;j<data.size();j++) {
      //cout << "DESCR:" << data[j] << endl;
      summerize(data[j]);

     }
     //cout << data.size() << endl;


  }

  return 0;

}


string deepthroat::findWho(string news_text,AssocArray<int> words_occ) {

  vector<string> verbs_list;
  verbs_list.push_back("is");
  verbs_list.push_back("has");
  verbs_list.push_back("does");
  verbs_list.push_back("did");
  verbs_list.push_back("was");
  verbs_list.push_back("were");
  verbs_list.push_back("are");
  verbs_list.push_back("can");
  verbs_list.push_back("doesn't");
  verbs_list.push_back("didn't");
  verbs_list.push_back("see");
  verbs_list.push_back("sees");


  vector<string> words_of_interest;

  vector<string> bl_subjects;
  bl_subjects.push_back("a");
  bl_subjects.push_back("he");
  bl_subjects.push_back("it");
  bl_subjects.push_back("who");
  bl_subjects.push_back("which");
  bl_subjects.push_back("to");
  bl_subjects.push_back("that");
  bl_subjects.push_back("and");
  bl_subjects.push_back("on");
  bl_subjects.push_back("this");
  bl_subjects.push_back("in");
  bl_subjects.push_back("the");
  bl_subjects.push_back("is");
  bl_subjects.push_back("at");






  string result;
  int max_occ = 0;

  for (int i =0; i< words_occ.Size();i++ ) {
    for (int j=0;j<verbs_list.size();j++) {
      string toSearch = words_occ.GetItemName(i) + " " + verbs_list[j];

      if ( news_text.find(toSearch) != string::npos && ! contains(bl_subjects,words_occ.GetItemName(i)) ) {
        words_of_interest.push_back(words_occ.GetItemName(i));
      }
    }
  }

  for (int i=0;i<words_of_interest.size();i++) {

    if ( words_occ[words_of_interest[i]] > max_occ   ) {
      result = words_of_interest[i];
      max_occ = words_occ[words_of_interest[i]];
    }

  }

  return result;

}


information* deepthroat::summerize(string news_text) {

  information* result = new information();

  AssocArray<int> words_occ;

  //performs frequency analysis on news words
  news_text = strtolower(news_text);

  vector<string> truncated_newstext = split(news_text,(char) 0x20);

  //1st pass for cleaning
  for(int i=0;i<truncated_newstext.size();i++) { 

    truncated_newstext[i] = sreplace(truncated_newstext[i],".","");
    truncated_newstext[i] = sreplace(truncated_newstext[i],")","");
    truncated_newstext[i] = sreplace(truncated_newstext[i],"(","");
    truncated_newstext[i] = sreplace(truncated_newstext[i],"'s","");
    truncated_newstext[i] = sreplace(truncated_newstext[i],",","");

  }
  

  //2nd pass for counting
  for(int i=0;i<truncated_newstext.size();i++) {    

    if (truncated_newstext[i] != "" ) {
      if (  words_occ.IsItem(truncated_newstext[i])  ) words_occ[truncated_newstext[i]] ++;
      else words_occ[truncated_newstext[i]] = 1;
    }
  }

  cout << "Subject is:" << findWho(news_text,words_occ) << endl;
  

  return result;

}









void* module_fct(module_io mio) {

  int dlock = 0;

  cout <<  "Deepthroat Financial News Analyzer v" << DEEPTHROAT_VERSION << endl;

  
  deepthroat* d = new deepthroat();
  d->readConf();

  while(1) {

    if (dlock == 0) {
      dlock = 1;
      d->fetchAll();

    }
    

    sleep(120);
  }
}




extern "C" void* module(module_io mio) {
  module_fct(mio);
  return NULL;

}
