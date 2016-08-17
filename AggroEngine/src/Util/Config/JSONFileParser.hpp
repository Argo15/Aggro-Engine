#pragma once

#include <string>
#include "PropertyTreeParser.hpp"
using namespace std;

/**
 * Class for parsing JSON files
 *
 * author: wcrane
 * since: 2013-10-06
**/
class JSONFileParser : public PropertyTreeParser
{
public:
	JSONFileParser(string sFilename);
	~JSONFileParser() {}
};