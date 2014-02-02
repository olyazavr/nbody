#ifndef _NBODY_SYSTEM_H
#define _NBODY_SYSTEM_H

#include <nbody/Vector3.h>
#include <nbody/Body.h>
#include <nbody/constants.h>

#include <iosfwd>
#include <string>

namespace nbody {

class Integrator {
    public:
        virtual void Integrate(float dt) = 0;
        virtual ~Integrator() {}
};

class Euler : public Integrator {
        size_t _nBodies;
        Body *_bodies;
        Euler(const Euler &sys) = delete;
        Euler& operator=(const Euler &sys) = delete;
    public:
        Euler(size_t N, Body *theBodies) : _nBodies(N), _bodies(theBodies) {}
        void Integrate(float dt);
};

class RK4 : public Integrator {
        size_t _nBodies;
        Body *_bodies;
        RK4(const RK4 &sys) = delete;
        RK4& operator=(const RK4 &sys) = delete;
    public:
        RK4(size_t N, Body *theBodies) : _nBodies(N), _bodies(theBodies) {}
        Vector3f get_dv(Body *body, float dt);
        Vector3f acc(Vector3f pos, float dt);
        void Integrate(float dt);
};

    
    // represents a system of bodies, and 
    // keeps track of its bodies and well as interacting them and returning the new coordinates of the bodies
    class System {
        size_t _nBodies;
        Body *_body; // this is an array!!!!
        float _softFactor = 1e-9f;
        float _dampingFactor = 1.0f;
        System() = delete;
        System(const System &sys) = delete;
        System& operator=(const System &sys) = delete;
        Integrator *_scheme;
    public:
        System(size_t N) : _nBodies{N}, _body{ new Body[N] }, _scheme{nullptr} { initRandomState(); 
                                            _scheme = new RK4( _nBodies, _body );}
        System(std::istream &input) : _nBodies{}, _body{nullptr}, _scheme{nullptr}  { readState(input); 
                                            _scheme = new RK4( _nBodies, _body );}
        System(std::string filename) : _nBodies{}, _body{nullptr}, _scheme{nullptr}  { readState(filename); 
                                             _scheme = new RK4( _nBodies, _body );}
        ~System() { delete [] _body; 
                    delete _scheme;}

        void integrateSystem(float dt);
        size_t getNbodies();
        float* getNewCoords();
        void readState(std::istream &input);
        void returnState(std::ostream &output) const;
        void readState(std::string filename);
        void writeState(std::ostream &output) const;
        void writeState(std::string filename) const;
        void initRandomState();
        void setSoftening(float soft) { _softFactor = soft; }
        void setDamping(float damp) { _dampingFactor = damp; }
    };

} 

// namespace nbody

#endif // _NBODY_SYSTEM_H
