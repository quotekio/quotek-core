/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include "record.hpp"

namespace quotek {
  namespace data {

    records::records() {}

    records::records(std::vector<std::tuple<long,float,float>>& data) {

      for (int i=0;i<data.size();i++) {
        
        this->append(std::get<0>(data[i]), std::get<1>(data[i]), std::get<2>(data[i]) );
      }

    }


    records::records(std::vector<float>& data) {

      for (int i=0;i<data.size();i++) { 
        this->append(i+1,data[i],0);
      }
    }

    records::records(std::vector<quotek::data::record>& data) {
      for (int i=0;i<data.size();i++) { 
        this->append(data[i]);
      }
    }


    records::~records() {}


    int records::search(long search_timestamp) {
    
      int first = 0;
      int last = data.size() -1;
      int middle = (first+last)/2;

      while( first <= last )
       {
          if ( data[middle].timestamp < search_timestamp )
             first = middle + 1;    
          else if ( data[middle].timestamp == search_timestamp ) 
          {
            return middle ;
          }
          else last = middle - 1;
          middle = (first + last)/2;
       }
      return -1;

    }

    quotek::data::records records::sample(long time_inf,
                                         long time_sup) {

      if (time_inf < 0 ) time_inf = time(0) + time_inf;
      if (time_sup <= 0 ) time_sup = time(0) + time_sup;

      std::vector<record>::const_iterator first = data.begin() + search(time_inf);
      std::vector<record>::const_iterator last = data.begin() + search(time_sup);

      std::vector<record> new_sample(first, last);

      return quotek::data::records(new_sample);
      
    }
    

    quotek::data::records records::extract(int start_offset, int size ) {

      std::vector<record>::const_iterator first = data.begin() + start_offset;
      std::vector<record>::const_iterator last = data.begin() + start_offset + size;

      if (last > data.end() ) last = data.end();

      std::vector<record> new_extract(first, last);
      return quotek::data::records(new_extract);

    }

    quotek::data::records records::extract(int n) {

      //Err, extraction size > data size.
      if ( n > data.size()  ) return quotek::data::records() ;

      std::vector<record>::const_iterator first = data.end() - n ;
      std::vector<record>::const_iterator last = data.end();

      std::vector<record> new_extract(first, last);
      return quotek::data::records(new_extract);

    }



    quotek::data::records records::down_sample(int period, 
                                     float tick, 
                                     std::string method) {

      quotek::data::records result;

      int recs_per_aggregate = period / tick ;

      // there was an issue here, no aggregate
      if ( recs_per_aggregate == 0 ) return result;

      // issue, invalid method
      if ( method != "close" && 
           method != "HL2" && 
           method != "typical" && 
           method != "OHLC4" ) {
        return result;
      }

      for (int i=0;i<this->data.size();i+=recs_per_aggregate) {

       quotek::data::records exrecs = extract(i,recs_per_aggregate);

        if ( method == "close" ) {
          result.append(exrecs.get_data().back().timestamp, exrecs.get_data().back().value, 0);
        }

        else {

          float high = exrecs.max().value;
          float low = exrecs.min().value;
          float open = exrecs.get_data().front().value;
          float close = exrecs.get_data().back().value;
          long tstamp = exrecs.get_data().back().timestamp;
          float val = 0;

          if (method == "HL2") val = (low + high) / 2.0 ;
          else if (method == "typical") val = (low + high + close) /3.0;
          else if (method == "OHLC4") val = ( low + high + close + open ) / 4.0;
          result.append(tstamp, val,0);

        }

      }
      return result;
    }

    std::vector<quotek::data::record>& records::get_data() {
      return this->data;
    }

    std::vector<long> records::export_timestamps() {
      std::vector<long> tstamps;
      for (int i=0;i<data.size();i++) {
        tstamps.emplace_back(data[i].timestamp);
      }
      return tstamps;
    }

    std::vector<float> records::export_values() {
      std::vector<float> values;
      for (int i=0;i<data.size();i++) {
        values.emplace_back(data[i].value);
      }
      return values;
    }

    std::vector<float> records::export_spreads() {
      std::vector<float> spreads;
      for (int i=0;i<data.size();i++) {
        spreads.emplace_back(data[i].spread);
      }
      return spreads;
    }

    void records::append(long timestamp, float value) {
      this->data.emplace_back( record(timestamp , value, 0) );
    }

    void records::append(long timestamp, float value, float spread) {
      this->data.emplace_back( record(timestamp , value, spread) ); 
    }

    void records::append(float value) {
      this->data.emplace_back( record(0, value,0) );

    }

    void records::append(quotek::data::record& r) {
      this->data.emplace_back(r); 
    }


    quotek::data::record& records::last() {
      return this->data.back();
    }

    size_t records::size() {
      return this->data.size();
    }

    quotek::data::record records::min(){

      if ( data.size() == 0 ) return quotek::data::record(0,0,0);

      float mval = data[0].value;
      float rec_idx = 0;

      for (int i=1;i<data.size();i++) {

        if ( data[i].value < mval ) {
          mval = data[i].value;
          rec_idx = i;
        }

      }
      return data[rec_idx];
    }


    std::string records::str(const bool add_timestamps = true) {

      std::stringstream ss;
      ss << "[";
      for (int i=0;i< data.size();i++ ){
        ss << data[i].str(add_timestamps);
        if (i < data.size() -1  ) ss << ",";
      }
      ss << "]";

      return ss.str();
    }

    quotek::data::record records::max() {

      if ( data.size() == 0 ) return quotek::data::record(0,0,0);

      float mval = data[0].value;
      float rec_idx = 0;

      for (int i=1;i<data.size();i++) {

        if ( data[i].value > mval ) {
          mval = data[i].value;
          rec_idx = i;
        }

      }
      return data[rec_idx];

    }

    record::record() {

    }

    record::record (long timestamp, float value, float spread) {
      this->timestamp = timestamp;
      this->value = value;
      this->spread = spread;
    }

    std::string record::str(const bool add_timestamps = true) {

      std::stringstream ss;
      if (add_timestamps == true) {
        ss << "[" << this->timestamp << "," << this->value << "," << this->spread << "]";
      }
      else ss << "[" <<  this->value << "," << this->spread << "]";       

      return ss.str();

    }

    

    record::~record() {

    }
    
  }
}