#include "backend.h"
#include "../rapidjson/document.h"
#include "../utils.h"

#include <influxdb/influxdb.h>
#include <quotek/http.hpp>
#include <sstream>
#include <stdlib.h>

class influxdb : public backend {
public:

    virtual int init(string params) {
      
      rapidjson::Document d;
      d.Parse<0>(params.c_str());

      host = d["host"].GetString();
      port = d["port"].GetString();
      username = d["username"].GetString();
      password = d["password"].GetString();
      database = d["database"].GetString();

      pre_url = "http://" + 
                 host + ":" + 
                 port + 
                 "/db/" +
                 database + 
                 "/series" + 
                 "?u=" + 
                 username + 
                 "&p=" + 
                 password + 
                 "&time_precision=s";
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

      pre_url = "http://" + 
                 host + ":" + 
                 port + 
                 "/db/" +
                 database + 
                 "/series" + 
                 "?u=" + 
                 username + 
                 "&p=" + 
                 password + 
                 "&time_precision=s";
      return 0;

    }

    virtual int connect() {
      //influx doesn't need to maintain connection
      return 0;
    }

    virtual quotek::data::records query(string q) {
      
      quotek::data::records result;

      quotek::http* hhdl = prepare_http_handler();
      std::string url = pre_url;
      std::string outp;
      
      quotek::data::record r;

      url += "&q=" + hhdl->escape(q);

      // Perform http request to influxdb backend and get result.
      outp = hhdl->get(url);
      // Destroy http handler to free memory.
      hhdl->destroy();

      // Effectively parse result
      rapidjson::Document d;
      d.Parse<0>(outp.c_str());

      if (d.IsArray())  {

        for ( int i=0; i< d[0u]["points"].Size(); i++  ) {
           rapidjson::Value& points = d[0u]["points"][i];           
           r.timestamp = points[0u].GetInt();
           r.value = points[3].GetDouble();
           r.spread = points[2].GetDouble();
           result.append(r);
        }
      }

      return result;
    }

    virtual quotek::data::records query(string indice, int tinf, int tsup) {

      quotek::data::records result ;
      quotek::http* hhdl = prepare_http_handler();
      std::string url = pre_url;
      std::ostringstream qstream;
      std::string outp;
      quotek::data::record r;

      if (tinf >= 0) {
        qstream << "SELECT value, spread FROM " << indice <<
                   " WHERE time > " << tinf << 
                   "s AND time <" << tsup << "s ORDER ASC";
      }

      else  {

        qstream << "SELECT value, spread FROM " << indice <<
                   " WHERE time > now() + " << tinf << 
                   "s AND time < now() + " << tsup << "s ORDER ASC";
      }

      

      url += "&q=" + hhdl->escape(qstream.str());
      // Perform http request to influxdb backend and get result.
      outp = hhdl->get(url);
      // Destroy http handler to free memory.
      hhdl->destroy();
      
      //shortcut avoiding parsing if no result
      if ( outp == "[]"  ) return result;
      
      // Effectively parse result
      rapidjson::Document d;
      d.Parse<0>(outp.c_str());

      if (d.HasParseError()) {
        cout << "[ERROR] Backend query error: JSON Parsing. returning empty result!" << endl;
        return result;
      }

      if (d.IsArray())  {

        for ( int i=0; i< d[0u]["points"].Size(); i++  ) {
           rapidjson::Value& points = d[0u]["points"][i];           
           r.timestamp = points[0u].GetInt();
           r.value = points[3].GetDouble();
           r.spread = points[2].GetDouble();
           result.append(r);
        }
      }

      return result;

    }

    virtual int store(string indice, quotek::data::records& recs) {

      std::ostringstream sdata;
      quotek::http* hhdl = prepare_http_handler();
      string outp;

      sdata << "[\n";
      sdata << "\t{ \"name\": \"" << indice << "\",\n";
      sdata << "\t  \"columns\" : [ \"time\", \"value\", \"spread\" ],\n";
      sdata << "\t  \"points\" : [" << records2json(recs) << "]\n"; 
      sdata << "\t}\n";
      sdata << "]";

      std::string sdata_str = sdata.str();

      outp = hhdl->post(pre_url,sdata_str);
      hhdl->destroy();
      return 0;
    }

