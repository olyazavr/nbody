#include <nbody/Body.h>
#include <nbody/Vector3.h>

#include <iostream>
#include <iomanip>

namespace nbody {

  std::istream& operator>>( std::istream &is, Body &body ) {
    is >> body._position >> body._velocity >> body._force >> body._mass;
    return is;
  }

  std::ostream& operator<<( std::ostream &os, const Body &body ) {
    os << std::scientific;
    os << std::setprecision( 8 );
    os << body._position << " " << body._velocity << " ";
    os << body._force << " " <<  body._mass;
    return os;
  }

} // namespace nbody
