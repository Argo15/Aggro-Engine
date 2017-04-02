#include "ShaderStore.hpp"

ShaderStore::ShaderStore()
{

}

shared_ptr<GLSLProgram> ShaderStore::getShader(string sVertexShaderPath, string sFragmentShaderPath)
{
	string sKey = sVertexShaderPath + sFragmentShaderPath;
	if (m_shaders.find(sKey) == m_shaders.end())
	{
		shared_ptr<GLSLProgram> pProgram(shared_ptr<GLSLProgram>(new GLSLProgram(sVertexShaderPath, sFragmentShaderPath)));
		
		string sLog;
		if (!pProgram->vertex_->isCompiled())
		{
			pProgram->vertex_->getShaderLog(sLog);
			cout << sVertexShaderPath << ": Vertex shader failed to compile: " << sLog << endl;
		}
		if (!pProgram->fragment_->isCompiled())
		{	
			pProgram->fragment_->getShaderLog(sLog);
			cout << sFragmentShaderPath << ": Fragment shader failed to compile " << sLog << endl;
		}
		m_shaders[sKey] = pProgram;
	}
	
	return m_shaders[sKey];
}