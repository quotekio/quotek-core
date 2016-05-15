/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
*/

#ifndef EMAIL_HPP
#define EMAIL_HPP

#include <regex>
#include <string>


namespace quotek {

  namespace data {

      /** email is a simple email parsing class, allowing to extract
       *  header and body informations from a raw email string. */
	  class email {

	    public:

            /** email class constructor 1. */
	    	email();

	    	/** email class constructor 2.
	    	 *  @param data raw email data string to parse. */
	    	email(std::string data);

            /** initiates the parsing of the email string. */
	    	int parse();

            /** initiates the parsing of the email string. 
             *  @param data data to parse. */
	    	int parse(std::string data);

            /** email class destructor */
	    	~email();

            /** stores the email sender information. */
	    	std::string from;
	    	/** stores the email recipient information. */
	    	std::string to;
	    	/** stores the email subject information. */
	    	std::string subject;
	    	/** stores the date at which email was received. */
	    	std::string date;
	    	/** stores the email's body. */
	    	std::string body;

	    	/** internal use variable which stores data to parse. */
	    	std::string data;

	  };

  }
}

#endif