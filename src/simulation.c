#include "monitor.h"
#include "various.h"
#include <math.h>

double timeStep = 216000; //seconds

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

    double rx = bodyPosition->x -  myPosition->x;
    double ry = bodyPosition->y -  myPosition->y;
    double rz = bodyPosition->z -  myPosition->z;
    //total distance
    double distance = sqrt(rx*rx+ry*ry+rz*rz);
    distance = pow(distance, 3);
    //printf("distance: %G\n", distance);
        if(distance == 0){
            printf(KERROR"DIVISION BY ZERO! ABORTING"KNORMAL);
            exit(1);
        }
    newAcceleration->x += u * rx/distance; //distance is actually distance^3
    newAcceleration->y += u * ry/distance;
    newAcceleration->z += u * rz/distance;
}

void SimpleSimulation(struct List* object, struct Coordinates* newCoordinates, struct Coordinates* newSpeedVector, struct Coordinates* objectCoordinates,struct Coordinates* newAcceleration){
    //it does not conserve the energy!
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


void* SimulationMain(void* input){
        ThreadData* data = (ThreadData*) input;
        struct List ** obj = &(*data->object);
        struct List * object = (*obj);

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
        if(condition_check(data->computation_section, data->numberOfThreads-1)){
            condition_signal(data->computation_section);
            monitor_exit(data->mon);
        }
        else{
            condition_wait(data->computation_section);
        }

        SimpleSimulation(object, newCoordinates, newSpeedVector, objectCoordinates, newAcceleration); //calculates new accelerations
        //FIXME

        //save lock.
        monitor_enter(data->mon);
        if(condition_check(data->saving_section, data->numberOfThreads-1)){
            condition_signal(data->saving_section);
            monitor_exit(data->mon);
        }
        else{
            condition_wait(data->saving_section);
        }
        Save(object->body->coordinates, object->body->speedVector, newCoordinates, newSpeedVector);
        monitor_enter(data->mon);
        PrintDetails(data->body->name, (*data->object));
        monitor_exit(data->mon);
    }

}

void* SimulationCommander(void* input){
    int  i = 0;
    ThreadData* data = (ThreadData*) input;
    while(1){
        printf("simulation at hour %d", i);
        getchar();
        monitor_enter(data->mon);
        simulation_lock_on(data->saving_section);
        simulation_lock_off(data->computation_section);
        condition_signal(data->computation_section); //FIXME (JUST FOR TESTING)
        monitor_exit(data->mon);

        getchar();
        monitor_enter(data->mon);
        simulation_lock_on(data->computation_section);
        simulation_lock_off(data->saving_section);
        condition_signal(data->saving_section); //FIXME (JUST FOR TESTING)
        monitor_exit(data->mon);
        i++;
    }

}



#if 0
PreciseSimulation(struct List** object){

        Coordinates* objectSpeedVector = (*object)->body->speedVector;
        Coordinates* objectCoordinates    = (*object)->body->coordinates;
        int position = (*object)->position;
        Coordinates* finalSpeedVector = createCoordinateSet((*object)->body->speedVector->x,
                                                            (*object)->body->speedVector->y,
                                                            (*object)->body->speedVector->z);


        Coordinates* finalCoordinates = createCoordinateSet((*object)->body->coordinates->x,
                                                            (*object)->body->coordinates->y,
                                                            (*object)->body->coordinates->z);

      Coordinates* expectedSpeedVector = createCoordinateSet((*object)->body->speedVector->x,
                                                             (*object)->body->speedVector->y,
                                                             (*object)->body->speedVector->z);

      Coordinates* expectedCoordinates = createCoordinateSet((*object)->body->coordinates->x,
                                                             (*object)->body->coordinates->y,
                                                             (*object)->body->coordinates->z);

    Coordinates* oldAcceleration      = createCoordinateSet(0,0,0);
    Coordinates* expectedAcceleration = createCoordinateSet(0,0,0);



        //iterates untill we get to the first object
        while(1){

        while((*object)->position != position){

            // distances for each axis at moment zero.
            double rx = objectCoordinates->x -  (*object)->body->coordinates->x;
            double ry = objectCoordinates->y -  (*object)->body->coordinates->y;
            double rz = objectCoordinates->z -  (*object)->body->coordinates->z;
            //total distance
            double distance = pow(rx*rx+ry*ry+rz*rz,3/2);

            //accelleration values for each axis from starting position
            oldAcceleration->x = ((*object)->body->u * rx)/distance;
            oldAcceleration->y = ((*object)->body->u * ry)/distance;
            oldAcceleration->z = ((*object)->body->u * rz)/distance;

            //expected coordinates if no force was acting on the objects
            //TODO: should implement Runge-kutta methods
            expectedCoordinates->x = objectCoordinates->x + (objectSpeedVector->x * timeStep);
            expectedCoordinates->y = objectCoordinates->y + (objectSpeedVector->y * timeStep);
            expectedCoordinates->z = objectCoordinates->z + (objectSpeedVector->z * timeStep);


            newSpeedVector->x += ax*timeStep;
            newSpeedVector->y += ay*timeStep;
            newSpeedVector->z += az*timeStep;

            newSpeedVector->x += ax*timeStep;
            newSpeedVector->y += ay*timeStep;
            newSpeedVector->z += az*timeStep;

            }
            newCoordinates->x += newSpeedVector->x*timeStep;
            newCoordinates->y += newSpeedVector->y*timeStep;
            newCoordinates->z += newSpeedVector->z*timeStep;
            //temporization lock;
        }
}

#endif
