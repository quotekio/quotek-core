/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#ifndef CSV_HPP
#define CSV_HPP

#include "../record.hpp"
#include "../datasource.hpp"

#include <string>

namespace quotek {

  namespace datasource {

    /**
     * csv class allows to fetch values (specially time-series) from a csv file. */

    class csv: public datasource_value {

      public:

        /** csv class constructor.
         *  @param source csv source file parameter, see datasource class documentation for more details.
         */
      	csv(std::string source);

      	/** csv class destructor.
      	 */
      	~csv();

        /** reads data inside csv and puts it inside a quotek records container. */
      	quotek::data::records query();

      	/** */
      	std::map<std::string, quotek::data::records> query_all();



      private:

      	void retrieve();






    };


  }
}

#endif
