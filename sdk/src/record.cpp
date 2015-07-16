/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#include "record.hpp"

namespace quotek {
  namespace data {

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

      std::vector<record> new_extract(first, last);
      return new_extract;

    }

    std::vector<float> record::values_export(std::vector<record>& recs) {

      std::vector<float> result;

      for (int i=0;i<recs.size();i++) {
        result.emplace_back(recs[i].value);
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

  }
}