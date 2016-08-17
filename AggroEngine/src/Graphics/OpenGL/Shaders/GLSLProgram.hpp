#pragma once

#include "../../Graphics.hpp"
#include "GLSLShader.hpp"
#include <string>
#include <map>
#include <vector>

/**
 * This GLSL shader class is based off the book "More OpenGL Game Programming."
 *
 * author: wcrane
 * since: 2013-12-28
**/
class GLSLProgram 
{
public:
	
	GLSLShader *vertex_, *fragment_;

	GLSLProgram();
	GLSLProgram(const std::string &shader, unsigned int shaderType=GL_VERTEX_SHADER_ARB);
	GLSLProgram(const std::string &vertexShader, const std::string &fragmentShader);
	~GLSLProgram();
  
	void attach(GLSLShader &shader);
	void attach(GLSLShader *shader);
  
	void detach(GLSLShader &shader);
	void detach(GLSLShader *shader);
	void link();
	void use() const;
	void disable() const;

	void sendUniform(const std::string &name, float x);
	void sendUniform(const std::string &name, float x, float y);
	void sendUniform(const std::string &name, float x, float y, float z);
	void sendUniform(const std::string &name, float x, float y, float z, float w);
	void sendUniform(const std::string &name, int x);
	void sendUniform(const std::string &name, int x, int y);
	void sendUniform(const std::string &name, int x, int y, int z);
	void sendUniform(const std::string &name, int x, int y, int z, int w);
	void sendUniform(const std::string &name, float *m, bool transp=false, int size=4);

	void sendUniform(GLuint location, float x);
	void sendUniform(GLuint location, float x, float y);
	void sendUniform(GLuint location, float x, float y, float z);
	void sendUniform(GLuint location, float x, float y, float z, float w);
	void sendUniform(GLuint location, int x);
	void sendUniform(GLuint location, int x, int y);
	void sendUniform(GLuint location, int x, int y, int z);
	void sendUniform(GLuint location, int x, int y, int z, int w);
	void sendUniform(GLuint location, float *m, bool transp=false, int size=4);

	GLuint getUniformLocation(const std::string &name) const;
	void setAttributeLocation(const std::string &name, GLuint location);
	GLuint getAttributeLocation(const std::string &name) const;

	void GetProgramLog(std::string &log) const;
	GLuint getHandle() const;
	void getParameter(GLenum param, GLint *data) const;
	void validate() const;
	bool IsValidProgram() const;
	void getAttributeInfo(GLuint location, std::string &name, GLenum &type, GLint &size) const;
	void getUniformInfo(GLuint location, std::string &name, GLenum &datatype, GLint &size) const;
	void getAttachedShaders(std::vector<GLuint> &shaderhandles);
	void getAttachedShaders(std::vector<GLSLShader> &shaders);

private:
	int getLoc(const std::string &name);
	
	GLuint handle_;
	typedef std::map<std::string,int> uniformmap;
	uniformmap uniforms_;	
};