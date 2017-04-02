#include "GLSLProgram.hpp"
#include "GLSLShader.hpp"
#include <string>
#include <algorithm>
#include <iostream>
using namespace std;

// Public part :

GLSLProgram::GLSLProgram()
  :vertex_(NULL), fragment_(NULL)
{
  handle_ = glCreateProgram();
}

GLSLProgram::GLSLProgram(const std::string &shader, unsigned int shaderType)
  :fragment_(NULL)
{
  handle_ = glCreateProgram();
  vertex_ = new GLSLShader(shader, shaderType);

  attach(vertex_);
  
  link();
}

GLSLProgram::GLSLProgram(const std::string &vertexShader, const std::string &fragmentShader)
  :vertex_(NULL), fragment_(NULL)
{
  handle_ = glCreateProgram();
  
  vertex_ = new GLSLShader(vertexShader, GL_VERTEX_SHADER_ARB);
  fragment_ = new GLSLShader(fragmentShader, GL_FRAGMENT_SHADER_ARB);
  
  attach(vertex_);
  attach(fragment_);

  link();
}

GLSLProgram::~GLSLProgram()
{
	if(vertex_)
	{
	    delete vertex_;
	}
	if(fragment_)
	{
		delete fragment_;
	}

	glDeleteProgram(handle_);
}

GLuint GLSLProgram::getHandle() const
{
	return handle_;
}

void GLSLProgram::GetProgramLog(std::string &log) const
{
	GLchar *debug;
	GLint debugLength;
	getParameter(GL_INFO_LOG_LENGTH, &debugLength);

	debug = new GLcharARB[debugLength];
	glGetProgramInfoLog(handle_, debugLength, &debugLength, debug);

	//cout << debug;
	log.append(debug,0,debugLength);
	delete [] debug;
}
void GLSLProgram::validate() const
{
	glValidateProgram(handle_);
}
bool GLSLProgram::IsValidProgram() const
{
	GLint status;
	getParameter(GL_VALIDATE_STATUS,&status);
	if(!status)
	{
		return false;
	}
	else
	{
		return true;
	}
}
void GLSLProgram::getParameter(GLenum param, GLint *data) const
{
	glGetProgramiv(handle_, param, data);
}

void GLSLProgram::attach(GLSLShader &shader)
{
	glAttachShader(handle_, shader.getHandle());
}

void GLSLProgram::attach(GLSLShader *shader)
{
	attach(*shader);
}

void GLSLProgram::detach(GLSLShader &shader)
{
	glDetachShader(handle_,shader.getHandle());
}

void GLSLProgram::detach(GLSLShader *shader)
{
	detach(*shader);
}

void GLSLProgram::link()
{
	glLinkProgram(handle_);
	uniforms_.clear();
}

void GLSLProgram::use() const
{
	glUseProgram(handle_);
}

void GLSLProgram::disable() const
{
	glUseProgram(0);
}

void GLSLProgram::sendUniform(const std::string &name, float x)
{
	glUniform1f(getLoc(name), x);
}

void GLSLProgram::sendUniform(const std::string &name, float x, float y)
{
	glUniform2f(getLoc(name), x, y);
}

void GLSLProgram::sendUniform(const std::string &name, float x, float y, float z)
{
	glUniform3f(getLoc(name), x, y, z);
}

void GLSLProgram::sendUniform(const std::string &name, float x, float y, float z,
			      float w)
{
	glUniform4f(getLoc(name), x, y, z, w);
}

void GLSLProgram::sendUniform(const std::string &name, int x)
{
	glUniform1i(getLoc(name), x);
}

void GLSLProgram::sendUniform(const std::string &name, int x, int y)
{
	glUniform2i(getLoc(name), x, y);
}

void GLSLProgram::sendUniform(const std::string &name, int x, int y, int z)
{
	glUniform3i(getLoc(name), x, y, z);
}

void GLSLProgram::sendUniform(const std::string &name, int x, int y, int z,
			      int w)
{
	glUniform4i(getLoc(name), x, y, z, w);
}

void GLSLProgram::sendUniform(const std::string &name, float *matrix, bool transpose,
			      int size)
{
	int loc = getLoc(name);
  
	switch(size)
    {
    case 2:
		glUniformMatrix2fv(loc, 1, transpose, matrix);
		break;
    case 3:
		glUniformMatrix3fv(loc, 1, transpose, matrix);
		break;
    case 4:
		glUniformMatrix4fv(loc, 1, transpose, matrix);
		break;
    }
}

