#include "PropertyTreeParser.hpp"
#include <boost/foreach.hpp>

PropertyTreeParser::PropertyTreeParser(shared_ptr<boost::property_tree::ptree> ptree)
	: pt(*ptree)
{

}

string PropertyTreeParser::getStringProperty(string sPropertyPath) const
{
	return pt.get<string>(sPropertyPath,"");
}

int PropertyTreeParser::getIntProperty(string sPropertyPath) const
{
	return pt.get<int>(sPropertyPath,0);
}

long PropertyTreeParser::getLongProperty(string sPropertyPath) const
{
	return pt.get<long>(sPropertyPath,0);
}

float PropertyTreeParser::getFloatProperty(string sPropertyPath) const
{
	return pt.get<float>(sPropertyPath,0);
}

double PropertyTreeParser::getDoubleProperty(string sPropertyPath) const
{
	return pt.get<double>(sPropertyPath,0);
}

bool PropertyTreeParser::getBooleanProperty(string sPropertyPath) const
{
	return pt.get<bool>(sPropertyPath,false);
}

vector<string> PropertyTreeParser::getStringArrayProperty(string sPropertyPath) const
{
	vector<string> propertyArray;
	BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, pt.get_child(sPropertyPath))
		propertyArray.push_back(v.second.get_value<string>());
	return propertyArray;
}

vector<int> PropertyTreeParser::getIntArrayProperty(string sPropertyPath) const
{
	vector<int> propertyArray;
	BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, pt.get_child(sPropertyPath))
		propertyArray.push_back(v.second.get_value<int>());
	return propertyArray;
}

vector<long> PropertyTreeParser::getLongArrayArrayProperty(string sPropertyPath) const
{
	vector<long> propertyArray;
	BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, pt.get_child(sPropertyPath))
		propertyArray.push_back(v.second.get_value<long>());
	return propertyArray;
}

vector<float> PropertyTreeParser::getFloatArrayProperty(string sPropertyPath) const
{
	vector<float> propertyArray;
	BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, pt.get_child(sPropertyPath))
		propertyArray.push_back(v.second.get_value<float>());
	return propertyArray;
}

vector<double> PropertyTreeParser::getDoubleArrayProperty(string sPropertyPath) const
{
	vector<double> propertyArray;
	BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, pt.get_child(sPropertyPath))
		propertyArray.push_back(v.second.get_value<double>());
	return propertyArray;
}

vector<bool> PropertyTreeParser::getBooleanArrayProperty(string sPropertyPath) const
{
	vector<bool> propertyArray;
	BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, pt.get_child(sPropertyPath))
		propertyArray.push_back(v.second.get_value<bool>());
	return propertyArray;
}