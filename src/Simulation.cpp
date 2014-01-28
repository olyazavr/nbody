#include <nbody/Simulation.h>

#include <sstream>
#include <chrono>
#include <stdexcept>
#include <string>
#include <fstream>

namespace nbody {

  // Silly function for generating a unique simulation file run output name
  std::string Simulation::generateName() {
    using std::chrono::system_clock;
    using namespace std::chrono;
    std::tm timeinfo = std::tm();
    timeinfo.tm_year = 114;   // year: 2014
    timeinfo.tm_mon = 0;      // month: january
    timeinfo.tm_mday = 27;     // day: 27th
    std::time_t tt = std::mktime( &timeinfo );

    system_clock::time_point tp = system_clock::from_time_t( tt );
    system_clock::duration d = system_clock::now() - tp;
    std::ostringstream os; os << duration_cast<seconds>( d ).count() << "-sim.txt";
    return os.str();
  }

  void Simulation::loadRun( std::istream &input ) {
    if( _system != nullptr ) {
      throw std::runtime_error( "Tried to attach new system to running simulation!" );
    } else {
      _system = new System{input};
    }
  }

  void Simulation::evolveSystem( int nSteps, float dt ) {
    if( _system != nullptr ) {
      for( int step = 0; step < nSteps; ++step ) {
        _system->update( dt );
      }
    } else {
      throw std::runtime_error( "Tried to evolve simulation with no system!" );
    }
  }

  void Simulation::saveRun() const {
    std::ofstream output{ _name };
    _system->writeState( output );
  }

} // namespace nbody
