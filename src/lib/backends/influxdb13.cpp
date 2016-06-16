#include "../utils.h"
#include <quotek/backend.hpp>
#include <quotek/json.hpp>
#include <quotek/http.hpp>
#include <sstream>

class influxdb13 : public backend {
public:

    virtual int init(string params) {
      
      quotek::json::node* rnode = quotek::json::parser::parse(params.c_str());
      quotek::json::jobject root = rnode->AsObject();

      host = root["host"]->AsString();
      port = root["port"]->AsString();
      username = root["username"]->AsString();
      password = root["password"]->AsString();
      database = root["database"]->AsString();

      pre_url = "http://" + host + ":" +  port;
      main_args = "?db=" + database + "&epoch=s";
      //main_args = "?db=" + database + "&epoch=s&pretty=true";

      return 0;

    }

    virtual int init(std::string ihost, 
                     int iport, 
                     std::string iusername, 
                     std::string ipassword, 
                     std::string idatabase ) {
      
      host = ihost;
      port = int2string(iport);
      username = iusername;
      password = ipassword;
      database = idatabase;

      pre_url = "http://" + host + ":" +  port;

      main_args = "?db=" + database + "&epoch=s";
      

      return 0;

    }

    virtual int connect() {
      //influx doesn't need to maintain connection
      return 0;
    }

    virtual quotek::data::records query(string q) {
      
      quotek::data::records result;

      quotek::http* hhdl = prepare_http_handler();
      std::string url = pre_url + "/query" + main_args ;
      std::string outp;
      
      quotek::data::record r;

      url += "&q=" + hhdl->escape(q);

      std::cout << "URL:" << url << std::endl;

      // Perform http request to influxdb backend and get result.
      outp = hhdl->get(url);
      // Destroy http handler to free memory.
      hhdl->destroy();

      // Effectively parse result

      quotek::json::node* rnode = quotek::json::parser::parse(outp.c_str());

      if ( rnode != nullptr ) {

        if ( rnode->IsObject() ) {

          quotek::json::jobject root = rnode->AsObject();
          quotek::json::jarray rresults = root["results"]->AsArray();

          quotek::json::jobject robject1 = rresults[0]->AsObject();
          quotek::json::jarray series = robject1["series"]->AsArray();
          quotek::json::jobject series1 = series[0]->AsObject();
          quotek::json::jarray values = series1["values"]->AsArray();

          for (int i=0;i< values.size();i++ ) {

             quotek::json::jarray p = values[i]->AsArray();
             r.timestamp = p[0]->AsNumber();
             r.value = p[2]->AsNumber();
             r.spread = p[1]->AsNumber();
             result.append(r);
          }
        }
      }

      return result;
    }

    virtual quotek::data::records query(string indice, int tinf, int tsup) {

      quotek::data::records result ;
      quotek::http* hhdl = prepare_http_handler();
      std::string url = pre_url + "/query" + main_args ;

      std::ostringstream qstream;
      std::string outp;
      quotek::data::record r;

      if (tinf >= 0) {

        qstream << "SELECT * from " << indice << " WHERE time > " << tinf <<
        "s AND time < " << tsup << "s;" ;

      }

      else  {

        qstream << "SELECT * from " << indice << " WHERE time > now() + " << tinf <<
        "s AND time < now() + " << tsup << "s;" ;

      }

      

      url += "&q=" + hhdl->escape(qstream.str());
      // Perform http request to influxdb backend and get result.
      outp = hhdl->get(url);
      // Destroy http handler to free memory.
      hhdl->destroy();
      
      //shortcut avoiding parsing if no result
      if ( outp == "{}"  ) return result;
      
      // Effectively parse result
      //cout << outp << endl;

      quotek::json::node* rnode = quotek::json::parser::parse(outp.c_str());

      if ( rnode != nullptr ) {

        if ( rnode->IsObject() ) {


          quotek::json::jobject root = rnode->AsObject();
          quotek::json::jarray rresults = root["results"]->AsArray();

          quotek::json::jobject robject1 = rresults[0]->AsObject();
          quotek::json::jarray series = robject1["series"]->AsArray();
          quotek::json::jobject series1 = series[0]->AsObject();
          quotek::json::jarray values = series1["values"]->AsArray();

          for (int i=0;i< values.size();i++ ) {

             quotek::json::jarray p = values[i]->AsArray();
             r.timestamp = p[0]->AsNumber();
             r.value = p[2]->AsNumber();
             r.spread = p[1]->AsNumber();
             result.append(r);

          }
        }
      }

      else {
        cout << "[ERROR] Backend query error: JSON Parsing. returning empty result!" << endl;
        return result;
      }
    
      return result;

    }

