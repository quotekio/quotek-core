#include "../utils.h"
#include <pqxx/pqxx>
#include <sstream>
#include <stdlib.h>

#include <quotek/backend.hpp>
#include <quotek/json.hpp>

class postgresql : public backend {
public:


    //OK
    virtual int init(string params) {
      
      stringstream cstr;

      quotek::json::node* rnode = quotek::json::parser::parse(params.c_str());
      quotek::json::jobject root = rnode->AsObject();

      host = root["host"]->AsString();
      port = root["port"]->AsString();
      username = root["username"]->AsString();
      password = root["password"]->AsString();
      database = root["database"]->AsString();

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
                   "s AND timestamp <" << tsup << " ORDER BY timestamp ASC";
      }

      else  {

        qstream << "SELECT timestamp, value, spread FROM " << indice <<
                   " WHERE to_timestamp(timestamp) > CURRENT_TIMESTAMP - INTERVAL '" << abs(tinf) << " seconds'" <<
                   " AND to_timestamp(timestamp) < CURRENT_TIMESTAMP - INTERVAL '" << abs(tsup) << " seconds' ORDER BY timestamp ASC";
      }

      pqxx::work w(*dbh);
      pqxx::result res;
      try {
        res = w.exec(qstream.str().c_str());
        w.commit();
      }

      catch (const pqxx::undefined_table &e) {
        return result;
      }

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

      qcreate << "CREATE TABLE IF NOT EXISTS " << indice << " (timestamp INT, value REAL, spread REAL);";

      pqxx::work w(*dbh);
      pqxx::result res = w.exec(qcreate.str().c_str());
      w.commit();
      
      squery << "INSERT INTO " << indice << " VALUES " << records2sql(recs) << ";";
      pqxx::work w2(*dbh);
      res = w2.exec(squery.str().c_str());
      w2.commit();

      return 0;
    }

    //OK
    virtual int store(string indice, quotek::data::record& rec) {

      std::ostringstream qcreate;
      std::ostringstream squery;
      string outp;

      qcreate << "CREATE TABLE IF NOT EXISTS " << indice << " (timestamp INT, value REAL, spread REAL);";

      pqxx::work w(*dbh);
      pqxx::result res = w.exec(qcreate.str().c_str());
      w.commit();
      
      squery << "INSERT INTO " << indice << " VALUES " << record2sql(rec) << ";";
      pqxx::work w2(*dbh);
      try {
        res = w2.exec(squery.str().c_str());
        w2.commit();
      }

      catch(const pqxx::undefined_column &e) {
        std::cerr << "Warning: record has issue for storage" << std::endl;
        return 0;
      }


      return 0;
    }

    //OK
    virtual int saveHistory(quotek::core::position& pos)  {

      std::ostringstream squery;
      string outp;
      
      squery << "INSERT INTO __history__ " 
      << "(timestamp,indice,epic,dealid,size,stop,\"limit\",open,pnl,pnl_peak,open_date,close_date,identifier)"
      << " VALUES " << pos2sql(pos) << ";"; 

      pqxx::work w(*dbh);
      pqxx::result res = w.exec(squery.str().c_str());
      w.commit();

      return 0;

    }
    
    //OK
    virtual int saveHistory(quotek::data::cvector<quotek::core::position>& plist) {

      std::ostringstream squery;
      string outp;
      
      squery << "INSERT INTO __history__ "
      << "(timestamp,indice,epic,dealid,size,stop,\"limit\",open,pnl,pnl_peak,open_date,close_date,identifier)"
      << " VALUES " << poslist2sql(plist) << ";"; 

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
    qcreate << "timestamp INT,";  
    qcreate << "indice VARCHAR(64),";
    qcreate << "epic VARCHAR(64),";
    qcreate << "dealid VARCHAR(64),";
    qcreate << "size INT,";
    qcreate << "stop real,";
    qcreate << "\"limit\" real,";
    qcreate << "open real,";
    qcreate << "pnl real,";
    qcreate << "pnl_peak real,";
    qcreate << "open_date INT,";
    qcreate << "close_date INT,";
    qcreate << "identifier VARCHAR(64));";

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
  
    int tstamp = time(NULL);

    std::ostringstream sqstream;
    sqstream << "(" << tstamp <<  ","
            << "'" << pos.asset_name << "', "
            << "'" << pos.asset_id << "', "
            << "'" << pos.ticket_id << "', "
            << pos.size << ", " << pos.stop << ", "
            << pos.limit << ", " << pos.open << ", "
            << pos.pnl << ", " << pos.stats->pnl_peak << ", "
            << pos.open_date << ", " << pos.close_date <<  ", " 
            << "'" << pos.identifier << "'" << ")";

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
