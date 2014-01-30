#include <glload/gl_3_0.h>
#include <glload/gll.hpp>
#include <GL/freeglut.h>

#include <cstdint>
#include <string>
#include <iostream>
#include <stdexcept>

class Shaders;

class GlutWrapper {
  static void displayCallback();
  static void reshapeCallback( int theWidth, int theHeight );
  static void keyboardCallback( unsigned char key, int x, int y );
  static void mouseCallback( int button, int state, int x, int y );
	static void openglDebug( GLenum source, GLenum type, GLuint id, 
													 GLenum severity, GLsizei length,
													 const GLchar* message, GLvoid* userParam );
	Shaders *_shaders;
	void GlutWrapper::initVertexBuffers( size_t bufSize, float *buf );
protected:
  int _width, _height;
  int _windowId;
	size_t _bufSize;
	float *_buf;
  std::string _windowTitle;
	GLuint _positionBufferObject;
	GLuint _vao;
	GLuint _program;
  static GlutWrapper *_instance;
public:
	enum Mode { DEBUG, NDEBUG };
  GlutWrapper();
  GlutWrapper( Mode debugMode );
	GlutWrapper::GlutWrapper( const std::string &title );
	GlutWrapper( const std::string &title, 
               Mode debugMode = Mode::NDEBUG );
  ~GlutWrapper();

  void init( int argc, char **argv, int theWidth, int theHeight, Shaders *shaders, size_t bufSize = 0, float *buf = nullptr );
  void createWindow();
	void run();
  static GlutWrapper* getInstance();

  inline int width() const { return _width; }
  inline int height() const { return _height; }

  virtual void display() = 0;
  virtual void reshape( int theWidth, int theHeight ) = 0;
  virtual void keyboard( unsigned char key, int x, int y );
  virtual void mouse( int button, int state, int x, int y );
private:
  Mode _debugMode;
};