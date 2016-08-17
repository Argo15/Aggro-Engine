#pragma once

#include "GLSLProgram.hpp"
#include <boost/unordered_map.hpp>
#include <iostream>
using namespace std;


/**
 * Structure for fetching shaders, or creating one if it does not exist.
 *
 * author: wcrane
 * since: 2013-12-28
**/
class ShaderStore
{
private:
	boost::unordered_map<string, shared_ptr<GLSLProgram>> m_shaders;

public:
	ShaderStore();

	shared_ptr<GLSLProgram> getShader(string sVertexShaderPath, string sFragmentShaderPath);
};