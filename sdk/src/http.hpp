/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#ifndef HTTP_HPP
#define HTTP_HPP

#include <curl/curl.h>
#include <stdlib.h>
#include <string>
#include <map>

namespace quotek {

  /** HTTP is a simple HTTP client that will allow your strategies to remotely fetch data.
   *  Also note that this class supports https. 
   */
  class http {

    public:
      /** http class constructor. */
  	  http();
      
      /** http class destructor. */
  	  ~http();

      /** alternative destructor. */
      void destroy();

      /** Performs an HTTP GET QUERY on provided URL and returns result as a string.
       *  @param url HTTP URL to perform query on.
       *  @return the HTTP server response, as a string.
       */
  	  std::string get(std::string url);

      /** Performs an HTTP GET QUERY on provided URL with provided params, and returns result as a string.
       *  @param url HTTP URL to perform query on.
       *  @param get_params key/values map corresponding to paramter/value.
       *  @return the HTTP server response, as a string.
       */
      std::string get(std::string url, std::map<std::string, std::string>& get_params);
      

      /** Performs an HTTP POST query on provided URL and returns result as a string.
       *  @param url HTTP URL to perform query on.
       *  @param post_data POST data string having the form "field1=data2&field2=data2..&fieldn=datan".
       *  @return the HTTP server response, as a string.
       */
  	  std::string post(std::string url, std::string& post_data);

      /** Performs an HTTP POST query on provided URL and returns result as a string.
       *  @param url HTTP URL to perform query on.
       *  @param post_data std::map containing the post data (eg post_data["name"] = "john" will send "name=john" as POST data).
       *  @return the HTTP server response, as a string.
       */
  	  std::string post(std::string url,std::map<std::string, std::string>& post_data);

      /** Performs an HTTP DELETE QUERY on provided URL and returns result as a string.
       *  @param url HTTP URL to perform query on.
       *  @return the HTTP server response, as a string.
       */
  	  std::string del(std::string url);

      /** Adds an header value prior to an HTTP request.
       *  @param header: header data to add. Header string is usually of the form: "Header_name: Value".
       */
      void add_header(std::string header);

      /**
       * Escapes characters in a string to pass it as a valid URL/ post data.
       * @param to_escape String to escape.
       * @return HTTP Escaped string.
       */
      std::string escape(std::string to_escape);

    private:
      
      /** Internal curl object handler. */
      CURL* ch;

      /** Internal headers list. */
      struct curl_slist *headers;
  };
}

#endif