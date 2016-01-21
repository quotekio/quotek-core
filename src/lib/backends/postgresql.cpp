#include "backend.h"
#include "../rapidjson/document.h"
#include "../utils.h"

#include <pqxx/pqxx>
#include <sstream>
#include <stdlib.h>

class postgresql : public backend {
public:

    virtual int init(string params) {
      
      rapidjson::Document d;
      stringstream cstr;

      d.Parse<0>(params.c_str());

      host = d["host"].GetString();
      port = d["port"].GetString();
      username = d["username"].GetString();
      password = d["password"].GetString();
      database = d["database"].GetString();

      cstr << "host=" << host << " dbname=" << database << " user=" << username << " password=" << password;
      
      try {
        dbh = new pqxx::connection(cstr.str().c_str());
      }
      catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
      }

      return 0;

    }

    virtual int init(std::string ihost, 
                     int iport, 
                     std::string iusername, 
                     std::string ipassword, 
                     std::string idatabase ) {

      stringstream cstr;

      host = ihost;
      port = int2string(iport);
      username = iusername;
      password = ipassword;
      database = idatabase;

      cstr << "host=" << host << " dbname=" << database << " user=" << username << " password=" << password;
      try {
        dbh = new pqxx::connection(cstr.str().c_str());
      }
      catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
      }

      return 0;

    }

    virtual int connect() {
      return 0;
    }

    virtual quotek::data::records query(string q) {
      
      quotek::data::records result;
      std::string outp;
      quotek::data::record r;

        /*if (!dbh.ExecTuplesOk(q.c_str())) return result;

        for (int i = 0; i < dbh.Tuples(); i++) {
          r.timestamp = dbh.GetValue(i,0);
          r.value = dbh.GetValue(i,1);
          r.sprad = dbh.GetValue(i,2);
          result.append(r);
        }
        */

      return result;
    }

    virtual quotek::data::records query(string indice, int tinf, int tsup) {

      quotek::data::records result ;
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

      

      // Perform http request to postgresql backend and get result.
      // Destroy http handler to free memory.
      
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


    virtual int save(long timestamp, 
                     std::string tag, 
                     std::string data) { 

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
       
       //Debug.
       //std::cout << outp << std::endl;
       return 0;
    }

    virtual int store(string indice, quotek::data::records& recs) {

      std::ostringstream sdata;
      string outp;

      sdata << "[\n";
      sdata << "\t{ \"name\": \"" << indice << "\",\n";
      sdata << "\t  \"columns\" : [ \"time\", \"value\", \"spread\" ],\n";
      sdata << "\t  \"points\" : [" << records2sql(recs) << "]\n"; 
      sdata << "\t}\n";
      sdata << "]";

      std::string sdata_str = sdata.str();
      return 0;
    }

    virtual int store(string indice, quotek::data::record& rec) {

      std::ostringstream sdata;
      string outp;

      sdata << "[\n";
      sdata << "\t{ \"name\": \"" << indice << "\",\n";
      sdata << "\t  \"columns\" : [ \"time\", \"value\", \"spread\" ],\n";
      sdata << "\t  \"points\" : [" << record2sql(rec) << "]\n"; 
      sdata << "\t}\n";
      sdata << "]";

      std::string sdata_str = sdata.str();

      return 0;
    }

    virtual int saveHistory(quotek::core::position& pos)  {

      std::ostringstream sdata;
      string outp;

      sdata << "[\n";
      sdata << "\t{ \"name\": \"__history__\",\n";
      sdata << "\t  \"columns\" : [ \"indice\", \"epic\", \"dealid\", \"size\", \"stop\", \"limit\", \"open\", \"pnl\", \"pnl_peak\", \"open_date\", \"close_date\", \"identifier\" ],\n";
      sdata << "\t  \"points\" : [" << pos2sql(pos) << "]\n"; 
      sdata << "\t}\n";
      sdata << "]";

      std::string sdata_str = sdata.str();

      return 0;

    }

    virtual int saveHistory(quotek::data::cvector<quotek::core::position>& plist) {

      std::ostringstream sdata;
      string outp;

      sdata << "[\n";
      sdata << "\t{ \"name\": \"__history__\",\n";
      sdata << "\t  \"columns\" : [ \"indice\", \"epic\", \"dealid\", \"size\", \"stop\", \"limit\", \"open\", \"pnl\", \"pnl_peak\", \"open_date\", \"close_date\", \"identifier\"],\n";
      sdata << "\t  \"points\" : [" << poslist2sql(plist) << "]\n"; 
      sdata << "\t}\n";
      sdata << "]";

      std::string sdata_str = sdata.str();

      //outp = hhdl->post(pre_url,sdata_str);
      //hhdl->destroy();
      return 0;

    }

private:
  string host;
  string port;
  string username;
  string password;
  string database;
  pqxx::connection* dbh;


  std::string record2sql(quotek::data::record& rec)  {

    std::ostringstream sqstream;
    sqstream << "(" << rec.timestamp << ", " << rec.value << ", " << rec.spread << ")";
    return sqstream.str();
  };

  std::string records2sql(quotek::data::records& recs) {

    std::ostringstream sqstream;    
    for (int i=0;i<recs.size();i++)  {
      sqstream << record2sql(recs.get_data()[i]);
      if (i !=  (recs.size() - 1 ) ) sqstream << ",";

    }
    return sqstream.str();
  };

  std::string pos2sql(quotek::core::position& pos)  {
  
    std::ostringstream sqstream;
    sqstream << "(" << "\"" << pos.asset_name << "\", "
            << "\"" << pos.asset_id << "\", "
            << "\"" << pos.ticket_id << "\", "
            << pos.size << ", " << pos.stop << ", "
            << pos.limit << ", " << pos.open << ", "
            << pos.pnl << ", " << pos.stats->pnl_peak << ", "
            << pos.open_date << ", " << pos.close_date <<  ", " 
            << "\"" << pos.identifier << "\"" << ")";

    return sqstream.str();

  }

  std::string poslist2sql(quotek::data::cvector<quotek::core::position>& plist) {

    std::ostringstream sqstream;    
    for (int i=0;i<plist.size();i++)  {
      sqstream << pos2sql(plist[i]);
      if (i !=  (plist.size() - 1 ) ) sqstream << ",";

    }
    return sqstream.str();
  }

};

// the class factories
extern "C" backend* create() {
    return new postgresql;
}

extern "C" void destroy(backend* p) {
    delete p;
}
