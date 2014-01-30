#include "Shaders.h"
#include "GlutWrapper.h"

#include <glload/gl_3_0.h>
#include <glload/gll.hpp>
#include <GL/freeglut.h>

#include <cstdint>
#include <string>
#include <iostream>
#include <stdexcept>

GlutWrapper *GlutWrapper::_instance = nullptr;

GlutWrapper::GlutWrapper() : 
  _debugMode{Mode::NDEBUG}, _width{}, _height{}, 
  _windowId{}, _windowTitle{} {}

GlutWrapper::GlutWrapper( Mode debugMode ) : 
  _debugMode{debugMode}, _width{}, _height{}, 
  _windowId{}, _windowTitle{} {}

GlutWrapper::GlutWrapper( const std::string &title, Mode debugMode ) : 
  _debugMode{debugMode}, _width{}, _height{}, 
  _windowId{}, _windowTitle{title} {}

GlutWrapper::~GlutWrapper() {
//  glutDestroyWindow( _windowId );
}

void GlutWrapper::init( int argc, char **argv, int theWidth, int theHeight, Shaders *shaders, size_t bufSize, float *buf ) {
	_width = theWidth;
	_height = theHeight;
	if( _windowTitle == "" ) {
		_windowTitle = std::string( argv[0] );
  }

  glutInit( &argc, argv );
  uint32_t displayMode = GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL;

  glutInitDisplayMode( displayMode );
  // We'll be using OpenGL 3.0
  glutInitContextVersion( 3, 0 );
  glutInitContextProfile( GLUT_CORE_PROFILE );

  // Give us some extra debugging info if we asked
  if( _debugMode == Mode::DEBUG ) {
    glutInitContextFlags( GLUT_DEBUG );
  }

	if( shaders == nullptr ) {
		throw std::runtime_error( "GlutWrapper::init was provided null Shaders pointer" );
  }

	createWindow();
	_program = shaders->build();
	initVertexBuffers( bufSize, buf );
	glGenVertexArrays( 1, &_vao );
}

void GlutWrapper::createWindow() {
  glutInitWindowSize( _width, _height );
  _windowId = glutCreateWindow( _windowTitle.c_str() );

	glload::LoadFunctions();
  glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION );

  if( !glload::IsVersionGEQ( 3, 0 ) ) {
    std::cout << "You have OpenGL " << glload::GetMajorVersion();
    std::cout << "." << glload::GetMinorVersion() << " but this needs 3.0.\n";
    throw std::runtime_error( "Wrong version of OpenGL" );
  }

  if( _debugMode == Mode::DEBUG ) {
    if( glext_ARB_debug_output ) {
      glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB );
      glDebugMessageCallbackARB( (GLDEBUGPROCARB) openglDebug, (void*) 15 );
    }
  }
}

void GlutWrapper::initVertexBuffers( size_t bufSize, float *buf ) {
  glGenBuffers( 1, &_positionBufferObject );

  glBindBuffer( GL_ARRAY_BUFFER, _positionBufferObject );
	if( buf == nullptr ) {
		throw std::runtime_error( "Provided a null buffer to initVertexBuffers." );
  } else {
		_bufSize = bufSize;
		_buf = buf;
		glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * bufSize, _buf, GL_STATIC_DRAW );
  }
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void GlutWrapper::run() {
	glBindVertexArray( _vao );
	glutDisplayFunc( displayCallback );
	glutReshapeFunc( reshapeCallback );
	glutKeyboardFunc( keyboardCallback );
	glutMainLoop();
}

// Jason L. McKesson's helpful debugging function
// See http://www.arcsynthesis.org/gltut/
void GlutWrapper::openglDebug( GLenum source, GLenum type, 
                               GLuint /*id*/, GLenum severity, GLsizei /*length*/,
                               const GLchar* message, GLvoid* /*userParam*/ ) {
  std::string srcName, errorType, typeSeverity;

  switch( source ) {
    case             GL_DEBUG_SOURCE_API_ARB: srcName = "API"; break;
    case   GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB: srcName = "Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: srcName = "Shader Compiler"; break;
    case     GL_DEBUG_SOURCE_THIRD_PARTY_ARB: srcName = "Third Party"; break;
    case     GL_DEBUG_SOURCE_APPLICATION_ARB: srcName = "Application"; break;
    case           GL_DEBUG_SOURCE_OTHER_ARB: srcName = "Other"; break;
  }

  switch( type ) {
    case               GL_DEBUG_TYPE_ERROR_ARB: errorType = "Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: errorType = "Deprecated Functionality"; break;
    case  GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB: errorType = "Undefined Behavior"; break;
    case         GL_DEBUG_TYPE_PORTABILITY_ARB: errorType = "Portability"; break;
    case         GL_DEBUG_TYPE_PERFORMANCE_ARB: errorType = "Performance"; break;
    case               GL_DEBUG_TYPE_OTHER_ARB: errorType = "Other"; break;
  }

  switch( severity ) {
    case   GL_DEBUG_SEVERITY_HIGH_ARB: typeSeverity = "High"; break;
    case GL_DEBUG_SEVERITY_MEDIUM_ARB: typeSeverity = "Medium"; break;
    case    GL_DEBUG_SEVERITY_LOW_ARB: typeSeverity = "Low"; break;
  }

  std::cout << errorType << " from " << srcName << ",\t" << typeSeverity;
  std::cout << " priority\nMessage: " << message << "\n";
}

void GlutWrapper::displayCallback() {
	_instance->display();
}

void GlutWrapper::reshapeCallback( int theWidth, int theHeight ) {
	_instance->reshape( theWidth, theHeight );
}

void GlutWrapper::mouseCallback( int button, int state, int x, int y ) {
	_instance->mouse( button, state, x, y );
}

void GlutWrapper::keyboardCallback( unsigned char key, int x, int y ) {
	_instance->keyboard( key, x, y );
}

// By default, don't do anything on a mouse click
void GlutWrapper::mouse( int /*button*/, int /*state*/, int /*x*/, int /*y*/ ) {}

// By default, don't do anything on a key press
void GlutWrapper::keyboard( unsigned char /*key*/, int /*x*/, int /*y*/ ) {}