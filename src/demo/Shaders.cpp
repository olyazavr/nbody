#include "Shaders.h"

#include <glload/gl_3_0.h>
#include <glload/gll.hpp>
#include <GL/freeglut.h>

#include <cstdint>
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>

Shaders::Shaders() : _shaderList{}, _vertexShaderText{}, _fragmentShaderText{} {}
Shaders::~Shaders() {}

GLuint Shaders::compile( GLenum eShaderType, const std::string &strShaderFile ) {
  GLuint shader = glCreateShader( eShaderType );
  const char *strFileData = strShaderFile.c_str();
  glShaderSource( shader, 1, &strFileData, nullptr );
  glCompileShader( shader );

  GLint status;
  glGetShaderiv( shader, GL_COMPILE_STATUS, &status );

  if (status == GL_FALSE) {
    GLint infoLogLength;
    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &infoLogLength );

    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glGetShaderInfoLog( shader, infoLogLength, nullptr, strInfoLog );

    std::string shaderType;

    switch( eShaderType ) {
      case   GL_VERTEX_SHADER: shaderType = "vertex"; break;
      case GL_FRAGMENT_SHADER: shaderType = "fragment"; break;
    }

    std::cerr << "Compile failure in " << shaderType << ":\n";
    std::cerr << strInfoLog << "\n";
    delete [] strInfoLog;
		throw std::runtime_error( "Failed to compile shader" );
  }

  return shader;
}

GLuint Shaders::link() {
  GLuint program = glCreateProgram();

  for( auto shader : _shaderList ) {
    glAttachShader( program, shader );
  }

  glLinkProgram( program );

  GLint status;
  glGetProgramiv( program, GL_LINK_STATUS, &status );

  if( status == GL_FALSE ) {
    GLint infoLogLength;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &infoLogLength );

    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glGetProgramInfoLog( program, infoLogLength, nullptr, strInfoLog );
    std::cerr << "Linker failure: " << strInfoLog << "\n";
    delete [] strInfoLog;
  }

  for( auto shader : _shaderList ) {
    glDetachShader( program, shader );
  }

  return program;
}

void Shaders::addToVertexList( std::string shaderFile ) {
	_vertexShaderText.push_back( shaderFile );
}

void Shaders::addToFragmentList( std::string shaderFile ) {
	_fragmentShaderText.push_back( shaderFile );
}

GLuint Shaders::build() {
	for( auto vertexShader : _vertexShaderText ) {
		_shaderList.push_back( compile( GL_VERTEX_SHADER, vertexShader ) );
  }
	for( auto fragmentShader : _fragmentShaderText ) {
		_shaderList.push_back( compile( GL_FRAGMENT_SHADER, fragmentShader ) );
  }

  GLuint openglProgram = link();

  std::for_each( std::begin( _shaderList ), std::end( _shaderList ), glDeleteShader );

	return openglProgram;
}