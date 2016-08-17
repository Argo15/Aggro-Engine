#include "JSONFileParser.hpp"
#include <boost/property_tree/json_parser.hpp>

JSONFileParser::JSONFileParser(string sFilename)
	: PropertyTreeParser()
{
	ifstream jsonFile(sFilename);
	std::stringstream ss;
	ss << jsonFile.rdbuf();
	boost::property_tree::read_json(ss, pt);
}
