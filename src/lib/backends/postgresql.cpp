#include "backend.h"
#include "../rapidjson/document.h"
#include "../utils.h"

#include <pqxx/pqxx>
#include <sstream>
#include <stdlib.h>

class postgresql : public backend {
public:


    //OK
    virtual int init(string params) {
      
      rapidjson::Document d;
      stringstream cstr;

      d.Parse<0>(params.c_str());

      host = d["host"].GetString();
      port = d["port"].GetString();
      username = d["username"].GetString();
      password = d["password"].GetString();
      database = d["database"].GetString();

      cstr << "host=" << host <<  " port="  << port << " dbname=" << database << " user=" << username << " password=" << password;
      
      try {
        dbh = new pqxx::connection(cstr.str().c_str());
      }
      catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
      }

      init_tables();

      return 0;
    }

    //OK
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

      cstr << "host=" << host << " port="  << port << " dbname=" << database << " user=" << username << " password=" << password;
      try {
        dbh = new pqxx::connection(cstr.str().c_str());
      }
      catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
      }

      init_tables();

      return 0;
    }

    //OK
    virtual int connect() {
      return 0;
    }


    //OK
    virtual quotek::data::records query(string q) {
      
      quotek::data::records result;
      std::string outp;
      quotek::data::record r;

      pqxx::work w(*dbh);
      pqxx::result res = w.exec(q.c_str());
      w.commit();
      
      for (int i=0;i<res.size(); i++) {
        const pqxx::tuple row = res[i];
        r.timestamp = row[0].as<int>();
        r.value = row[1].as<float>();
        r.spread = row[2].as<float>();
        result.append(r);
      }

      return result;
    }

    //OK
    virtual quotek::data::records query(string indice, int tinf, int tsup) {

      quotek::data::records result ;
      std::ostringstream qstream;
      std::string outp;
      quotek::data::record r;

      if (tinf >= 0) {
        qstream << "SELECT timestamp, value, spread FROM " << indice <<
                   " WHERE timestamp > " << tinf << 
                   "s AND timestamp <" << tsup << " ORDER ASC";
      }

      else  {

        qstream << "SELECT timestamp, value, spread FROM " << indice <<
                   " WHERE timestamp > now() + " << tinf << 
                   " AND timestamp < now() + " << tsup << " ORDER ASC";
      }

      pqxx::work w(*dbh);
      pqxx::result res = w.exec(qstream.str().c_str());
      w.commit();
      
      for (int i=0;i<res.size(); i++) {
        const pqxx::tuple row = res[i];
        r.timestamp = row[0].as<int>();
        r.value = row[1].as<float>();
        r.spread = row[2].as<float>();
        result.append(r);
      }
      return result;

    }


    //NOK
    virtual int save(long timestamp, 
                     std::string tag, 
                     std::string data) { 

     std::string outp;
     stringstream sdata; 

       long tstamp = ( timestamp == 0 ) ? time(0) : timestamp;
       return 0;
    }

    //OK
    virtual int store(string indice, quotek::data::records& recs) {

      std::ostringstream qcreate;
      std::ostringstream squery;
      string outp;

      qcreate << "CREATE TABLE IF NOT EXISTS " << indice << " (timestamp INTEGER, value FLOAT, spread FLOAT);";

      pqxx::work w(*dbh);
      pqxx::result res = w.exec(qcreate.str().c_str());
      w.commit();
      
      squery << "INSERT INTO " << indice << " VALUES " << records2sql(recs) << ";";
      res = w.exec(squery.str().c_str());
      w.commit();

      return 0;
    }

    //OK
    virtual int store(string indice, quotek::data::record& rec) {

      std::ostringstream qcreate;
      std::ostringstream squery;
      string outp;

      qcreate << "CREATE TABLE IF NOT EXISTS " << indice << " (timestamp INTEGER, value FLOAT, spread FLOAT);";

      pqxx::work w(*dbh);
      pqxx::result res = w.exec(qcreate.str().c_str());
      w.commit();
      
      squery << "INSERT INTO " << indice << " VALUES " << record2sql(rec) << ";";
      res = w.exec(squery.str().c_str());
      w.commit();

      return 0;
    }

    //OK
    virtual int saveHistory(quotek::core::position& pos)  {

      std::ostringstream squery;
      string outp;
      
      squery << "INSERT INTO __history__ VALUES " << pos2sql(pos) << ";"; 

      pqxx::work w(*dbh);
      pqxx::result res = w.exec(squery.str().c_str());
      w.commit();

      return 0;

    }
    
    //OK
    virtual int saveHistory(quotek::data::cvector<quotek::core::position>& plist) {

      std::ostringstream squery;
      string outp;
      
      squery << "INSERT INTO __history__ VALUES " << poslist2sql(plist) << ";"; 

      pqxx::work w(*dbh);
      pqxx::result res = w.exec(squery.str().c_str());
      w.commit();

      return 0;

    }

private:
  string host;
  string port;
  string username;
  string password;
  string database;
  pqxx::connection* dbh;



  int init_tables() {
    std::ostringstream qcreate;

    qcreate << "CREATE TABLE IF NOT EXISTS __history__ (";
    qcreate << "indice VARCHAR(64),";
    qcreate << "epic VARCHAR(64),";
    qcreate << "dealid VARCHAR(64),";
    qcreate << "size INTEGER,";
    qcreate << "stop FLOAT,";
    qcreate << "limit FLOAT,";
    qcreate << "open FLOAT,";
    qcreate << "pnl FLOAT,";
    qcreate << "pnl_peak FLOAT,";
    qcreate << "open_date INTEGER,";
    qcreate << "close_date INTEGER,";
    qcreate << "identifier VARCHAR(64),";

    pqxx::work w(*dbh);
    pqxx::result res = w.exec(qcreate.str().c_str());
    w.commit();

    return 0;

  }

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
