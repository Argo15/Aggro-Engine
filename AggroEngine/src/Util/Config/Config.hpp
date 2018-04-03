#pragma once

#include <memory>
#include <iostream>
#include "JSONFileParser.hpp"

typedef PropertyTreeParser Properties;

/**
 * Holds config properties stored in config.json
 *
 * author: wcrane
 * since: 2013-11-03
**/
class Config
{
private:
	Properties m_properties;

public:
	Config(string sConfigFile);
	~Config() {}

	const Properties &getProperties() const;

	static Config& instance()
	{
		static Config INSTANCE("Config/config.json");
		return INSTANCE;
	}
};