// sendUniform functions with supplied location
void GLSLProgram::sendUniform(GLuint location, float x)
{
	glUniform1f(location, x);
}

void GLSLProgram::sendUniform(GLuint location, float x, float y)
{
	glUniform2f(location, x, y);
}

void GLSLProgram::sendUniform(GLuint location, float x, float y, float z)
{
	glUniform3f(location, x, y, z);
}

void GLSLProgram::sendUniform(GLuint location, float x, float y, float z,
							  float w)
{
	glUniform4f(location, x, y, z, w);
}

void GLSLProgram::sendUniform(GLuint location, int x)
{
	glUniform1i(location, x);
}

void GLSLProgram::sendUniform(GLuint location, int x, int y)
{
	glUniform2i(location, x, y);
}

void GLSLProgram::sendUniform(GLuint location, int x, int y, int z)
{
	glUniform3i(location, x, y, z);
}

void GLSLProgram::sendUniform(GLuint location, int x, int y, int z,
							  int w)
{
	glUniform4i(location, x, y, z, w);
}

void GLSLProgram::sendUniform(GLuint location, float *matrix, bool transpose,
							  int size)
{
	switch(size)
	{
	case '2':
		glUniformMatrix2fv(location, 0, transpose, matrix);
		break;
	case '3':
		glUniformMatrix3fv(location, 0, transpose, matrix);
		break;
	case '4':
		glUniformMatrix4fv(location, 0, transpose, matrix);
		break;
	}
}

GLuint GLSLProgram::getUniformLocation(const std::string &name) const
{
	return glGetUniformLocation(handle_,name.c_str());
}

void GLSLProgram::setAttributeLocation(const std::string &name, GLuint location)
{
	glBindAttribLocation(handle_,location, name.c_str());
}

GLuint GLSLProgram::getAttributeLocation(const std::string &name) const
{
	return glGetAttribLocation(handle_,name.c_str());	
}

void GLSLProgram::getAttributeInfo(GLuint location, std::string &name, GLenum &datatype, GLint &size) const
{
	// first we need to query the length of the longest name
	GLsizei length;
	getParameter(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,(GLint*)&length);

	std::vector<char> nametmp(length);
	GLsizei copiedsize;
	glGetActiveAttrib(handle_,location,length,&copiedsize,&size,&datatype,&nametmp[0]);

	if(0 == copiedsize)
	{
		return;
	}

	name.assign(&nametmp[0]);
}

void GLSLProgram::getUniformInfo(GLuint location, std::string &name, GLenum &datatype, GLint &size) const
{
	// first we need to query the length of the longest name
	GLint length;
	getParameter(GL_ACTIVE_UNIFORM_MAX_LENGTH,&length);

	std::vector<char> nametmp(length);
	GLsizei copiedsize;
	glGetActiveUniform(handle_,location,length,&copiedsize,&size,&datatype,&nametmp[0]);

	if(0 == copiedsize)
	{
		return;
	}

	name.assign(&nametmp[0]);
}

void GLSLProgram::getAttachedShaders(std::vector<GLuint> &shaderhandles)
{
	GLint number;
	getParameter(GL_ATTACHED_SHADERS,&number);

	shaderhandles.resize(number);
	glGetAttachedShaders(handle_,number,NULL, &shaderhandles[0]);
}

namespace {

	struct shaderconstuctor
	{
		std::vector<GLSLShader> &shaders_;
		shaderconstuctor(std::vector<GLSLShader> &shaders) : shaders_(shaders)
		{
		}
		void operator()(std::vector<GLuint>::value_type data)
		{
			shaders_.push_back(GLSLShader(data));
		}
	};
}
void GLSLProgram::getAttachedShaders(std::vector<GLSLShader> &shaders)
{
	std::vector<GLuint> shaderhandles;
	getAttachedShaders(shaderhandles);

	std::for_each(shaderhandles.begin(),shaderhandles.end(),shaderconstuctor(shaders));
}

// Private part :

int GLSLProgram::getLoc(const std::string &name)
{
	int loc = -1;
	uniformmap::const_iterator it = uniforms_.find(name);
	if(it == uniforms_.end())
	{
		loc = glGetUniformLocation(handle_, name.c_str());
		if(loc == -1)
		{
			std::string s;
			s.append(name);
			s.append(" - is not a valid uniform variable name");
			cout << s << endl;
		}
		uniforms_.insert(uniformmap::value_type(name,loc));
	}
	else
	{
		loc = it->second;
	}

	return loc;
}
