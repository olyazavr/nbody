#include <nbody/Simulation.h>

#include <iostream>
#include <fstream>

int main() {
  try {
    std::ifstream input{ "resources/nbody/binary-system-simple.txt" };
    nbody::Simulation sim{input};
    for( int i = 0; i < 40; ++i ) {
      std::cout << "==EVOLUTION " << i + 1 << "\n";
      sim.saveRun();
      sim.evolveSystem( 1e4, 0.000001 );
    }
    sim.saveRun();
    return 0;
  } catch( const std::exception &e ) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
