#include "../utils.h"
#include <quotek/backend.hpp>
#include <quotek/json.hpp>
#include <quotek/http.hpp>
#include <sstream>

class influxdb : public backend {
public:

    virtual int init(string params) {
      
      quotek::json::node* rnode = quotek::json::parser::parse(params.c_str());
      quotek::json::jobject root = rnode->AsObject();

      host = root["host"]->AsString();
      port = root["port"]->AsString();
      username = root["username"]->AsString();
      password = root["password"]->AsString();
      database = root["database"]->AsString();



      
     

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

      quotek::json::node* rnode = quotek::json::parser::parse(outp.c_str());

      if ( rnode != nullptr ) {
        if ( rnode->IsArray() ) {

          quotek::json::jarray root = rnode->AsArray();
          quotek::json::jobject relem = root[0]->AsObject();
          quotek::json::jarray points = relem["points"]->AsArray();

          for (int i=0;i< points.size();i++ ) {

             quotek::json::jarray p = points[i]->AsArray();
             r.timestamp = p[0]->AsNumber();
             r.value = p[3]->AsNumber();
             r.spread = p[2]->AsNumber();
             result.append(r);
          }
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
      //cout << outp << endl;

      quotek::json::node* rnode = quotek::json::parser::parse(outp.c_str());

      if ( rnode != nullptr ) {
        if ( rnode->IsArray() ) {

          quotek::json::jarray root = rnode->AsArray();
          quotek::json::jobject relem = root[0]->AsObject();
          quotek::json::jarray points = relem["points"]->AsArray();

          for (int i=0;i< points.size();i++ ) {

             quotek::json::jarray p = points[i]->AsArray();
             r.timestamp = p[0]->AsNumber();
             r.value = p[3]->AsNumber();
             r.spread = p[2]->AsNumber();
             result.append(r);
          }
        }
        else {
          cout << "[ERROR] Backend query error: JSON Parsing. returning empty result!" << endl;
          return result;
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

       long tstamp = ( timestamp == 0 ) ? time(0) : timestamp;

       sdata << "[\n";
       sdata << "\t{ \"name\": \"__save__\",\n";
       sdata << "\t  \"columns\" : [ \"time\", \"tag\", \"data\" ],\n";
       //may have to escape data ?!
       sdata << "\t  \"points\" : [ [" << timestamp <<  ",\"" << tag << "\",\"" << data << "\"] ]";
       sdata << "\t}\n";
       sdata << "]";
       
       //debug
       //std::cout << sdata.str() << std::endl;

       std::string sdata_str = sdata.str();
       outp = hhdl->post(pre_url, sdata_str);
       
       //Debug.
       //std::cout << outp << std::endl;
       hhdl->destroy();
       return 0;
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
