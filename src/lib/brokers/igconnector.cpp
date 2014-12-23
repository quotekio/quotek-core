#include "broker.h"
#include <curl/curl.h>
#include "../rapidjson/rapidjson.h"
#include "../rapidjson/document.h"
#include <stdlib.h>
#include "../utils.h"
#include "../assoc.h"

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

      //loads currencies map right after connect
      loadCurrenciesMap();

      //debug
      /*for (int i=0;i<ilist.size();i++) {
        cout << "EPIC:" << ilist[i] << ", CURRENCY:" << currencies_map[ilist[i]] << endl;
      }*/

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

      if (d["positions"].IsArray()) {
        for (int i=0;i<d["positions"].Capacity();i++) {

          bpex p1;
          p1.size = d["positions"][rapidjson::SizeType(i)]["position"]["contractSize"].GetInt(); 

          string direction = d["positions"][rapidjson::SizeType(i)]["position"]["direction"].GetString();

          //puts size in reverse if direction is sell.
          if ( direction == "SELL" )  {
            p1.size *= -1;
          }

          p1.dealid = d["positions"][rapidjson::SizeType(i)]["position"]["dealId"].GetString();
          p1.open = d["positions"][rapidjson::SizeType(i)]["position"]["openLevel"].GetDouble();
          p1.epic = d["positions"][rapidjson::SizeType(i)]["market"]["epic"].GetString();

          p1.stop = 0;
          if (  d["positions"][rapidjson::SizeType(i)]["position"]["stopLevel"].IsDouble() ) {
            p1.stop = d["positions"][rapidjson::SizeType(i)]["position"]["stopLevel"].GetDouble();
          }

          p1.limit = 0;
          if (  d["positions"][rapidjson::SizeType(i)]["position"]["stopLevel"].IsDouble() ) {
            p1.limit = d["positions"][rapidjson::SizeType(i)]["position"]["limitLevel"].GetDouble();
          }

          result.push_back(p1);
          
        }
      }

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
    pdata += "    \"expiry\": \"DFB\",\n";
    pdata += "    \"direction\": \"" + upper(way) + "\",\n";
    pdata += "    \"size\": \"" + int2string(nbc) + "\",\n";
    pdata += "    \"orderType\": \"MARKET\",\n";
    //pdata += "    \"level\": null,\n";
    pdata += "    \"guaranteedStop\": \"true\",\n";
    pdata += "    \"stopLevel\": " + stop_str + ",\n";
    //pdata += "    \"stopDistance\": null,\n";
    //pdata += "    \"trailingStop\": null,\n";
    //pdata += "    \"trailingStopIncrement\": null,\n";
    pdata += "    \"forceOpen\": \"true\",\n";
    pdata += "    \"limitLevel\": " + limit_str + ",\n";
    //pdata += "    \"limitDistance\": null,\n";
    //pdata += "    \"quoteId\": null,\n";
    pdata += "    \"currencyCode\": \"" + currencies_map[epic] + "\"\n" ;
    pdata += "}"; 

    cout << pdata << endl;

    curl_easy_setopt(ch,CURLOPT_URL,c_url.c_str());
    curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_wh);
    curl_easy_setopt(ch,CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(ch,CURLOPT_WRITEDATA,&temp);
    curl_easy_setopt(ch,CURLOPT_POST, 1);
    curl_easy_setopt(ch,CURLOPT_POSTFIELDS, pdata.c_str());
    curl_easy_perform(ch);
    curl_easy_cleanup(ch);

    curl_slist_free_all(headers);
    
    /*
    cout << temp << endl;
    d.Parse<0>(temp.c_str());
    */
    
    return temp;

  }


  virtual string closePos(string dealid) {
  
    string temp = "";
    string c_url = api_url + "/gateway/deal/positions/otc" ;
    curl_slist *headers = NULL;
    rapidjson::Document d;
    
    headers = addHeaders();

    CURL* ch = curl_easy_init();
    curl_easy_setopt(ch,CURLOPT_URL,c_url.c_str());
    curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_wh);
    curl_easy_setopt(ch,CURLOPT_WRITEDATA,&temp);
    curl_easy_setopt(ch, CURLOPT_CUSTOMREQUEST, "DELETE"); 
    curl_easy_perform(ch);
    curl_easy_cleanup(ch);

    curl_slist_free_all(headers);

    return temp;
  
  }


private:

  string cst;
  string security_token;
  AssocArray<string> currencies_map;

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
