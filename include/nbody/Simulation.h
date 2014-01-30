#ifndef _NBODY_SIMULATION_H
#define _NBODY_SIMULATION_H

#include <nbody/System.h>

#include <iosfwd>
#include <string>

namespace nbody {
    
    class Simulation {
        System *_system;
        std::string _name;
        Simulation& operator=(const Simulation &l);
        std::string generateName();
    public:
        Simulation(const Simulation &l);
        Simulation() : _system{nullptr}, _name{ generateName() } {}
        Simulation(std::istream &input) : _system{new System(input)}, _name{ generateName() } {}
        void evolveSystem(int nSteps, float dt);
        void loadRun(std::istream &input);
        void saveRun() const;
        size_t getNbodies(); 
        std::string getName() const;
        int* getNewCoords();
    };
    
} // namespace nbody

#endif // _NBODY_SIMULATION_H
