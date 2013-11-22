#include "broker.h"
#include <curl/curl.h>
#include <stdlib.h>

size_t curl_wh(void *ptr, size_t size, size_t nmemb, string stream)
{
    if (ptr != NULL) {

      string temp(static_cast<const char*>(ptr), size * nmemb);
      stream += temp;
    }
    return size*nmemb;
}

class igmGw : public broker {
public:

  
    virtual int connect() {
      cout << "CONNECT" << endl;
      return 0;
    }

    virtual string getValues() {
   
      string temp = "";
      CURL* ch = curl_easy_init();
      curl_easy_setopt(ch,CURLOPT_URL,"http://127.0.0.1:8090/get/values");
      curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_wh);
      curl_easy_setopt(ch,CURLOPT_WRITEDATA,&temp);
      curl_easy_perform(ch);
      curl_easy_cleanup(ch);

     
      return temp;
    }

    virtual string getPositions() {
    
      string temp = "";
      CURL* ch = curl_easy_init();
      curl_easy_setopt(ch,CURLOPT_URL,"http://127.0.0.1:8091/get/positions");
      curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_wh);
      curl_easy_setopt(ch,CURLOPT_WRITEDATA,&temp);
      curl_easy_perform(ch);
      curl_easy_cleanup(ch);
      return temp;
    }
    

  virtual string openPos(string epic,string way,int nbc,int stop,int limit) {


    string temp = "";

    char* post = (char*) malloc(512);
    sprintf(post,"epic=%s&way=%s&nbc=%d&stop=%d&limit=%d",epic.c_str(),way.c_str(),nbc,stop,limit);

    CURL* ch = curl_easy_init();
    curl_easy_setopt(ch,CURLOPT_URL,"http://127.0.0.1:8092/action/openpos");
    curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_wh);
    curl_easy_setopt(ch,CURLOPT_POST,1);
    curl_easy_setopt(ch,CURLOPT_POSTFIELDS,post);
    curl_easy_setopt(ch,CURLOPT_WRITEDATA,&temp);
    curl_easy_perform(ch);
    curl_easy_cleanup(ch);

    free(post);

    return temp;

  }


  virtual string closePos(string dealid) {
  
    string temp = "";
    char*  post = (char*) malloc(512);

    sprintf(post,"dealid=%s",dealid.c_str());
    CURL* ch = curl_easy_init();
    curl_easy_setopt(ch,CURLOPT_URL,"http://127.0.0.1:8093/action/closepos");
    curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_wh);
    curl_easy_setopt(ch,CURLOPT_POST,1);
    curl_easy_setopt(ch,CURLOPT_POSTFIELDS,post);
    curl_easy_setopt(ch,CURLOPT_WRITEDATA,&temp);
    curl_easy_perform(ch);
    curl_easy_cleanup(ch);

    free(post);

    return temp;
  
  }


private:

};

// the class factories
extern "C" broker* create() {
    return new igmGw;
}

extern "C" void destroy(broker* p) {
    delete p;
}
