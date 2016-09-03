/*
Quotek Strategies SDK 3.0
Copyright 2013-2016 Quotek SAS
*/

#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <iterator> 

#ifdef _WIN64
  #include <functionnal>
  #include <cctype>
  #include <locale>
#endif

namespace quotek {

  namespace core {
      
    /**
     * This namespace contains conveniency and other useful functions for everyday C++ development.
     */

    namespace utils {

    
      std::vector<std::string> &split(const std::string &s, 
                               char delim, 
                               std::vector<std::string> &elems);

      /** Splits a string into multiple parts, given a delimiter character.
       *  @param s String to split.
       *  @param delim Delimiter character.
       *  @return a vector of strings containing the splitted parts of the string. 
       */
      std::vector<std::string> split(const std::string &s, 
                                     char delim);


      
      /** Tokenise a string by making use of C++ std::istream_iterator. 
       *  It is useful to split the words inside a sentence, for instance.
       *  @param s String to tokenise.
       *  @return a vector of strings containing the tokens. 
       */
      std::vector<std::string> tokenise(std::string const& s);


      /** Removes the non-alphanum characters at the left of a string.
       *  @param s String to left-trim.
       *  @return reference to the left-trimed string.
       */
      std::string &ltrim(std::string &s);

      /** Removes the non-alphanum characters at the right of a string.
       *  @param s String to right-trim.
       *  @return reference to the right-trimed string.
       */
      std::string &rtrim(std::string &s);


      /** Removes the non-alphanum characters at both ends of a string.
       *  @param s String to trim.
       *  @return reference to the trimed string.
       */
      std::string &trim(std::string &s);
      
      unsigned int randr(unsigned int min, unsigned int max);

      std::vector<int> range(int min, int max);  
      std::vector<int> range(int r);

    }
  }
}

#endif
