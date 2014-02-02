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
  
  void Euler::Integrate(float dt){
    for( size_t i = 0; i < _nBodies; i++){
        Vector3f r, v, a;
        Body *body = _bodies + i;
        r = body->position();
        v = body->velocity();
        a = body->force();

        body->position() = r + ( v * dt ) + ( 0.5f * a * dt * dt );
        body->velocity() = v + ( a * dt );
    }
    
}

//is acceleration in Body still necessary?
Vector3f RK4::acc(Vector3f pos, float dt){
    Vector3f a, d;
    a = Vector3f(0.0f,0.0f,0.0f);
    for( size_t i = 0; i < _nBodies; i++){
        Body *other = _bodies + i;
        d = (other->position() + other->velocity() * dt) - pos;
        if( d.norm() != 0 ){
            float invDistCubed = 1.0f/( cube(d.norm()) );//use softFactor?
            Vector3f g = NEWTON_G * other->mass() * invDistCubed * d;
            a = a + g;
        }
    }
    return a;
}

Vector3f RK4::get_dv( Body *body, float dt){
    Vector3f r = body->position();
    Vector3f v = body->velocity();
    r = r + ( v * dt );
    Vector3f dv = acc( r, dt ) * dt;
    return dv;
}

void RK4::Integrate(float dt){
    Vector3f *dv_vals = new Vector3f[_nBodies];
    Vector3f *dr_vals = new Vector3f[_nBodies];
    for( size_t i = 1; i < 5; i++){
        float step = floor(i/2) * dt; //produces 0, then 0.5, then 0.5, then 1, as per RK formula
        //index of dr/dv value corresponds to the index of its body

        for( size_t j = 0; j < _nBodies; j++ ){
            Vector3f r, v, dr, dv;
            Body *body = _bodies + j;
            r = body->position();
            v = body->velocity();

            dv = get_dv( body, step);

            if( step == 0.5f ){
                dr_vals[j] = dr_vals[j] + 1.0f/3.0f * dv * dt;
                dv_vals[j] = dv_vals[j] + 1.0f/3.0f * dv;
            } else {
                dr_vals[j] = dr_vals[j] + 1.0f/6.0f * dv * dt;
                dv_vals[j] = dv_vals[j] + 1.0f/6.0f * dv;
            }
        }
    }

    for( size_t i = 0; i < _nBodies; i++ ){  
        Body *body = _bodies + i;
        Vector3f r = body->position();
        body->position() = r + dr_vals[i];
        body->velocity() = r + dv_vals[i];
    }  

    delete[] dr_vals;
    delete[] dv_vals;   
    
}

    
    // represents a system of bodies, and 
    // keeps track of its bodies and well as interacting them and returning the new coordinates of the bodies
    template<typename Integrator_T>
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
                                            _scheme = new Integrator_T( _nBodies, _body );}
        System(std::istream &input) : _nBodies{}, _body{nullptr}, _scheme{nullptr}  { readState(input); 
                                            _scheme = new Integrator_T( _nBodies, _body );}
        System(std::string filename) : _nBodies{}, _body{nullptr}, _scheme{nullptr}  { readState(filename); 
                                             _scheme = new Integrator_T( _nBodies, _body );}
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
