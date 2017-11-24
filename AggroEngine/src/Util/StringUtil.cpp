#include "StringUtil.hpp"

std::string StringUtil::intToString(int value)
{
	return std::to_string(value);
}

std::string StringUtil::vec3ToString(glm::vec3 vector)
{
	return "{" + std::to_string(vector.x) + ", " + std::to_string(vector.y) + ", " + std::to_string(vector.z) + "}";
}

