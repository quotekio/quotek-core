/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include "quandl.hpp"
#include <rapidjson/document.h>
#include <iostream>
#include <sstream>

namespace quotek {

  namespace datasource {
   
        std::string quandl::root_url = "https://www.quandl.com/api/v3/";

      	quandl::quandl(std::string api_key) {
          this->api_key = api_key;
        }

      	quandl::~quandl() {

        }

      	std::vector<quotek::datasource::quandl_db> quandl::get_databases(const int pgnum) {

          std::vector<quotek::datasource::quandl_db> result;

          quotek::http h1;
          std::stringstream query_url;
          std::string sep = "?";

          query_url << root_url << "/databases";

          if ( api_key != "" ) {
            query_url << sep << "api_key=" << api_key;
            sep = "&";
          }

          if ( pgnum > 0 ) query_url << sep << "page=" << pgnum;

          std::string res = h1.get(query_url.str());

          rapidjson::Document d;
          d.Parse<0>(res.c_str());

          if ( d.HasMember("databases") ) {

            for(int i=0;i< d["databases"].Size(); i++ ) {

              quotek::datasource::quandl_db d1;
              d1.id = d["databases"][i]["id"].GetInt();
              d1.name = d["databases"][i]["name"].GetString();
              d1.code = d["databases"][i]["database_code"].GetString();
              d1.description = d["databases"][i]["description"].GetString();
              d1.datasets = d["databases"][i]["datasets_count"].GetInt();

              result.emplace_back(d1);

            }

          }

          if (  d["meta"].HasMember("next_page")  ) {

            if ( ! d["meta"]["next_page"].IsNull() ) {
              std::vector<quotek::datasource::quandl_db> nvect = get_databases( d["meta"]["next_page"].GetInt() );
              result.insert(result.end(), nvect.begin(), nvect.end());
            }
          }
          return result;
        }

        quotek::datasource::quandl_meta quandl::get_metadata(std::string database,
                                                        std::string dataset) {

          quotek::datasource::quandl_meta meta1;

          quotek::http h1;
          std::stringstream query_url;
          std::string sep = "?";

          query_url << root_url << "/datasets/" << database << "/" << dataset << "/metadata.json";

          if ( api_key != "" ) {
            query_url << sep << "api_key=" << api_key;
            sep = "&";
          }

          std::string res = h1.get(query_url.str());

          rapidjson::Document d;
          d.Parse<0>(res.c_str());

          if ( d.HasMember("dataset") ) {

            meta1.id = d["dataset"]["id"].GetInt();
            meta1.name = d["dataset"]["name"].GetString();
            meta1.code = d["dataset"]["dataset_code"].GetString();
            meta1.description = d["dataset"]["description"].GetString();

            meta1.type = d["dataset"]["type"].GetString();
            meta1.frequency = d["dataset"]["frequency"].GetString();

            for ( int i=0;i<  d["dataset"]["column_names"].Size(); i++ ) {
              meta1.columns.emplace_back(d["dataset"]["column_names"][i].GetString());
            } 
          }
          return meta1;
        }

        std::string quandl::query(std::string database,
                              std::string dataset,
                              std::string format,
                              const std::string start_date ,
                              const std::string end_date ,
                              const std::string collapse ,
                              const std::string transform ,
                              const std::string sort_order ,
                              const int nb_rows,
                              const int column_num ,
                              const bool exclude_colnames ) {

          quotek::http h1;
          std::stringstream query_url;
          std::string sep = "?";

          query_url << root_url << "/datasets/" << database << "/" << dataset << "/data." << format ;

          if ( api_key != "") {
            query_url << sep << "api_key=" << api_key;
            sep = "&";
          }

          if ( start_date != "" ) {
            query_url << sep << "start_date=" << start_date;
            sep = "&";
          }

          if ( end_date != "" ) {
            query_url << sep << "end_date=" << end_date;
            sep = "&";
          }

          if ( collapse != "" ) {
            query_url << sep << "collapse=" << collapse;
            sep = "&";
          }

          if ( transform != "" ) {
            query_url << sep << "transform=" << transform;
            sep = "&";
          }

          if ( sort_order != "" ) {
            query_url << sep << "order=" << sort_order;
            sep = "&";
          }

          if ( nb_rows > 0 ) {
            query_url << sep << "rows=" << nb_rows;
            sep = "&";
          }

          if ( column_num >= 0 ) {
            query_url << sep << "column_index=" << column_num;
            sep = "&"; 
          }

          if ( exclude_colnames ) {
            query_url << sep << "exclude_column_names=true";
            sep = "&";
          }

          std::string res = h1.get(query_url.str());
          return res;

        }
  
        quotek::data::records quandl::query_prices(std::string database,
                                           std::string dataset,
                                           const std::string start_date ,
                                           const std::string end_date,
                                           const std::string collapse  ) {

          std::string resp = this->query(database,
                                         dataset,
                                         "csv",
                                         start_date,
                                         end_date,
                                         collapse,
                                         "",
                                         "asc");

          quotek::datasource::csv csv1("string://",',');

          csv1.from_string(resp);
          csv1.setOffset(1);
          csv1.setTimeColumn(0,"%Y-%m-%d");

          return csv1.import_records(0,4);

        }

       
        std::vector<quotek::data::history> quandl::query_fullprices(std::string database,
                                               std::string dataset,
                                               const std::string start_date ,
                                               const std::string end_date  ,
                                               const std::string collapse ) {

          std::string resp = this->query(database,
                                         dataset,
                                         "csv",
                                         start_date,
                                         end_date,
                                         collapse,
                                         "",
                                         "asc");

          quotek::datasource::csv csv1("string://",',');

          csv1.from_string(resp);
          csv1.setOffset(1);
          csv1.setTimeColumn(0,"%Y-%m-%d");
        
          return csv1.import_history(0,1,2,3,4);

        }

       std::vector<float> quandl::get_column(std::string database,
                                       std::string dataset,
                                       int column_num,
                                       const std::string start_date ,
                                       const std::string end_date ,
                                       const std::string collapse ) {

        std::vector<float> result;

        std::string resp = this->query(database,
                                         dataset,
                                         "csv",
                                         start_date,
                                         end_date,
                                         collapse,
                                         "",
                                         "asc",
                                         -1,
                                         column_num,
                                         true);

        std::vector<std::string> lines = quotek::core::utils::split(resp,'\n');

        for (int i=0;i<lines.size();i++) {
          std::vector<std::string> vals = quotek::core::utils::split(lines[i],',');
          result.emplace_back(  atof( vals[1].c_str() ) );
        }

        return result;

       }

  }
}
