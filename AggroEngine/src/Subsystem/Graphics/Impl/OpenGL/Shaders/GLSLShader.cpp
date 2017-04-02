#include "GLSLShader.hpp"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

GLSLShader::GLSLShader(const std::string &filename, GLenum shaderType) : compiled_(false)
{
  //char *source;
  handle_ = glCreateShader(shaderType);
  
  const char * source = readShader(filename);
  glShaderSource(handle_, 1, static_cast<const GLchar**>(&source), NULL);
  
  compile();
  
  delete [] source;
}

GLSLShader::GLSLShader(GLenum shaderType)
{
	
	if(shaderType == GL_VERTEX_SHADER || shaderType == GL_FRAGMENT_SHADER)
	{
		handle_ = glCreateShader(shaderType);
	}
	else
	{
		handle_ = shaderType;
	}
}


GLSLShader::~GLSLShader()
{
	glDeleteShader(handle_);
}

void GLSLShader::compile()
{
  GLint compiled;
  
  glCompileShader(handle_);
  getParameter(GL_COMPILE_STATUS, &compiled);
  if(!compiled)
  {
	  compiled_ = false;
  }
  else
  {
	  compiled_ = true;
  }
}
bool GLSLShader::isCompiled() const
{
	return compiled_;
}
void GLSLShader::getShaderLog(std::string &log) const
{
	GLchar *debug;
	GLint debugLength;
	getParameter(GL_INFO_LOG_LENGTH, &debugLength);
      
	debug = new GLchar[debugLength];
	glGetShaderInfoLog(handle_, debugLength, &debugLength, debug);
		
    //cout << debug;
	log.append(debug,0,debugLength);
    delete [] debug;
}
void GLSLShader::getShaderSource(std::string &shader) const
{
	GLchar *source;
	GLint sourcelen;
	getParameter(GL_SHADER_SOURCE_LENGTH, &sourcelen);

	source = new GLchar[sourcelen];
	glGetShaderSource(handle_,sourcelen, &sourcelen, source);
	shader.append(source,0,sourcelen);
	delete []source;
}
GLuint GLSLShader::getHandle() const
{
	return handle_;
}

void GLSLShader::getParameter(GLenum param, GLint *data) const
{
	glGetShaderiv(handle_, param, data);
}

void GLSLShader::setShaderSource(std::string &code)
{
	const char *source = code.c_str();
	glShaderSource(handle_,1,static_cast<const GLchar**>(&source),NULL);
}

// Private part :

char *GLSLShader::readShader(const std::string &filename)
{
  ifstream temp(filename.c_str());
  int count = 0;
  char *buf;
  
  temp.seekg(0, ios::end);
  count = temp.tellg();
  
  buf = new char[count + 1];
  memset(buf,0,count);
  temp.seekg(0, ios::beg);
  temp.read(buf, count);
  buf[count] = 0;
  temp.close();
  
  return buf;
}