    virtual int save(long timestamp, 
                     std::string tag, 
                     std::string data) { 

     quotek::http* hhdl = prepare_http_handler();
     std::string outp;
     stringstream sdata;  
     return 0;

    }

    virtual int store(string indice, quotek::data::records& recs) {

      std::ostringstream sdata;
      quotek::http* hhdl = prepare_http_handler();
      string outp;

      
      for (auto r: recs.get_data() ) {

        sdata << indice << ",spread=" 
              << r.spread << " value=" 
              << r.value << " " 
              << r.timestamp * 1000 * 1000 * 1000
              << std::endl;

      }
      
      std::string sdata_str = sdata.str();

      outp = hhdl->post(pre_url,sdata_str);
      hhdl->destroy();
      return 0;
    }

    virtual int store(string indice, quotek::data::record& rec) {

      std::ostringstream sdata;
      quotek::http* hhdl = prepare_http_handler();
      string outp;

      std::string url = pre_url + "/write" + main_args;
      
      sdata << indice << ",spread=" 
            << rec.spread << " value=" 
            << rec.value << " " 
            << rec.timestamp * 1000 * 1000 * 1000;

      std::string sdata_str = sdata.str();
      outp = hhdl->post(url,sdata_str);

      hhdl->destroy();
      return 0;
    }

    virtual int saveHistory(quotek::core::position& pos)  {

      std::ostringstream sdata;
      quotek::http* hhdl = prepare_http_handler();
      string outp;

      std::string url = pre_url + "/write" + main_args;

      sdata << "__history__,indice=" 
            << pos.asset_name 
            << ",epic="
            << pos.asset_id
            << ",dealid=" 
            << pos.ticket_id
            << ",size="
            << pos.size
            << ",stop="
            << pos.stop
            << ",limit="
            << pos.limit
            << ",open="
            << pos.open
            << ",pnl_peak="
            << pos.stats->pnl_peak
            << ",open_date="
            << pos.open_date
            << ",close_date="
            << pos.close_date
            << ",identifier="
            << pos.identifier
            << " value=" << pos.pnl << " " << time(NULL) * 1000 * 1000 * 1000;
     
      std::string sdata_str = sdata.str();

      outp = hhdl->post(url,sdata_str);

      hhdl->destroy();
      return 0;

    }

    virtual int saveHistory(quotek::data::cvector<quotek::core::position>& plist) {

      std::ostringstream sdata;
      quotek::http* hhdl = prepare_http_handler();
      string outp;
      std::string url = pre_url + "/write" + main_args;

      for (auto pos : plist) {

        sdata << "__history__,indice=" 
              << pos.asset_name 
              << ",epic="
              << pos.asset_id
              << ",dealid=" 
              << pos.ticket_id
              << ",size="
              << pos.size
              << ",stop="
              << pos.stop
              << ",limit="
              << pos.limit
              << ",open="
              << pos.open
              << ",pnl_peak="
              << pos.stats->pnl_peak
              << ",open_date="
              << pos.open_date
              << ",close_date="
              << pos.close_date
              << ",identifier="
              << pos.identifier
              << " value=" << pos.pnl << " " << time(NULL) * 1000 * 1000 * 1000
              << std::endl;
      }


      std::string sdata_str = sdata.str();

      outp = hhdl->post(url,sdata_str);
      hhdl->destroy();
      return 0;

    }

private:
  string host;
  string port;
  string username;
  string password;
  string database;
  string pre_url;
  string main_args;


  quotek::http* prepare_http_handler() {
    quotek::http* hhdl = new quotek::http();
    hhdl->add_header("Content: Application/Json");
    hhdl->add_header("Accept: Application/Json");
    return hhdl;
  };


};

// the class factories
extern "C" backend* create() {
    return new influxdb13;
}

extern "C" void destroy(backend* p) {
    delete p;
}
