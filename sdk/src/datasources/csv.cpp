/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include "csv.hpp"

namespace quotek {

  namespace datasource {

    
    csv::csv(std::string source_file,
             char separator) {

      this->source_file = source_file;

      int spos = this->source_file.find("://");

      if ( spos == std::string::npos ) {
        std::cout << "WARNING: No file scheme provided, failing back to file://" << std::endl;
        this->scheme = "file://";
      }

      else {
        this->scheme = this->source_file.substr(0, spos+3);
        this->source_file = this->source_file.substr(spos+3);
      }

      this->separator = separator;

      this->offset = -1;
      this->dt_colnum = -1;
      this->dt_format = "";
      this->linepos = 0;

    }

    csv::~csv() {

    }

    void csv::setTimeColumn(int dt_colnum, std::string dt_format) {

      this->dt_colnum = dt_colnum;
      this->dt_format = dt_format;

    }
       
    void csv::setOffset(int offset) {
      this->offset = offset;
    }
        
    void csv::addFilter(std::string filter) {
      this->filters.emplace_back(filter);
      std::regex r(filter);
      this->compiled_filters.emplace_back(r);
    }

    void csv::clearFilters() {
      this->filters.clear();
      this->compiled_filters.clear();
    }

    std::vector<std::string> csv::getFilters() {
      return filters;
    }

       
    std::vector<quotek::data::record> csv::import_records(int nb_records, int value_colnum) {

      std::vector<quotek::data::record> result;

      std::ifstream* csvi;
      std::string line;
 
      this->linepos = 0;

      if (this->scheme == "http://") {
        csvi  = http_fetch();
      }

      else {
        csvi = new std::ifstream(this->source_file.c_str());
      }

      while(csvi->good() && result.size() < nb_records ){
        //line can be processed.
        if (this->readline(csvi,line)) {
  
          //splits and parses line
          std::vector<std::string> sline = quotek::core::utils::split(line,separator);

          if ( value_colnum >= sline.size()  ) {
            std::cout << "ERROR: line doesn't have enough columns to fetch value, skipping!" << std::endl; 
          }

          quotek::data::record r1;

          if ( this->dt_colnum != -1 ) {
            r1.timestamp = quotek::core::time::p_strptime( sline[dt_colnum], dt_format);
          }

          else r1.timestamp = 0;


          r1.value = atof(sline[value_colnum].c_str());
          r1.spread = 0;

          result.emplace_back(r1);     

        }
      }

      return result;
    }

    std::vector<quotek::data::history> csv::import_history(int nb_lines, 
                                                          int open_colnum, 
                                                          int close_colnum,
                                                          int low_colnum,
                                                          int high_colnum) {

      std::vector<quotek::data::history> result;

      std::ifstream* csvi;
      std::string line;
      
      this->linepos = 0;

      if (this->scheme == "http://") {
        csvi  = http_fetch();
      }

      else {
        csvi = new std::ifstream(this->source_file.c_str());
      }

      while(csvi->good() && result.size() < nb_lines ){
        //line can be processed.
        if (this->readline(csvi,line)) {
      
          //splits and parses line
          std::vector<std::string> sline = quotek::core::utils::split(line,separator);

          if ( open_colnum >= sline.size() || 
               close_colnum >= sline.size() ||
               high_colnum >= sline.size() ||
               low_colnum >= sline.size() ) {
            std::cout << "ERROR: line doesn't have enough columns, skipping!" << std::endl; 
          }

          quotek::data::history h1;

          if ( this->dt_colnum != -1 ) {
            h1.timestamp = quotek::core::time::p_strptime( sline[dt_colnum], dt_format);
          }

          else h1.timestamp = 0;

          h1.open = atof(sline[open_colnum].c_str());
          h1.close = atof(sline[close_colnum].c_str());
          h1.low = atof(sline[low_colnum].c_str());
          h1.high = atof(sline[high_colnum].c_str());

          result.emplace_back(h1);     

        }
      }

      return result;

    }

    std::ifstream* csv::http_fetch() {

      quotek::http h1;
      std::string csvcontent = h1.get(this->scheme + this->source_file) ;

      std::string output_path = std::string(TMPDIR) + "/blabla.csv";

      std::ofstream o1(output_path.c_str()) ;
      o1 << csvcontent;
      o1.close();

      std::ifstream* f1 = new std::ifstream(output_path.c_str());
      return f1;
    }

    bool csv::readline(std::ifstream* csvi, std::string& line) {

      getline(*csvi,line);

      std::cout << line << std::endl;

      this->linepos++;

      if ( this->linepos < this->offset ) return false;

      bool failed = false;
      for (int i=0;i< this->compiled_filters.size();i++) {
        if ( ! std::regex_match(line,this->compiled_filters[i]) ) {
          return false;
        }
      }
      return true;
    }
  }
}