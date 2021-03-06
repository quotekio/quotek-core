#ifndef UTILS_H
#define UTILS_H

#include  <vector>
#include <string>
#include <iostream>
#include <sstream>

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <regex>
#include <iterator>


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

std::string &ltrim(std::string &s);
std::string &rtrim(std::string &s);
std::string &trim(std::string &s);

bool endswith(const std::string &value, const std::string &ending);


uint64_t getMsTimeStamp();
std::string randstring(int len);
std::string sreplace(std::string &s,
                      std::string toReplace,
                      std::string replaceWith);

void sreplaceAll(std::string&, 
	             const std::string&,
                 const std::string&);

std::string &upper(std::string &s);

std::string currentDateTime();
std::string epochToDateTime(time_t t);
std::string float2string(float);
std::string int2string(int);
bool contains(std::vector<std::string>,std::string);

std::string strtolower(std::string);


std::string stripHTML(std::string);
std::string stripTags(std::string);

time_t str2time(std::string s);
time_t str2time2(std::string s);

bool vector_in(std::vector<int>*,int);
bool vector_in_str(std::vector<std::string>*,std::string);

std::string json_escape(const std::string& input);

float randfloat(float min, float max);
int randint(int min,int max);

int roundint(double);
float roundfloat(float f,float prec);


#endif