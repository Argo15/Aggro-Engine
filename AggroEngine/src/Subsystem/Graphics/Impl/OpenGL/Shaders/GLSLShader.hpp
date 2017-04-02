#pragma once

#include <string>
#include "Graphics.hpp"

/**
 * This GLSL shader class is based off the book "More OpenGL Game Programming."
 *
 * author: wcrane
 * since: 2013-12-28
**/
class GLSLShader
{
public:
  GLSLShader(const std::string &filename, GLenum shaderType = GL_VERTEX_SHADER);
  GLSLShader(GLenum shaderType = GL_VERTEX_SHADER );
  ~GLSLShader();
  void compile();
  bool isCompiled() const; 
  void getShaderLog(std::string &log) const;
  void getShaderSource(std::string &shader) const;
  void setShaderSource(std::string &code);
  
  GLuint getHandle() const;
  void getParameter(GLenum param, GLint *data) const;

private:
  char *readShader(const std::string &filename);
  bool compiled_;
  GLint handle_;
};
