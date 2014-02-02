#include <nbody/Simulation.h>

#include <sstream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <string>

namespace nbody {
    
    // Silly function for generating a unique simulation file run output name
    std::string Simulation::generateName() {
        std::string name("name");
        return name;
    }
    
    void Simulation::loadRun(std::istream &input) {
        if(_system != nullptr) {
            throw std::runtime_error("Tried to attach new system to running simulation!");
        } else {
            _system = new System{input};
        }
    }

    std::string Simulation::getName() const{
        return _name;
    }

    // copy constructor
    Simulation::Simulation(const Simulation &l): _system{NULL}, _name{l.getName()} {
        //nope
    }

    // assignment
    Simulation& Simulation::operator=(const Simulation &l){
        _name = l.getName();
        return *this;
    }
    
    void Simulation::evolveSystem(int nSteps, float dt) {
        if(_system != nullptr) {
            for(int step = 0; step < nSteps; ++step) {
                _system->integrateSystem(dt);
            }
        } else {
            throw std::runtime_error("Tried to evolve simulation with no system!");
        }
    }
    
    void Simulation::saveRun() const {
        std::ofstream output;
        output.open( _name.c_str() );
        _system->writeState(output);
    }

    // get the new coordinates (x1, y1, z1, x2, y2, z2)
    float* Simulation::getNewCoords(){
        return _system->getNewCoords();
    }

    // gets the number of bodies
    size_t Simulation::getNbodies(){
        return _system->getNbodies();
    }
    
} // namespace nbody
