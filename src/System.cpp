#include <nbody/constants.h>
#include <nbody/System.h>
#include <nbody/Vector3.h>

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <iomanip>

namespace nbody {
        
    // actual integration
    void System::integrateSystem(float dt) {
        _scheme->Integrate(dt);
    }
    
    void System::readState(std::istream &input) {
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
    void System::initRandomState(){
        for(size_t i = 0; i < _nBodies; i++){
            _body[i].position() = Vector3f( rand() % 100, rand() % 100, rand() % 100 );
            _body[i].velocity() = Vector3f( rand() % 100, rand() % 100, rand() % 100 );
            _body[i].force() = Vector3f( rand() % 100, rand() % 100, rand() % 100 );
            _body[i].mass() = float(rand()) * float(rand()); //might break since mass is unassignable
        }
    }

    void System::writeState(std::ostream &output) const {
        output << _nBodies << "\n";
        for(size_t i = 0; i < _nBodies; ++i) {
            output << _body[i] << "\n";
        }
    }

    void System::returnState(std::ostream &output) const {
        output << _nBodies << "\n";
        for(size_t i = 0; i < _nBodies; ++i) {
            output << _body[i] << "\n";
        }
    }

    // get the new coordinates (x1, y1, z1, x2, y2, z2)
    float* System::getNewCoords() {
        float *values = new float[_nBodies * 3]; // remember to delete []!!
        for(size_t i = 0; i < _nBodies; ++i) {
            values[3*i] = _body->position().x();
            values[3*i+1] = _body->position().y();
            values[3*i+2] = _body->position().z();
        }

        return values;
    }
    
     // gets the number of bodies
    size_t System::getNbodies() {
        return _nBodies;
    }


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
    
} // namespace nbody
