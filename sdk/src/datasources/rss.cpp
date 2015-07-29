/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#include "rss.hpp"
#include <rapidxml/rapidxml.hpp>
#include <iostream>

namespace quotek {

  namespace datasource {
   
   
      	rss::rss(std::string source) {
          this->source = source;
        }

      	rss::~rss() {

        }

      	std::vector<quotek::data::news> rss::fetch(int nb_news) {

          std::vector<quotek::data::news> result;

          quotek::http h1;
          std::string data = h1.get(this->source);

          rapidxml::xml_document<> doc;
          doc.parse<0>(&data[0]);

          rapidxml::xml_node<> *root_node = doc.first_node()->first_node("channel");

          int i = 0;
          for(rapidxml::xml_node<> *n_node=root_node->first_node("item"); n_node; n_node=n_node->next_sibling())
          {

             if (i > nb_news -1 ) break;
             i++;

             quotek::data::news n1;

             n1.date = 1;
             n1.title = n_node->first_node("title")->value(); 
             n1.content = n_node->first_node("description")->value();

             result.emplace_back(n1);

          }

          return result;

        }

  }
}
