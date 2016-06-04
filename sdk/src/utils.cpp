/*
Quotek Strategies SDK 3.0
Copyright 2013-2016 Quotek SAS
*/

#include "utils.hpp"

namespace quotek {

  namespace core {

    namespace utils {

      std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
          std::stringstream ss(s);
          std::string item;
          while (std::getline(ss, item, delim)) {
              elems.push_back(item);
          }
          return elems;
      }

      std::vector<std::string> tokenise(std::string const& input) {
        std::istringstream str(input);
        std::istream_iterator<std::string> cur(str), end;
        return std::vector<std::string>(cur, end);

      }

      std::vector<std::string> split(const std::string &s, char delim) {
          std::vector<std::string> elems;
          split(s, delim, elems);
          return elems;
      }

      std::string &ltrim(std::string &s) {
              s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
              return s;
      }

      std::string &rtrim(std::string &s) {
              s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
              return s;
      }

      std::string &trim(std::string &s) {
              return ltrim(rtrim(s));
      }

      unsigned int randr(unsigned int min, unsigned int max) {
             double scaled = (double)rand()/RAND_MAX;
             return (max - min +1)*scaled + min;
      }

      std::vector<int> range(int min, int max) {

        std::vector<int> result;

        for (int i=min;i < max;i++) {
          result.emplace_back(i);
        }
        return result;
      }
      
      std::vector<int> range(int r) {
        return range(0,r);
      }


    }
  }
}
