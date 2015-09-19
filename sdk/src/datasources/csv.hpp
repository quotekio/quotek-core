/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#ifndef CSV_HPP
#define CSV_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "../constants.hpp"
#include "../datasource.hpp"
#include "../http.hpp"
#include "../record.hpp"
#include "../history.hpp"
#include "../timeutils.hpp"
#include "../utils.hpp"

namespace quotek {

  namespace datasource {

    /** csv is a class that helps importing financial data stored in CSV files.
     *  it has multiple import features depending on what user wants to achieve.
     */
    class csv: public datasource {

      public:

        /** csv class constructor .
         *  @param source_file: Source of the CSV file, under the form scheme://path_to_csv. Scheme can be either:
         *  *file:// , for local csv files.
         *  *http:// , for remote, csv files that can be fetched over http.
         *  @param separator: string that tells what is the columns separator in the csv.
         */
      	csv(std::string source_file,
            char separator);

      	/** rss class destructor */
      	~csv();

        /**
         * This function allows to tell which column in the csv contains the date/time data (if one exists),
         * and which format it is. The format follows the usual UNIX date() format expression.
         * @param dt_colnum: integer telling which column in the csv contains datatime data. It is zero-starting, meaning that first column is 0, second is 1, etc..
         * @param dt_format: date string format, essential for date parsing. For example "%Y-%m-%d %H:%M:%S" as date format means the csv contains dates like "2015-09-15 16:00:00".
         * If the time format in the CSV is an epoch timestamp, just put "%s" in it.
         */
        void setTimeColumn(int dt_colnum, std::string dt_format);

        /**
         * Sets a line offset at which to start importing data.
         * Can be useful for very large CSV files for which user only wants a part.
         * @param offset: integer representing the line of the csv file to start at.
         */
        void setOffset(int offset);
        
        /** Adds a new line filter to the instanciated csv object.
         *  A line filter allows to take only the CSV lines that match the filter.
         *  filters must be provided as regular expression strings for later compilation.
         *  See the c++ <regex> documentation for further infos about how to use them.
         *  @param filter:  regex string to provide for filtering.
         *  
         *  Note about the filters list: filters add each other (boolean AND), so if you want to 
         *  perform OR operations, you will have to put them in a single regex.
         */

        void addFilter(std::string filter);

        /** clears all the previously added filters of the instanciated csv object. */
        void clearFilters();

        /** This function returns the list of CSV filters of the instanciated csv object.
         *  @return list of the csv filters, with the form of a vector.
         */
        std::vector<std::string> getFilters();

        /** Imports data in the source csv as a vector of quotek records.
         *  @param nb_records: Sets the maximum size of lines to import, if user wants to import only a part of the data nb_records = 0 means import all.
      	 *  @param value_colnum: Tells which column to take record value from. It is zero-starting, meaning that first column is 0, second is 1, etc..
         *  @return a vector of quotek::data::record objects.
         */
      	std::vector<quotek::data::record> import_records(int nb_records, int value_colnum);

        /** Imports data in the source csv as a vector of quotek history objects.
         *  @param nb_lines: maximum number of history lines the user wants to import. 0 means import all.
         *  @param open_column: zero-starting index of the column containing the open value.
         *  @param close_colnum: zero-starting index of the column containing the close value.
         *  @param low_colnum: zero-starting index of the column containing the low value.
         *  @param high_colnum: zero-starting index of the column containing the high value.
         *  @return a vector of quotek::data::history objects.
         */
        std::vector<quotek::data::history> import_history(int nb_lines, 
                                                          int open_colnum, 
                                                          int close_colnum,
                                                          int low_colnum,
                                                          int high_colnum);

    private:

      std::ifstream* http_fetch();
      bool readline(std::ifstream* csvi, std::string& line);

      std::string scheme;
      std::string source_file;
      char separator;
      std::vector<std::string> filters;
      std::vector<std::regex> compiled_filters;

      int offset;
      int dt_colnum;
      std::string dt_format;

      int linepos;
    };

  }
}

#endif
