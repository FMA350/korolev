
int numberOfThreads;

double timeStep;

time_t SimulationTime;

int simulationIsLocked = 0;


// simulation methods


void SimpleSimulation(struct List** object, struct Monitor *mon_computation_section, struct Monitor *mon_save_section, int n_thread){


    Coordinates* objectSpeedVector      = (*object)->body->speedVector;
    Coordinates* objectCoordinates      = (*object)->body->coordinates;
    int position                        = (*object)->position;
    Coordinates* newSpeedVector = createCoordinateSet(  (*object)->body->speedVector->x,
                                                        (*object)->body->speedVector->y,
                                                        (*object)->body->speedVector->z);

    Coordinates* newCoordinates = createCoordinateSet(  (*object)->body->coordinates->x,
                                                        (*object)->body->coordinates->y,
                                                        (*object)->body->coordinates->z);

    Coordinates* newAcceleration = createCoordinateSet(0,0,0);

    while(1){
    //simulationLock
    monitor_enter();
    if(simulationIsLocked){
        condition_wait(resumeSimulation);
    }
    else{
        monitor_exit()
    }
    monitor_enter();

        //iterates untill we get to the first object

    mon_wait(mon_computation_section);
    while((*object)->position != position){

        // distances for each axis at moment zero.
        double rx = objectCoordinates->x -  (*object)->body->coordinates->x;
        double ry = objectCoordinates->y -  (*object)->body->coordinates->y;
        double rz = objectCoordinates->z -  (*object)->body->coordinates->z;
        //total distance
        double distance = pow(rx*rx+ry*ry+rz*rz,3/2);

        //accelleration values for each axis from starting position
        newAcceleration->x += ((*object)->body->u * rx)/distance;
        newAcceleration->y += ((*object)->body->u * ry)/distance;
        newAcceleration->z += ((*object)->body->u * rz)/distance;
        }
        //new coordinates using the old value for the speed.
        newCoordinates->x += (*object)->body->speedVector->x *timeStep;
        newCoordinates->y += (*object)->body->speedVector->y *timeStep;
        newCoordinates->z += (*object)->body->speedVector->z*timeStep;
        //new speedVector adding V.old + a.new *dt
        (*object)->body->speedVector->x = (*object)->body->speedVector->x + newAcceleration->x *timeStep;
        (*object)->body->speedVector->y = (*object)->body->speedVector->y + newAcceleration->y *timeStep;
        (*object)->body->speedVector->z = (*object)->body->speedVector->z + newAcceleration->z *timeStep;

        //temporization lock;
        mon_wait(mon_save_section);
        //Save new position of the objects
        (*object)->body->speedVector->x = newCoordinates->x;
        (*object)->body->speedVector->y = newCoordinates->y;
        (*object)->body->speedVector->z = newCoordinates->x;


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


Coordinates * calculateAccelerations(Coordinates* myPosition, Coordinates* bodyPosition, double bodyU){

    double rx = bodyPosition->x -  myPosition->x;
    double ry = bodyPosition->y -  myPosition->y;
    double rz = bodyPosition->z -  myPosition->z;
    //total distance
    double distance = pow(rx*rx+ry*ry+rz*rz,3/2);

    oldAcceleration->x = ((*object)->body->u * rx)/distance;
    oldAcceleration->y = ((*object)->body->u * ry)/distance;
    oldAcceleration->z = ((*object)->body->u * rz)/distance;
}
