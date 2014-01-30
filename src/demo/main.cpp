#include <nbody/Simulation.h>
#include <nbody/GlutWrapper.h>
#include <nbody/Shaders.h>

#include <glload/gl_3_0.h>
#include <glload/gll.hpp>
#include <GL/freeglut.h>

#include <OpenGL/freeglut.h>

#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cmath>

namespace nBodyShaders {
  const std::string vertex1(
    "#version 130\n"
    "in vec4 position;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = position;\n"
    "}\n"
  );

  const std::string fragment1(
    "#version 130\n"
    "out vec4 outputColor;\n"
    "void main()\n"
    "{\n"
    "   outputColor = vec4(1.0f,1.0f,1.0f,1.0f);"
    "}\n"
  );
} // namespace shaders

class NBodyWindow : public GlutWrapper {
  nbody::Simulation *_sim; // used to get the updated coordinates
public:
    NBodyWindow( const std::string &title, 
                 nbody::Simulation *sim,
                 Mode debugMode = Mode::NDEBUG);
    ~NBodyWindow();

    void display();
    void reshape( int theWidth, int theHeight );
    void keyboard( unsigned char key, int x, int y );
    NBodyWindow& operator=(const NBodyWindow &l);
    NBodyWindow(const NBodyWindow &l);
    nbody::Simulation* getSim() const;
};

nbody::Simulation* NBodyWindow::getSim() const{
    return _sim;
}

// init all the things!
NBodyWindow::NBodyWindow( const std::string &title, nbody::Simulation *sim, Mode debugMode ) : 
GlutWrapper{ title, debugMode }, _sim{sim} {
    _instance = this;
}

// copy constructor
NBodyWindow::NBodyWindow(const NBodyWindow &l) : GlutWrapper{ NULL, Mode::NDEBUG }, _sim{l.getSim()}{
    //nope
}

// assignment
NBodyWindow& NBodyWindow::operator=(const NBodyWindow &l){
    _sim = l.getSim();
    return *this;
}

NBodyWindow::~NBodyWindow() {}

void NBodyWindow::reshape( int theWidth, int theHeight ) {
    glViewport( 0, 0, (GLsizei) theWidth, (GLsizei) theHeight );
}

void NBodyWindow::keyboard( unsigned char key, int /*x*/, int /*y*/ ) {
    const char ESCAPE_KEY = 27;
  if( key == ESCAPE_KEY ) {
    glutLeaveMainLoop();
  }
}

int theCount = 0;

void NBodyWindow::display() {
    // get the new coordinates, update the display
  int * coords = _sim->getNewCoords();
    for( size_t i = 0; i < _bufSize / 4; ++i ) {
        _buf[4*i] = coords[i]; // x
        _buf[4*i+1] = coords[i + 1]; // y
        _buf[4*i+2] = coords[i + 2]; // z
        _buf[4*i+3] = 1.0f; // nothing
        theCount++;
  }

  // bind all the things
    glBindBuffer( GL_ARRAY_BUFFER, _positionBufferObject );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( float ) * _bufSize, _buf );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

  // clear bg
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glUseProgram( _program );
    glBindBuffer( GL_ARRAY_BUFFER, _positionBufferObject );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, 0 );

    // actually draw our stuff
    glDrawArrays( GL_TRIANGLE_STRIP, 0, (GLsizei) _bufSize );

    glDisableVertexAttribArray( 0 );
    glUseProgram( 0 );

    // display!!
    glutSwapBuffers();
    glutPostRedisplay();
}

int main( int argc, char **argv ) {
  // number of bodies and their coordinates
  size_t N;
  int * coords;
  nbody::Simulation *sim;

  try {
        // read in this file (with bodies info)
        std::ifstream input{ "resources/nbody/binary-system-simple.txt" };
        sim = new nbody::Simulation(input);

        // get the number of bodes and coordinates
        N = sim->getNbodies();
        coords = sim->getNewCoords();

        for(int i = 0; i < 40; ++i) {
            std::cout << "==EVOLUTION " << i + 1 << "\n";
            sim->saveRun();
            sim->evolveSystem(1e4, 0.000001);
        }
        sim->saveRun();
        return 0;
    } catch(const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

  try {
        size_t bufSize = 4 * N;
        float *buf = new float[bufSize];

    // starting positions
        for( size_t i = 0; i < N; ++i ) {
            buf[4*i] = coords[i]; // x
            buf[4*i+1] = coords[i + 1]; // y
            buf[4*i+2] = coords[i + 2]; // z
            buf[4*i+3] = 1.0f; // nothing

            std::cout << buf[4*i] << " " << buf[4*i+1];
      std::cout << " " << buf[4*i+2] << " " << buf[4*i+3] << "\n";
    }

    // start the magic!
        Shaders shaders;
        shaders.addToVertexList( nBodyShaders::vertex1 );
        shaders.addToFragmentList( nBodyShaders::fragment1 );

    // pass in sim because we need updated coordinates. UNSAFE, but
    // so is all of OpenGL
    NBodyWindow * window = new NBodyWindow("N-Body Simulation!", sim, GlutWrapper::NDEBUG);
        window->init( argc, argv, 500, 500, &shaders, bufSize, buf );
        window->run();

    // release resources
    delete [] buf;
    delete [] sim;
    delete [] window;
    return 0;
  } catch( const std::exception &e ) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
