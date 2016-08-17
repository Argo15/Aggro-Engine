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
};

static const shared_ptr<Config> gConfig = shared_ptr<Config>(new Config("Config/config.json"));