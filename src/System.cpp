#include <nbody/constants.h>
#include <nbody/System.h>
#include <nbody/Vector3.h>

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <iomanip>

namespace nbody {
        
    // actual integration
    template<typename Integrator_T>
    void System<Integrator_T>::integrateSystem(float dt) {
        _scheme->Integrate(dt);
    }
    
    template<typename Integrator_T>
    void System<Integrator_T>::readState(std::istream &input) {
        input >> _nBodies;
        if(_nBodies > MAX_BODIES_RECOMMENDED) {
            throw std::runtime_error("Too many input bodies");
        }
        _body = new Body[_nBodies];
        for(size_t i = 0; i < _nBodies; ++i) {
            input >> _body[i];
        }
    }
    
    //sets all vector components and mass to random integers between 0 and 99
    template<typename Integrator_T>
    void System<Integrator_T>::initRandomState(){
        for(size_t i = 0; i < _nBodies; i++){
            _body[i].position() = Vector3f( rand() % 100, rand() % 100, rand() % 100 );
            _body[i].velocity() = Vector3f( rand() % 100, rand() % 100, rand() % 100 );
            _body[i].force() = Vector3f( rand() % 100, rand() % 100, rand() % 100 );
            _body[i].mass() = float(rand()) * float(rand()); //might break since mass is unassignable
        }
    }

    template<typename Integrator_T>
    void System<Integrator_T>::writeState(std::ostream &output) const {
        output << _nBodies << "\n";
        for(size_t i = 0; i < _nBodies; ++i) {
            output << _body[i] << "\n";
        }
    }

    template<typename Integrator_T>
    void System<Integrator_T>::returnState(std::ostream &output) const {
        output << _nBodies << "\n";
        for(size_t i = 0; i < _nBodies; ++i) {
            output << _body[i] << "\n";
        }
    }

    // get the new coordinates (x1, y1, z1, x2, y2, z2)
    template<typename Integrator_T>
    float* System<Integrator_T>::getNewCoords() {
        float *values = new float[_nBodies * 3]; // remember to delete []!!
        for(size_t i = 0; i < _nBodies; ++i) {
            values[3*i] = _body->position().x();
            values[3*i+1] = _body->position().y();
            values[3*i+2] = _body->position().z();
        }

        return values;
    }
    
     // gets the number of bodies
    template<typename Integrator_T>
    size_t System<Integrator_T>::getNbodies() {
        return _nBodies;
    }
    
} // namespace nbody
