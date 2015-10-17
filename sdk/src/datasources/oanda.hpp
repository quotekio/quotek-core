/*
Quotek Strategies SDK 2.2
Copyright 2013-2015 Quotek SAS
http://www.quotek.io

OANDA module is an experimental set of functions, therefore they are not officially 
supported nor undocumented.
Use them at your very own risks.
*/

#ifndef OANDA_HPP
#define OANDA_HPP

#include "../datasource.hpp"
#include "../utils.hpp"
#include <map>
#include <vector>

namespace quotek {

  namespace datasource {
		class oanda: public datasource {

		  public:

		  	oanda();
		  	~oanda();
		  	std::map<std::string, std::vector<float> > get_fxratios();
		  	std::vector<float> get_ratio(std::string fx_pair);

		};

  }
}

#endif