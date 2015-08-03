/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
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

    records::~records() {}

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

        std::vector<quotek::data::record> exrecs = quotek::data::record::extract(this->data,i,recs_per_aggregate);

        if ( method == "close" ) {
          result.append(exrecs.back().timestamp, exrecs.back().value, 0);
        }

        else {

          float high = quotek::data::record::max(exrecs);
          float low = quotek::data::record::min(exrecs);
          float open = exrecs.front().value;
          float close = exrecs.back().value;
          long tstamp = exrecs.back().timestamp;
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

    std::vector<long>& records::get_timestamps() {
      return this->timestamps;
    }

    std::vector<float>& records::get_values() {
      return this->values;
    }

    std::vector<float>& records::get_spreads() {
      return this->spreads;
    }

    void records::append(long timestamp, float value) {
      this->timestamps.emplace_back(timestamp);
      this->values.emplace_back(value);
      this->spreads.emplace_back(0);
      this->data.emplace_back( record(timestamp , value, 0) );
    }

    void records::append(long timestamp, float value, float spread) {
      this->timestamps.emplace_back(timestamp);
      this->values.emplace_back(value);
      this->spreads.emplace_back(spread);
      this->data.emplace_back( record(timestamp , value, spread) ); 
    }

    void records::append(quotek::data::record& r) {
      this->timestamps.emplace_back(r.timestamp);
      this->values.emplace_back(r.value);
      this->spreads.emplace_back(r.spread);
      this->data.emplace_back(r); 
    }

    quotek::data::record& records::last() {
      return this->data.back();
    }

    size_t records::size() {
      return this->data.size();
    }


    record::record() {

    }

    record::record (long timestamp, float value, float spread) {
      this->timestamp = timestamp;
      this->value = value;
      this->spread = spread;
    }

    record::~record() {

    }

    int record::search(std::vector<record>& recs,
                       long search_timestamp) {
  
      int first = 0;
      int last = recs.size() -1;
      int middle = (first+last)/2;

      while( first <= last )
       {
          if ( recs[middle].timestamp < search_timestamp )
             first = middle + 1;    
          else if ( recs[middle].timestamp == search_timestamp ) 
          {
            return middle ;
          }
          else last = middle - 1;
          middle = (first + last)/2;
       }
      return -1;

    }

    std::vector<record> record::sample(std::vector<record>& recs,
                                       long time_inf,
                                       long time_sup) {

      if (time_inf < 0 ) time_inf = time(0) + time_inf;
      if (time_sup <= 0 ) time_sup = time(0) + time_sup;

      std::vector<record>::const_iterator first = recs.begin() + record::search(recs,time_inf);
      std::vector<record>::const_iterator last = recs.begin() + record::search(recs,time_sup);

      std::vector<record> new_sample(first, last);

      return new_sample;
      
    }
   
    std::vector<record> record::extract(std::vector<record>& recs, int start_offset, int size ) {

      std::vector<record>::const_iterator first = recs.begin() + start_offset;
      std::vector<record>::const_iterator last = recs.begin() + start_offset + size;

      if (last > recs.end() ) last = recs.end();


      std::vector<record> new_extract(first, last);
      return new_extract;

    }
    
    std::vector<record> record::time_as_value(std::vector<record>& recs) {

      std::vector<record> tav;
      for (int i=0;i<recs.size();i++) {
        tav.emplace_back( record(recs[i].timestamp, recs[i].timestamp,0) );
      }
      return tav;
    }

    std::vector<float> record::values_export(std::vector<record>& recs) {

      std::vector<float> result;

      for (int i=0;i<recs.size();i++) {
        result.emplace_back(recs[i].value);
      }
      return result;
    }

    std::vector<long> record::timestamps_export(std::vector<record>& recs) {

      std::vector<long> result;
      for (int i=0;i<recs.size();i++) {
        result.emplace_back(recs[i].timestamp);
      }
      return result;
    }

    std::vector<record> record::values_import(std::vector<float>& data_to_import) {
      std::vector<record> result;

      for (int i=0;i<data_to_import.size();i++) {
        result.emplace_back( record(0, data_to_import[i], 0) );
      }

      return result;
    }

    float record::min(std::vector<quotek::data::record>& recs) {

      float min = 300000000;
      float cur = 0;

      for(int i=0;i<recs.size();i++) {
         cur = recs[i].value;
         if ( cur < min ) {
           min = cur;
         }
      }
      return min;
    }

    float record::max(std::vector<quotek::data::record>& recs) {
      
      float max = -300000000;
      float cur = 0;

      for(int i=0;i<recs.size();i++) {
         cur = recs[i].value;
         if ( cur > max ) {
           max = cur;
         }
      }
      return max;
    }

  }
}