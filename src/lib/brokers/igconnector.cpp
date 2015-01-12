#include "broker.h"
#include <curl/curl.h>
#include "../rapidjson/rapidjson.h"
#include "../rapidjson/document.h"
#include <stdlib.h>
#include "../utils.h"
#include "../assoc.h"
#include <pthread.h>

#define MAX_UPTIME 42000

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
      
      //creates uptime loop, to reconnect when IG session expires.
      pthread_t uptime_loop;
      pthread_create(&uptime_loop,NULL,igConnector::staticUptimeLoop, this);

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
      cout << "(Re)Starting broker connection.." << endl;

      cst = "";
      security_token = "";

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
      if (d.HasParseError() ) return 1;
     
      vector<string> hdata = split(htemp,'\n');

      for (int i=0;i<hdata.size();i++) {

        if ( hdata[i].find("CST: ") == 0 ) {
          cst = hdata[i];
        }

        if ( hdata[i].find("X-SECURITY-TOKEN: ") == 0 ) {
          security_token = hdata[i];
        }

      }

      if ( cst == "" || security_token == "") {
        return 1;
      }

      //loads currencies map right after connect
      loadCurrenciesMap();

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

      headers = addHeaders();

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
      if (d.HasParseError() ) return result;

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

    virtual vector<bpex> getPositions() {
    
      vector<bpex> result;

      string temp = "";
      CURL* ch = curl_easy_init();
      string c_url = api_url + "/gateway/deal/positions";
      curl_slist *headers = NULL;
      rapidjson::Document d;
 
      headers = addHeaders();

      curl_easy_setopt(ch,CURLOPT_URL,c_url.c_str());
      curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_wh);
      curl_easy_setopt(ch,CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(ch,CURLOPT_WRITEDATA,&temp);
      curl_easy_perform(ch);
      curl_easy_cleanup(ch);

      curl_slist_free_all(headers);
      
      d.Parse<0>(temp.c_str());
      if (d.HasParseError() ) return result;

      if (d["positions"].IsArray()) {
        for (int i=0;i<d["positions"].Capacity();i++) {

          bpex p1;

          rapidjson::Value& jval = d["positions"][rapidjson::SizeType(i)]["position"];
          rapidjson::Value& jval_m = d["positions"][rapidjson::SizeType(i)]["market"];

          p1.size = roundint(jval["dealSize"].GetDouble()); 

          string direction = jval["direction"].GetString();

          //puts size in reverse if direction is sell.
          if ( direction == "SELL" )  {
            p1.size *= -1;
          }

          p1.dealid = jval["dealId"].GetString();
          p1.open = jval["openLevel"].GetDouble();
          p1.epic = jval_m["epic"].GetString();

          p1.stop = 0;
          
          if ( ! jval["stopLevel"].IsNull() ) {
            p1.stop = jval["stopLevel"].GetDouble();
          }

          p1.limit = 0;
          if (! jval["limitLevel"].IsNull() ) {
            p1.limit = jval["limitLevel"].GetDouble();
          }

          result.push_back(p1);
          
        }
      }

      lastpos = result;
      return result;
  }
    

  virtual string openPos(string epic,string way,int nbc,int stop,int limit) {

    string temp = "";
    string pdata = "";
    string stop_str = "";
    string limit_str = "";
    CURL* ch = curl_easy_init();
    string c_url = api_url + "/gateway/deal/positions/otc";
    curl_slist *headers = NULL;
    rapidjson::Document d;
    
    headers = addHeaders();

    if (stop == 0) {
      stop_str = "null";
    }
    else {
      stop_str = "\"" + int2string(stop) + "\"";
    }

    if (limit == 0) {
      limit_str = "null";
    }
    else  {
      limit_str = "\"" + int2string(limit) + "\"";
    }

    //build post data
    pdata = "{\n";
    pdata += "    \"epic\": \"" + epic + "\",\n";
    pdata += "    \"expiry\": \"-\",\n";
    pdata += "    \"direction\": \"" + upper(way) + "\",\n";
    pdata += "    \"size\": \"" + int2string(nbc) + "\",\n";
    pdata += "    \"orderType\": \"MARKET\",\n";
    //pdata += "    \"level\": null,\n";
    pdata += "    \"guaranteedStop\": \"false\",\n";
    pdata += "    \"stopDistance\": " + stop_str + ",\n";
    pdata += "    \"stopLevel\": null,\n";
    //pdata += "    \"trailingStop\": null,\n";
    //pdata += "    \"trailingStopIncrement\": null,\n";
    pdata += "    \"forceOpen\": \"true\",\n";
    pdata += "    \"limitDistance\": " + limit_str + ",\n";
    pdata += "    \"limitLevel\": null,\n";
    //pdata += "    \"quoteId\": null,\n";
    pdata += "    \"currencyCode\": \"" + currencies_map[epic] + "\"\n" ;
    pdata += "}"; 

    curl_easy_setopt(ch,CURLOPT_URL,c_url.c_str());
    curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_wh);
    curl_easy_setopt(ch,CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(ch,CURLOPT_WRITEDATA,&temp);
    curl_easy_setopt(ch,CURLOPT_POST, 1);
    curl_easy_setopt(ch,CURLOPT_POSTFIELDS, pdata.c_str());
    curl_easy_perform(ch);
    curl_easy_cleanup(ch);

    curl_slist_free_all(headers);
    
    d.Parse<0>(temp.c_str());
    if (d.HasParseError() ) return "";

    string res = "ERROR: NULLREF";
    if (! d["dealReference"].IsNull() ) res = d["dealReference"].GetString();
    else if (! d["errorCode"].IsNull()) res = std::string("ERROR:") + d["errorCode"].GetString();
    return res;

  }


  virtual string closePos(string dealid) {
  
    string temp = "";
    string c_url = api_url + "/gateway/deal/positions/otc" ;
    curl_slist *headers = NULL;
    rapidjson::Document d;
    string pdata;

    bpex pos;

    //finds position in current list
    for (int i=0;i<lastpos.size();i++) {
      if (lastpos[i].dealid == dealid) {
        pos = lastpos[i];
        break;
      }
    }

    string way = ( pos.size < 0 ) ? "BUY" : "SELL";
    int size = (pos.size < 0) ? pos.size * -1 : pos.size; 
    
    headers = addHeaders();

    //hack IG DELETE with body Issue
    headers = curl_slist_append(headers, "_METHOD: DELETE");

    pdata = "{\n";
    pdata += "    \"dealId\": \"" + dealid + "\",\n";
    pdata += "    \"expiry\": \"-\",\n";
    pdata += "    \"direction\": \"" + upper(way) + "\",\n";
    pdata += "    \"size\": \"" + int2string(size) + "\",\n";
    pdata += "    \"orderType\": \"MARKET\"\n";
    pdata += "}";

    CURL* ch = curl_easy_init();
    curl_easy_setopt(ch,CURLOPT_URL,c_url.c_str());
    curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_wh);
    curl_easy_setopt(ch,CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(ch,CURLOPT_WRITEDATA,&temp);
    //hack IG DELETE with body Issue
    //curl_easy_setopt(ch, CURLOPT_CUSTOMREQUEST, "DELETE"); 
    curl_easy_setopt(ch,CURLOPT_POST,1);
    curl_easy_setopt(ch,CURLOPT_POSTFIELDS, pdata.c_str());
    curl_easy_perform(ch);
    curl_easy_cleanup(ch);

    curl_slist_free_all(headers);

    d.Parse<0>(temp.c_str());
    if (d.HasParseError() ) return "";


    string res = "ERROR: NULLREF";
    if (! d["dealReference"].IsNull() ) res = d["dealReference"].GetString();
    else if (! d["errorCode"].IsNull()) res = std::string("ERROR:") + d["errorCode"].GetString();
    return res;

  }


