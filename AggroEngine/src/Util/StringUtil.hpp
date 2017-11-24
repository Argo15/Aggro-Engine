#pragma once

#include <string>
#include "Matrix.hpp"
using namespace std;

/**
* Helpful string functions
*
* author: wcrane
* since: 2016-02-15
**/
class StringUtil
{
public:
	static string intToString(int value);
	static string vec3ToString(glm::vec3 vector);
};