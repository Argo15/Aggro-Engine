#pragma once

#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
using namespace std;

/**
 * Base class for parsing a property tree (ie XML, JSON)
 *
 * author: wcrane
 * since: 2013-10-06
**/
class PropertyTreeParser
{
protected:
	boost::property_tree::ptree pt;

public:
	PropertyTreeParser() : pt() {}
	PropertyTreeParser(shared_ptr<boost::property_tree::ptree> ptree);
	virtual ~PropertyTreeParser() {}; 

	// Primitive Accessors
	string getStringProperty(string sPropertyPath) const;
	int getIntProperty(string sPropertyPath) const;
	long getLongProperty(string sPropertyPath) const;
	float getFloatProperty(string sPropertyPath) const;
	double getDoubleProperty(string sPropertyPath) const;
	bool getBooleanProperty(string sPropertyPath) const;

	// Array Accessors
	vector<string> getStringArrayProperty(string sPropertyPath) const;
	vector<int> getIntArrayProperty(string sPropertyPath) const;
	vector<long> getLongArrayArrayProperty(string sPropertyPath) const;
	vector<float> getFloatArrayProperty(string sPropertyPath) const;
	vector<double> getDoubleArrayProperty(string sPropertyPath) const;
	vector<bool> getBooleanArrayProperty(string sPropertyPath) const;
};
