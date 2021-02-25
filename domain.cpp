#include "domain.h"


 std::tuple<double, double> Stop::AsTuple() const {
   return std::make_tuple(coordinates_.lat, coordinates_.lng);
 }

