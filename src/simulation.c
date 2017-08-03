#include "monitor.h"
#include "various.h"
#include "data.h"
#include "matmath.h"
//#include "main.c"
#include <math.h>


//double timeStep = 86400; //day
double timeStep = 3600; //hour
//double timeStep = 60;   //minute
extern int sim_iteration;
extern void (*SimulationMethod)(struct List*, struct Coordinates*, struct Coordinates*,struct Coordinates*);

//time_t SimulationTime;

// simulation methods

void Save(struct Coordinates* coordinates, struct Coordinates* speedVector, struct Coordinates* newCoordinates, struct Coordinates* newSpeedVector){
    coordinates->x = newCoordinates->x;
    coordinates->y = newCoordinates->y;
    coordinates->z = newCoordinates->z;

    speedVector->x = newSpeedVector->x;
    speedVector->y = newSpeedVector->y;
    speedVector->z = newSpeedVector->z;
}

void NewtonGravitation(struct Coordinates* myPosition, struct Coordinates* otherElementPosition, struct Coordinates* newAcceleration, double u){
    //Instant accleration felt by objects.
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

void Euler(struct List* object, struct Coordinates* newCoordinates, struct Coordinates* newSpeedVector,struct Coordinates* newAcceleration){
    //it does not conserve the energy
    int position = object->position;
    struct List *currentElement = &(*object->next);
    while(currentElement->position != position){
        NewtonGravitation(object->body->coordinates, currentElement->body->coordinates, newAcceleration, currentElement->body->u);
        currentElement = currentElement->next;
        }
    //new coordinates using the old value for the speed.
    newCoordinates->x += object->body->speedVector->x * timeStep;
    newCoordinates->y += object->body->speedVector->y * timeStep;
    newCoordinates->z += object->body->speedVector->z * timeStep;
    //new speedVector adding V.old + a.new *dt
    newSpeedVector->x = object->body->speedVector->x + newAcceleration->x *timeStep;
    newSpeedVector->y = object->body->speedVector->y + newAcceleration->y *timeStep;
    newSpeedVector->z = object->body->speedVector->z + newAcceleration->z *timeStep;

    newAcceleration->x = 0;
    newAcceleration->y = 0;
    newAcceleration->z = 0;
}

 void RungeKutta(struct List* object, struct Coordinates* newCoordinates, struct Coordinates* newSpeedVector,struct Coordinates* newAcceleration){
// @ implemented following the paper:
// Implementing a Fourth Order Runge-Kutta Method for Orbit Simulation
// CJ Voesenek
// June 14, 2008
// TODO: contact the author, errata in knr e pag1
// TODO: Bring the declaration and initialization of k*r and k*v out of the function
//       for added efficiency.

    //init:
    struct Coordinates* k1r = createCoordinateSet(0,0,0);
    struct Coordinates* k2r = createCoordinateSet(0,0,0);
    struct Coordinates* k3r = createCoordinateSet(0,0,0);
    struct Coordinates* k4r = createCoordinateSet(0,0,0);

    struct Coordinates* k1v = createCoordinateSet(0,0,0);
    struct Coordinates* k2v = createCoordinateSet(0,0,0);
    struct Coordinates* k3v = createCoordinateSet(0,0,0);
    struct Coordinates* k4v = createCoordinateSet(0,0,0);

    struct Coordinates* newVelocity     = createCoordinateSet(0,0,0);
    //struct Coordinates* newAcceleration = createCoordinateSet(0,0,0);

    int position = object->position;
    struct List *currentElement = &(*object->next);

    /****************** CALCULATE k1v   *******************/
    //calculates the accleration in the current position and puts it in k1v
    while(currentElement->position != position){
        NewtonGravitation(object->body->coordinates, currentElement->body->coordinates, k1v, currentElement->body->u);
        currentElement = currentElement->next;
    }
    //k1v holds now the accleration for the body in his initial state.
    currentElement = currentElement->next; //move the list again back to the next element

    /****************** CALCULATE k1r   *******************/
    assign(k1r, object->body->speedVector); // k1r = velocity in Ri

    /****************** CALCULATE k2v   *******************/
    assign(newVelocity, k1r);      //we don't want to modify k1r, so assign its value to newVelocity;
    mols(newVelocity, timeStep/2); //velocity * time = distance!
    assign (newCoordinates, newVelocity); //so the new coordinates equal the velocity*dt/2...
    summ(newCoordinates ,object->body->coordinates); //plus the old coordinates.
    while(currentElement->position != position){
        //calculates the accleration in the current position and puts it in k2v
        NewtonGravitation(newCoordinates, currentElement->body->coordinates, k2v, currentElement->body->u);
        currentElement = currentElement->next;
    }
    //k2v holds now the accleration for the body in a hypothetical state
    currentElement = currentElement->next; //move the list again back to the next element


    /****************** CALCULATE k2r   *******************/
    assign(k2r, object->body->speedVector); //let's start assigning to k2r the initial speed value
    assign(newAcceleration, k1v); //we don't want to modify k1v, so assign its value to newAcceleration;
    mols(newAcceleration, timeStep/2); //new acceleration for half the time = new speed!
    summ(k2r, newAcceleration); //acceleration is now a speed; k2r = old speed + new speed.

    /****************** CALCULATE k3v   *******************/
    assign(newVelocity, k2r);      //we don't want to modify k2r, so assign its value to newVelocity;
    mols(newVelocity, timeStep/2); // new velocity is now a distance.
    assign (newCoordinates, newVelocity); //so the new coordinates equal the velocity*dt/2...
    summ(newCoordinates ,object->body->coordinates); //plus the old coordinates.
    while(currentElement->position != position){
        //calculates the accleration in the current position and puts it in k3v
        NewtonGravitation(newCoordinates, currentElement->body->coordinates, k3v, currentElement->body->u);
        currentElement = currentElement->next;
    }
    //k3v holds now the accleration for the body in a hypothetical state
    currentElement = currentElement->next; //move the list again back to the next element

    /****************** CALCULATE k3r   *******************/
    assign(k3r, object->body->speedVector); //let's start assigning to k3r the initial speed value
    assign(newAcceleration, k2v); //we don't want to modify k2v, so assign its value to newAcceleration;
    mols(newAcceleration, timeStep/2); //new acceleration for half the time = new speed!
    summ(k3r, newAcceleration); //acceleration is now a speed; k3r = old speed + new speed.

    /****************** CALCULATE k4v   *******************/
    assign(newVelocity, k3r);      //we don't want to modify k3r, so assign its value to newVelocity;
    mols(newVelocity, timeStep); // new velocity is now a distance.
    assign (newCoordinates, newVelocity); //so the new coordinates equal the velocity*dt...
    summ(newCoordinates ,object->body->coordinates); //plus the old coordinates.
    while(currentElement->position != position){
        //calculates the accleration in the current position and puts it in k4v
        NewtonGravitation(newCoordinates, currentElement->body->coordinates, k4v, currentElement->body->u);
        currentElement = currentElement->next;
    }
    //k4v holds now the accleration for the body in a hypothetical state
    currentElement = currentElement->next; //move the list again back to the next element

    /****************** CALCULATE k4r   *******************/
    assign(k4r, object->body->speedVector); //let's start assigning to k4r the initial speed value
    assign(newAcceleration, k3v); //we don't want to modify k3v, so assign its value to newAcceleration;
    mols(newAcceleration, timeStep); //new acceleration for half the time = new speed!
    summ(k4r, newAcceleration); //acceleration is now a speed; k3r = old speed + new speed.


/*******************    SAVING   ****************************/

//Assign to "newCoordinates" the final result of the calculation (position).
    assign(newVelocity, k1r);
        mols(k2r, 2);
    summ(newVelocity, k2r);
        mols(k3r, 2);
    summ(newVelocity, k3r);
    summ(newVelocity, k4r);
    mols(newVelocity, timeStep/6); //newVelocity now contains a deltaPosition
    summ(newVelocity, object->body->coordinates); // to get the final position I must do coordinates + deltaCoordinates.
    assign(newCoordinates, newVelocity); //ready to be stored.

    //Assign to "newSpeedVector" the final result of the calculation (speed).
    assign(newAcceleration, k1v);
        mols(k2v, 2);
    summ(newAcceleration, k2v);
        mols(k3v, 2);
    summ(newAcceleration, k3v);
    summ(newAcceleration, k4v);
    mols(newAcceleration, timeStep/6); //new accleration is the new deltaV
    summ(newAcceleration, object->body->speedVector); // to get the final speed I must V + deltaV.
    assign(newSpeedVector, newAcceleration); //ready to be stored.
//terribly inefficent. but it is 2.25 AM.
    free(k1v);
    free(k2v);
    free(k3v);
    free(k4v);

    free(k1r);
    free(k2r);
    free(k3r);
    free(k4r);
    free(newVelocity);
}


void* SimulationMain(void* input){
        ThreadData* data = (ThreadData*) input;
        int code;
        struct List * object = data->object;
        //struct List * object = (*obj);

        struct Coordinates* objectSpeedVector      = &(*data->body->speedVector);
        struct Coordinates* objectCoordinates      = &(*data->body->coordinates);
        struct Coordinates* newSpeedVector = createCoordinateSet(data->body->speedVector->x,
                                                                 data->body->speedVector->y,
                                                                 data->body->speedVector->z);
        struct Coordinates* newCoordinates = createCoordinateSet(object->body->coordinates->x,
                                                                 object->body->coordinates->y,
                                                                 object->body->coordinates->z);
        struct Coordinates* newAcceleration = createCoordinateSet(0,0,0);

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
            PrintState(data->simulationName, &object);
            condition_signal(data->computation_section); //wake up all the threads
            monitor_exit(data->mon);
        }
        else{
            //either WAIT_FOR_START OR SPIN_AT_BARRIER
            condition_wait(data->computation_section);
        }
        SimulationMethod(object, newCoordinates, newSpeedVector, newAcceleration);

        //save lock.
        monitor_enter(data->mon);
        if(condition_check(data->saving_section)){
            condition_signal(data->saving_section);
            monitor_exit(data->mon);
        }
        else{
            condition_wait(data->saving_section);
        }
        Save(object->body->coordinates, object->body->speedVector, newCoordinates, newSpeedVector);
        monitor_enter(data->mon);
        PrintDetails(data->body->name, data->object);
        monitor_exit(data->mon);
    }

}

void* SimulationCommander(void* input){

    ThreadData* data = (ThreadData*) input;
    while(condition_threads_waiting(data->computation_section)!=data->numberOfThreads){
        printf("sleeping...\n");
        sleep(1);
    }
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
    monitor_enter(data->mon);
    simulation_lock_on(data->computation_section);
    simulation_lock_on(data->saving_section);
    monitor_exit(data->mon);
    pthread_exit(0);
}
