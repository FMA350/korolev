#include <math.h>
#include <stdlib.h>
#include <pthread.h> //for pthread_exit
#include <stdio.h>
#include <unistd.h>  //for sleep
#include  <time.h>   //to calculate efficiency and time

#include "commands.h"
#include "monitor.h"
#include "various.h"
#include "data.h"
#include "matmath.h"



//double timeStep = 86400; //day
double timeStep = 3600; //hour
//double timeStep = 60;   //minute
extern int sim_iteration;
extern void (*SimulationMethod)(struct List*, SimulationThreadData* data);

time_t SimulationTime;

// simulation methods

void Save(struct Coordinates* coordinates, struct Coordinates* speedVector, struct Coordinates* newCoordinates, struct Coordinates* newSpeedVector){
    assign(coordinates, newCoordinates);
    assign(speedVector, newSpeedVector);
}

void NewtonGravitation(struct Coordinates* myPosition, struct Coordinates* otherElementPosition, struct Coordinates* newAcceleration, double u){
    //Instant accleration felt by an object due to another object
    double rx = otherElementPosition->x -  myPosition->x;
    double ry = otherElementPosition->y -  myPosition->y;
    double rz = otherElementPosition->z -  myPosition->z;
    //total distance
    double distance = sqrt(rx*rx+ry*ry+rz*rz);
    distance = pow(distance, 3);

    newAcceleration->x += u * rx/distance;
    newAcceleration->y += u * ry/distance;
    newAcceleration->z += u * rz/distance;
}

