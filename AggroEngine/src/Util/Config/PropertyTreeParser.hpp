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
	string getStringProperty(string sPropertyPath, string defaultStr = "") const;
	int getIntProperty(string sPropertyPath, int defaultInt = 0) const;
	long getLongProperty(string sPropertyPath, long defaultLong = 0) const;
	float getFloatProperty(string sPropertyPath, float defaultFloat = 0) const;
	double getDoubleProperty(string sPropertyPath, double defaultDouble = 0) const;
	bool getBooleanProperty(string sPropertyPath, bool defaultStr = false) const;

	// Array Accessors
	vector<string> getStringArrayProperty(string sPropertyPath) const;
	vector<int> getIntArrayProperty(string sPropertyPath) const;
	vector<long> getLongArrayArrayProperty(string sPropertyPath) const;
	vector<float> getFloatArrayProperty(string sPropertyPath) const;
	vector<double> getDoubleArrayProperty(string sPropertyPath) const;
	vector<bool> getBooleanArrayProperty(string sPropertyPath) const;
};
