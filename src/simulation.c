#include "monitor.h"
#include "various.h"
#include "data.h"
#include "matmath.h"

#include <math.h>


//double timeStep = 86400; //day
double timeStep = 3600; //hour
//double timeStep = 60;   //minute
extern int sim_iteration;
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

void NewtonGravitation(struct Coordinates* myPosition, struct Coordinates* bodyPosition, struct Coordinates* newAcceleration, double u){
    //Instant accleration felt by objects.
    double rx = bodyPosition->x -  myPosition->x;
    double ry = bodyPosition->y -  myPosition->y;
    double rz = bodyPosition->z -  myPosition->z;
    //total distance
    double distance = sqrt(rx*rx+ry*ry+rz*rz);
    distance = pow(distance, 3);
    //printf("distance: %G\n", distance);
        if(distance < 1){
            printf(KERROR"DIVISION BY ZERO! ABORTING"KNORMAL);
            exit(1);
        }
    newAcceleration->x += u * rx/distance; //distance is actually distance^3
    newAcceleration->y += u * ry/distance;
    newAcceleration->z += u * rz/distance;
}

void SimpleSimulation(struct List* object, struct Coordinates* newCoordinates, struct Coordinates* newSpeedVector,struct Coordinates* newAcceleration){
    //it does not conserve the energy!
    //TODO:rename to Euler
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

//  void RungeKutta(struct List* object, struct Coordinates* newCoordinates, struct Coordinates* newSpeedVector,struct Coordinates* newAcceleration){
// // @ implemented following the paper:
// // Implementing a Fourth Order Runge-Kutta Method for Orbit Simulation
// // CJ Voesenek
// // June 14, 2008
//
//     int position = object->position;
//     struct List *currentElement = &(*object->next);
//     while(currentElement->position != position){
//         //calculates the accleration in the current position
//         NewtonGravitation(object->body->coordinates, currentElement->body->coordinates, newAcceleration, currentElement->body->u);
//         currentElement = currentElement->next;
//     }
//     //new accleration contains the sum of the accleration at t = 0;
//     struct Coordinates k1v = createCoordinateSet(newAcceleration->x,newAcceleration->y,newAcceleration->z);
//      //supposed acceleration at Ri (initial position)
//     struct Coordinates k1r = object->body->speedVector; // k1r = velocity in Ri
//     struct Coordinates k2v = object->body->coordinates;
//     sum(k2v,molts()); //TODO
//
//
// }


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
            //PrintState(data->simulationName, &(*obj));
            condition_signal(data->computation_section); //wake up all the threads
            monitor_exit(data->mon);
        }
        else{
            //either WAIT_FOR_START OR SPIN_AT_BARRIER
            condition_wait(data->computation_section);
        }
        SimpleSimulation(object, newCoordinates, newSpeedVector, newAcceleration);

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

    //printf(KDATA"simulation over at step %d\n\n\n"KNORMAL, condition_current_iteration(data->computation_section));
    pthread_exit(0);
}