void Euler(struct List* object, SimulationThreadData* data){
    //it does not conserve the energy of the system for a long time.
    int position = object->position;

    struct List *currentElement = &(*object->next);
    while(currentElement->position != position){
        NewtonGravitation(object->body->coordinates, currentElement->body->coordinates, data->newAcceleration, currentElement->body->u);
        currentElement = currentElement->next;
    }
    //new coordinates using the old value for the speed.
    assign(data->newCoordinates, object->body->coordinates);
    assign(data->temp, object->body->speedVector);
    mols(data->temp, timeStep);
    summ(data->newCoordinates, data->temp);

    //data->newVelocity += V.old + a.new *dt
    assign(data->temp, object->body->speedVector);
    mols(data->newAcceleration, timeStep);
    summ(data->temp, data->newAcceleration);
    assign(data->newVelocity, data->temp);
    data->temp->x = 0;
    data->temp->y = 0;
    data->temp->z = 0;
}

 void RungeKutta(struct List* object, SimulationThreadData* data){
// @ implemented following the paper:
// Implementing a Fourth Order Runge-Kutta Method for Orbit Simulation
// CJ Voesenek
// June 14, 2008
// TODO: contact the author, errata in knr e pag1
// TODO: Bring the declaration and initialization of k*r and k*v out of the function
//       for added efficiency.

    //init:
    // struct Coordinates* k1r = createCoordinateSet(0,0,0);
    // struct Coordinates* data->k2r = createCoordinateSet(0,0,0);
    // struct Coordinates* data->k3r = createCoordinateSet(0,0,0);
    // struct Coordinates* data->k4r = createCoordinateSet(0,0,0);
    //
    // struct Coordinates* k1v = createCoordinateSet(0,0,0);
    // struct Coordinates* data->k2v = createCoordinateSet(0,0,0);
    // struct Coordinates* data->k3v = createCoordinateSet(0,0,0);
    // struct Coordinates* data->k4v = createCoordinateSet(0,0,0);
    //
    // struct Coordinates* newVelocity     = createCoordinateSet(0,0,0);

    int position = object->position;
    struct List *currentElement = &(*object->next);

    /****************** CALCULATE k1v   *******************/
    //calculates the accleration in the current position and puts it in k1v
    while(currentElement->position != position){
        NewtonGravitation(object->body->coordinates, currentElement->body->coordinates, data->k1v, currentElement->body->u);
        currentElement = currentElement->next;
    }
    //k1v holds now the accleration for the body in his initial state.
    currentElement = currentElement->next; //move the list again back to the next element

    /****************** CALCULATE data->k1r   *******************/
    assign(data->k1r, object->body->speedVector); // data->k1r = velocity in Ri

    /****************** CALCULATE data->k2v   *******************/
    assign(data->newVelocity, data->k1r);      //we don't want to modify data->k1r, so assign its value to data->newVelocity;
    mols(data->newVelocity, timeStep/2); //velocity * time = distance!
    assign (data->newCoordinates, data->newVelocity); //so the new coordinates equal the velocity*dt/2...
    summ(data->newCoordinates ,object->body->coordinates); //plus the old coordinates.
    while(currentElement->position != position){
        //calculates the accleration in the current position and puts it in data->k2v
        NewtonGravitation(data->newCoordinates, currentElement->body->coordinates, data->k2v, currentElement->body->u);
        currentElement = currentElement->next;
    }
    //data->k2v holds now the accleration for the body in a hypothetical state
    currentElement = currentElement->next; //move the list again back to the next element


    /****************** CALCULATE data->k2r   *******************/
    assign(data->k2r, object->body->speedVector); //let's start assigning to data->k2r the initial speed value
    assign(data->newAcceleration, data->k1v); //we don't want to modify data->k1v, so assign its value to newAcceleration;
    mols(data->newAcceleration, timeStep/2); //new acceleration for half the time = new speed!
    summ(data->k2r, data->newAcceleration); //acceleration is now a speed; data->k2r = old speed + new speed.

    /****************** CALCULATE data->k3v   *******************/
    assign(data->newVelocity, data->k2r);      //we don't want to modify data->k2r, so assign its value to data->newVelocity;
    mols(data->newVelocity, timeStep/2); // new velocity is now a distance.
    assign (data->newCoordinates, data->newVelocity); //so the new coordinates equal the velocity*dt/2...
    summ(data->newCoordinates ,object->body->coordinates); //plus the old coordinates.
    while(currentElement->position != position){
        //calculates the accleration in the current position and puts it in data->k3v
        NewtonGravitation(data->newCoordinates, currentElement->body->coordinates, data->k3v, currentElement->body->u);
        currentElement = currentElement->next;
    }
    //data->k3v holds now the accleration for the body in a hypothetical state
    currentElement = currentElement->next; //move the list again back to the next element

    /****************** CALCULATE data->k3r   *******************/
    assign(data->k3r, object->body->speedVector); //let's start assigning to data->k3r the initial speed value
    assign(data->newAcceleration, data->k2v); //we don't want to modify data->k2v, so assign its value to newAcceleration;
    mols(data->newAcceleration, timeStep/2); //new acceleration for half the time = new speed!
    summ(data->k3r, data->newAcceleration); //acceleration is now a speed; data->k3r = old speed + new speed.

    /****************** CALCULATE data->k4v   *******************/
    assign(data->newVelocity, data->k3r);      //we don't want to modify data->k3r, so assign its value to data->newVelocity;
    mols(data->newVelocity, timeStep); // new velocity is now a distance.
    assign (data->newCoordinates, data->newVelocity); //so the new coordinates equal the velocity*dt...
    summ(data->newCoordinates ,object->body->coordinates); //plus the old coordinates.
    while(currentElement->position != position){
        //calculates the accleration in the current position and puts it in data->k4v
        NewtonGravitation(data->newCoordinates, currentElement->body->coordinates, data->k4v, currentElement->body->u);
        currentElement = currentElement->next;
    }
    //data->k4v holds now the accleration for the body in a hypothetical state

    /****************** CALCULATE data->k4r   *******************/
    assign(data->k4r, object->body->speedVector); //let's start assigning to data->k4r the initial speed value
    assign(data->newAcceleration, data->k3v); //we don't want to modify data->k3v, so assign its value to newAcceleration;
    mols(data->newAcceleration, timeStep); //new acceleration for half the time = new speed!
    summ(data->k4r, data->newAcceleration); //acceleration is now a speed; data->k3r = old speed + new speed.


/*******************    SAVING   ****************************/

//Assign to "newCoordinates" the final result of the calculation (position).
    assign(data->newVelocity, data->k1r);
        mols(data->k2r, 2);
    summ(data->newVelocity, data->k2r);
        mols(data->k3r, 2);
    summ(data->newVelocity, data->k3r);
    summ(data->newVelocity, data->k4r);
    mols(data->newVelocity, timeStep/6); //data->newVelocity now contains a deltaPosition
    summ(data->newVelocity, object->body->coordinates); // to get the final position I must do coordinates + deltaCoordinates.
    assign(data->newCoordinates, data->newVelocity); //ready to be stored.

    //Assign to "newSpeedVector" the final result of the calculation (speed).
    assign(data->newAcceleration, data->k1v);
        mols(data->k2v, 2);
    summ(data->newAcceleration, data->k2v);
        mols(data->k3v, 2);
    summ(data->newAcceleration, data->k3v);
    summ(data->newAcceleration, data->k4v);
    mols(data->newAcceleration, timeStep/6); //new accleration is the new deltaV
    summ(data->newAcceleration, object->body->speedVector); // to get the final speed I must V + deltaV.
    assign(data->newVelocity, data->newAcceleration); //ready to be stored.
    //cleanup
    data->k1v->x = 0;
    data->k1v->y = 0;
    data->k1v->z = 0;
//terribly inefficent. but it is 2.25 AM.
    // free(k1v);
    // free(data->k2v);
    // free(data->k3v);
    // free(data->k4v);
    //
    // free(k1r);
    // free(data->k2r);
    // free(data->k3r);
    // free(data->k4r);
    // free(newVelocity);
}