    virtual int store(string indice, quotek::data::record& rec) {

      std::ostringstream sdata;
      quotek::http* hhdl = prepare_http_handler();
      string outp;

      sdata << "[\n";
      sdata << "\t{ \"name\": \"" << indice << "\",\n";
      sdata << "\t  \"columns\" : [ \"time\", \"value\", \"spread\" ],\n";
      sdata << "\t  \"points\" : [" << record2json(rec) << "]\n"; 
      sdata << "\t}\n";
      sdata << "]";

      std::string sdata_str = sdata.str();

      outp = hhdl->post(pre_url,sdata_str);

      hhdl->destroy();
      return 0;
    }

    virtual int saveHistory(quotek::core::position& pos)  {

      std::ostringstream sdata;
      quotek::http* hhdl = prepare_http_handler();
      string outp;

      sdata << "[\n";
      sdata << "\t{ \"name\": \"__history__\",\n";
      sdata << "\t  \"columns\" : [ \"indice\", \"epic\", \"dealid\", \"size\", \"stop\", \"limit\", \"open\", \"pnl\", \"pnl_peak\", \"open_date\", \"close_date\", \"identifier\" ],\n";
      sdata << "\t  \"points\" : [" << pos2json(pos) << "]\n"; 
      sdata << "\t}\n";
      sdata << "]";

      std::string sdata_str = sdata.str();

      outp = hhdl->post(pre_url,sdata_str);      
      hhdl->destroy();
      return 0;

    }

    virtual int saveHistory(quotek::data::cvector<quotek::core::position>& plist) {

      std::ostringstream sdata;
      quotek::http* hhdl = prepare_http_handler();
      string outp;

      sdata << "[\n";
      sdata << "\t{ \"name\": \"__history__\",\n";
      sdata << "\t  \"columns\" : [ \"indice\", \"epic\", \"dealid\", \"size\", \"stop\", \"limit\", \"open\", \"pnl\", \"pnl_peak\", \"open_date\", \"close_date\", \"identifier\"],\n";
      sdata << "\t  \"points\" : [" << poslist2json(plist) << "]\n"; 
      sdata << "\t}\n";
      sdata << "]";

      std::string sdata_str = sdata.str();

      outp = hhdl->post(pre_url,sdata_str);
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

  std::string record2json(quotek::data::record& rec)  {

    std::ostringstream jstream;
    jstream << "[" << rec.timestamp << ", " << rec.value << ", " << rec.spread << "]";
    return jstream.str();
  };

  std::string records2json(quotek::data::records& recs) {

    std::ostringstream jstream;    
    for (int i=0;i<recs.size();i++)  {
      jstream << record2json(recs.get_data()[i]);
      if (i !=  (recs.size() - 1 ) ) jstream << ",";

    }
    return jstream.str();
  };

  std::string pos2json(quotek::core::position& pos)  {
  
    std::ostringstream jstream;
    jstream << "[" << "\"" << pos.asset_name << "\", "
            << "\"" << pos.asset_id << "\", "
            << "\"" << pos.ticket_id << "\", "
            << pos.size << ", " << pos.stop << ", "
            << pos.limit << ", " << pos.open << ", "
            << pos.pnl << ", " << pos.stats->pnl_peak << ", "
            << pos.open_date << ", " << pos.close_date <<  ", " 
            << "\"" << pos.identifier << "\"" << "]";

    return jstream.str();

  }

  std::string poslist2json(quotek::data::cvector<quotek::core::position>& plist) {

    std::ostringstream jstream;    
    for (int i=0;i<plist.size();i++)  {
      jstream << pos2json(plist[i]);
      if (i !=  (plist.size() - 1 ) ) jstream << ",";

    }
    return jstream.str();
  }


  quotek::http* prepare_http_handler() {
    quotek::http* hhdl = new quotek::http();
    hhdl->add_header("Content: Application/Json");
    hhdl->add_header("Accept: Application/Json");
    return hhdl;
  };


};

// the class factories
extern "C" backend* create() {
    return new influxdb;
}

extern "C" void destroy(backend* p) {
    delete p;
}