private:

  string cst;
  string security_token;
  AssocArray<string> currencies_map;
  vector<bpex> lastpos;
  int uptime_s;

  inline curl_slist* addHeaders() {

    curl_slist* headers = NULL;

    //Adds API Key to header;
    string apikey_header = "X-IG-API-KEY: " + api_key;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "version: 1");
    headers = curl_slist_append(headers, apikey_header.c_str());
    headers = curl_slist_append(headers, cst.c_str());
    headers = curl_slist_append(headers, security_token.c_str());
  
    return headers;

  }


  static void* staticUptimeLoop(void* p) {
    static_cast<igConnector*>(p)->uptimeLoop(NULL);
    return NULL;
  }

  void* uptimeLoop(void*) {
    uptime_s = 0;
    while(1) {
      uptime_s++;
      if (uptime_s >= MAX_UPTIME) {
        connect();

        uptime_s =0;
      }
      sleep(1);

    }
    return NULL;
  }

  void loadCurrenciesMap() {

    struct curl_slist *headers = NULL;
    string epics_list = "";
    rapidjson::Document d;

    for (int i=0;i<ilist.size();i++)  {
      epics_list += ilist[i] + ",";
    }

    string c_url = api_url + "/gateway/deal/markets?epics=" + epics_list;

    headers = addHeaders();

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
    if (d.HasParseError()) return ;

    if (d["marketDetails"].IsArray()) {
      
      for (int i=0;i<d["marketDetails"].Capacity();i++) {

        string epic = d["marketDetails"][rapidjson::SizeType(i)]["instrument"]["epic"].GetString();
        currencies_map[epic] = "";
        rapidjson::Value& c_array = d["marketDetails"][rapidjson::SizeType(i)]["instrument"]["currencies"];

        if (c_array.IsArray()) {

          //pick the best suite currency
          for (int j=0;j< c_array.Capacity();j++  ) {

            string c_array_code = c_array[rapidjson::SizeType(j)]["code"].GetString();

            if ( c_array_code == "EUR"  ) {
              currencies_map[epic] = "EUR";
              break; 
            }             

            else if ( c_array_code == "USD"  ) {
              currencies_map[epic] = "USD";
              break; 
            }
          }

          if (currencies_map[epic] == "")  {
            currencies_map[epic] = c_array[rapidjson::SizeType(0)]["code"].GetString();
          }

        }

      }

    }

  }

};

// the class factories
extern "C" broker* create() {
    return new igConnector;
}

extern "C" void destroy(broker* p) {
    delete p;
}
