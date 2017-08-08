//various
#include <stdio.h>
#include <math.h>
#include <various.h>


//header
#include "orbitalFunctions.h"

double CalculateSystemEnergy(struct List *list){
    //TODO: could be heavily optimized

    double totalEnergy = 0;
    double energy;
    double mass;
    double distance;

    double px;
    double py;
    double pz;
    double rx;
    double ry;
    double rz;
    SetToBeginning(&list);
    int numberOfBodies = list->previous->position; //since we are at position 0;
    for(int i = 0; i <= numberOfBodies; i++){
        //save some useful data that we will use later
        mass = list->body->mass;
        px = list->body->coordinates->x;
        py = list->body->coordinates->y;
        pz = list->body->coordinates->z;
        energy = 0;

        //for each body, add the kinetic energy as E = m*(V^2)/2
        energy += mass * (list->body->speedVector->x*list->body->speedVector->x+
                                    list->body->speedVector->y*list->body->speedVector->y+
                                    list->body->speedVector->z*list->body->speedVector->z)/2;
        //and the gravitational potential energy G*m*M/r for each object

        for(int j = 0; j < numberOfBodies; j++){
        list = list->next;
             //we will do a full cicle of the list
            rx = px-list->body->coordinates->x; //x distance
            ry = py-list->body->coordinates->y; //y distance
            rz = pz-list->body->coordinates->z; //z distance
            //total distance
            distance = sqrt(rx*rx+ry*ry+rz*rz);
            energy -= (GravConstG*mass*list->body->mass)/distance;
            /*negative since gravitational potential energy is negative.
            the work that the gravity does attracting a planet is thus positive. */
        }
        if(energy<0){
            printf(KDATA"The object %s has energy %G J and is in a stable elliptical orbit\n"KNORMAL, list->body->name, energy);
        }
        else{
            printf(KDATA"The object %s has energy %G J and is being ejected from the system \n"KNORMAL, list->body->name, energy );
        }
        totalEnergy += energy;
        list = list->next->next; //goes to the next element (list->current->next)
    }

    return totalEnergy;
}

double CalculateOrbitalSpeed(double r, double aphelion, double perihelion, double u){
  //only works if the orbiting body
 //v = sqrt(u*(2/r - 1/a))
  double velocity = sqrt(u*((2/r) - (1 / ((aphelion+perihelion)/2))));
  return velocity;
}
