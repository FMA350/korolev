//various
#include <stdio.h>
#include <math.h>
#include <various.h>

//header
#include "orbitalFunctions.h"

double CalculateSystemEnergy(struct List *list){
    SetToBeginning(&list);
    double energy = 0;
    int numberOfBodies = list->previous->position;
    for(int i = 0; i < numberOfBodies; i++){
        //for each body, add the kinetic energy as E = m*(V^2)/2
        energy += list->body->mass * (list->body->coordinates->x*list->body->coordinates->x+
                                    list->body->coordinates->y*list->body->coordinates->y+
                                    list->body->coordinates->z*list->body->coordinates->z)/2;
    }
    return energy;
}

double CalculateOrbitalSpeed(double r, double aphelion, double perihelion, double u){
  //only works if the orbiting body
 //v = sqrt(u*(2/r - 1/a))
  double velocity = sqrt(u*((2/r) - (1 / ((aphelion+perihelion)/2))));
  return velocity;
}
