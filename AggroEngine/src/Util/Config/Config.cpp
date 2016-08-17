#include "Config.hpp"

Config::Config(string sConfigFile)
{
	try 
	{
		m_properties = JSONFileParser(sConfigFile);
	}
	catch(const std::exception &ex)
    {
		cout << ex.what() << endl;
		shared_ptr<boost::property_tree::ptree> ptree(new boost::property_tree::ptree());
		m_properties = PropertyTreeParser(ptree);
    }
}

const Properties &Config::getProperties() const
{
	return m_properties;
}