void* SimulationMain(void* input){
        SimulationThreadData* data = (SimulationThreadData*) input;
        int code; //flag used for multithreading
        struct List ** object = &data->object; //pointer to obj
        // data.newVelocity = createCoordinateSet((*object)->body->speedVector->x,
        //                                                          (*object)->body->speedVector->y,
        //                                                          (*object)->body->speedVector->z);
        // data.newCoordinates = createCoordinateSet((*object)->body->coordinates->x,
        //                                                          (*object)->body->coordinates->y,
        //                                                          (*object)->body->coordinates->z);
        // data.newAcceleration = createCoordinateSet(0,0,0);

    while(1){
        //computation lock
        monitor_enter(data->mon);
        code = condition_check(data->computation_section);
        if(code == SIMULATION_END){
            //I am the last thread to exit.
            condition_commander_signal(data->computation_section);
            monitor_exit(data->mon);
            pthread_exit(0);
        }
        else if (code == EXIT_THREAD){
            monitor_exit(data->mon);
            pthread_exit(0);
        }
        else if(code == BREAK_BARRIER){
            sim_iteration++;
            PrintState(data->simulationName, object);
            condition_signal(data->computation_section); //wake up all the threads
            monitor_exit(data->mon);
        }
        else{
            //either WAIT_FOR_START OR SPIN_AT_BARRIER
            condition_wait(data->computation_section);
        }
        //Euler(object, newCoordinates, newSpeedVector, newAcceleration);
        SimulationMethod((*object), data);

        //save lock.
        monitor_enter(data->mon);
        if(condition_check(data->saving_section)){
            condition_signal(data->saving_section);
            monitor_exit(data->mon);
        }
        else{
            condition_wait(data->saving_section);
        }

        Save((*object)->body->coordinates, (*object)->body->speedVector, data->newCoordinates, data->newVelocity);
        //Save(objectCoordinates, objectSpeedVector, newCoordinates, newSpeedVector);
        monitor_enter(data->mon);
        PrintDetails(data->body->name, *object);
        monitor_exit(data->mon);
    }
}

void* SimulationCommander(void* input){

    SimulationThreadData* data = (SimulationThreadData*) input;
    while(condition_threads_waiting(data->computation_section)!=data->numberOfThreads){
        printf("sleeping...\n");
        sleep(1);
    }
    clock_t launch = clock();
    monitor_enter(data->mon);
    printf("All threads ready, starting...\n");
    simulation_lock_off(data->computation_section);
    simulation_lock_off(data->saving_section);
    printf("commander signaling the computation_section\n");
    condition_signal(data->computation_section);
    printf("commander suspending at the computation_section\n");
    condition_commander_start(data->computation_section);

    //waits for the simulation to be over.

    printf("commander awakened at the computation_section\n");
    clock_t done = clock();
    double diff = ((double)done - (double)launch) / CLOCKS_PER_SEC;
    printf("total time: %f\n",diff);
    monitor_enter(data->mon);
    simulation_lock_on(data->computation_section);
    simulation_lock_on(data->saving_section);
    monitor_exit(data->mon);
    pthread_exit(0);
}
