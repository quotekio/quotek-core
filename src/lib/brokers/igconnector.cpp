#include "broker.h"
#include <curl/curl.h>
#include "rapidjson/rapidjson.h"
#include "../rapidjson/document.h"
#include <stdlib.h>
#include "../utils.h"

size_t curl_wh(void *ptr, size_t size, size_t nmemb, string stream)
{
    if (ptr != NULL) {

      string temp(static_cast<const char*>(ptr), size * nmemb);
      stream += temp;
    }
    return size*nmemb;
}

class igConnector : public broker {
public:

    bool requires_indices_list;

    virtual int init(string params) {
      
      cout << "Initializing broker Module.." << endl;      

      rapidjson::Document d;
      d.Parse<0>(params.c_str());

      username = d["username"].GetString();
      password = d["password"].GetString();
      api_key = d["api_key"].GetString();

      api_url = "https://demo-api.ig.com";
      requires_indices_list = 1;
      return 0;

    }

    virtual int requiresIndicesList() {
      return requires_indices_list;
    }

    virtual int setIndicesList(vector<string> il) {
      cout << "Setting Broker Indices List.." << endl;
      ilist = il;

      return 0;
    }
  
    virtual int connect() {
      cout << "Starting broker connection.." << endl;
      string temp = "";
      string htemp = "";
      string pdata = "";
      struct curl_slist *headers = NULL;
      CURL* ch = curl_easy_init();

      //Adds API Key to header;
      string apikey_header = "X-IG-API-KEY: " + api_key;
      headers = curl_slist_append(headers, "Accept: application/json");
      headers = curl_slist_append(headers, "Content-Type: application/json");
      headers = curl_slist_append(headers, apikey_header.c_str());

      //creates post data
      pdata = "{\"identifier\":\"" + username + "\",";
      pdata += "\"password\":\"" + password + "\" }";

      string c_url = api_url + "/gateway/deal/session";

      curl_easy_setopt(ch,CURLOPT_URL,c_url.c_str());
      curl_easy_setopt(ch,CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(ch,CURLOPT_POST,1);
      curl_easy_setopt(ch,CURLOPT_POSTFIELDS, pdata.c_str());
      curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_wh);
      curl_easy_setopt(ch,CURLOPT_WRITEDATA,&temp);
      curl_easy_setopt(ch,CURLOPT_HEADERFUNCTION,curl_wh);
      curl_easy_setopt(ch,CURLOPT_HEADERDATA,&htemp);

      curl_easy_perform(ch);
      curl_easy_cleanup(ch);
      curl_slist_free_all(headers);

      rapidjson::Document d;
      d.Parse<0>(temp.c_str());


      vector<string> hdata = split(htemp,'\n');

      for (int i=0;i<hdata.size();i++) {

        if ( hdata[i].find("CST: ") == 0 ) {
          cst = hdata[i];
        }

        if ( hdata[i].find("X-SECURITY-TOKEN: ") == 0 ) {
          security_token = hdata[i];
        }

      }
      
      return 0;
    }


    virtual vector<bvex> getValues() {
   
      vector<bvex> result;

      struct curl_slist *headers = NULL;
      string epics_list = "";
      rapidjson::Document d;

      for (int i=0;i<ilist.size();i++)  {
        epics_list += ilist[i] + ",";
      }

      string c_url = api_url + "/gateway/deal/markets?epics=" + epics_list;

      //Adds API Key to header;
      string apikey_header = "X-IG-API-KEY: " + api_key;
      headers = curl_slist_append(headers, "Accept: application/json");
      headers = curl_slist_append(headers, "Content-Type: application/json");
      headers = curl_slist_append(headers, apikey_header.c_str());
      headers = curl_slist_append(headers, cst.c_str());
      headers = curl_slist_append(headers, security_token.c_str());


      string temp = "";
      CURL* ch = curl_easy_init();
      curl_easy_setopt(ch,CURLOPT_URL,c_url.c_str());
      curl_easy_setopt(ch,CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_wh);
      curl_easy_setopt(ch,CURLOPT_WRITEDATA,&temp);
      curl_easy_perform(ch);
      curl_easy_cleanup(ch);
      curl_slist_free_all(headers);

      d.Parse<0>(temp.c_str());

      if (d["marketDetails"].IsArray()) {
        
        for (int i=0;i<d["marketDetails"].Capacity();i++) {

          bvex ex1;
          ex1.epic = d["marketDetails"][rapidjson::SizeType(i)]["instrument"]["epic"].GetString();
          ex1.buy = d["marketDetails"][rapidjson::SizeType(i)]["snapshot"]["offer"].GetDouble();
          ex1.sell = d["marketDetails"][rapidjson::SizeType(i)]["snapshot"]["bid"].GetDouble();
          result.push_back(ex1);

        }

      }

      return result;
    }

    virtual string getPositions() {
    
      string temp = "";
      CURL* ch = curl_easy_init();
      curl_easy_setopt(ch,CURLOPT_URL,"http://127.0.0.1:9090/get/positions");
      curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_wh);
      curl_easy_setopt(ch,CURLOPT_WRITEDATA,&temp);
      curl_easy_perform(ch);
      curl_easy_cleanup(ch);
      return temp;
    }
    

  virtual string openPos(string epic,string way,int nbc,int stop,int limit) {


    string temp = "";

    char* url_and_params = (char*) malloc(1024);
    sprintf(url_and_params,"http://127.0.0.1:9090/action/openpos?epic=%s&way=%s&nbc=%d&stop=%d&limit=%d",epic.c_str(),way.c_str(),nbc,stop,limit);

    CURL* ch = curl_easy_init();
    curl_easy_setopt(ch,CURLOPT_URL,url_and_params);
    curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_wh);
    curl_easy_setopt(ch,CURLOPT_WRITEDATA,&temp);
    curl_easy_perform(ch);
    curl_easy_cleanup(ch);

    free(url_and_params);
    return temp;

  }


  virtual string closePos(string dealid) {
  
    string temp = "";
    char*  url_and_params = (char*) malloc(1024);

    sprintf(url_and_params,"http://127.0.0.1:9090/action/closepos?dealid=%s",dealid.c_str());
    CURL* ch = curl_easy_init();
    curl_easy_setopt(ch,CURLOPT_URL,url_and_params);
    curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_wh);
    curl_easy_setopt(ch,CURLOPT_WRITEDATA,&temp);
    curl_easy_perform(ch);
    curl_easy_cleanup(ch);

    free(url_and_params);
    return temp;
  
  }


private:

  string cst;
  string security_token;

};

// the class factories
extern "C" broker* create() {
    return new igConnector;
}

extern "C" void destroy(broker* p) {
    delete p;
}
