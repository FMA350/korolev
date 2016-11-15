//various
#include <stdio.h>
#include <math.h>

//header
#include "orbitalFunctions.h"


double CalculateOrbitalSpeed(double r, double aphelion, double perihelion, double u){
  //only works if the orbiting body
 //v = sqrt(u*(2/r - 1/a))
  double velocity = sqrt(u*((2/r) - (1 / ((aphelion+perihelion)/2))));
  return velocity;
}
