double timeStep = 216000; //seconds

//time_t SimulationTime;

// simulation methods

void Save(struct Coordinates* coordinates, struct Coordinates* speedVector, struct Coordinates* newCoordinates, struct Coordinates* newSpeedVector){
    coordinates->x = newCoordinates->x;
    coordinates->y = newCoordinates->y;
    coordinates->z = newCoordinates->x;

    speedVector->x = newSpeedVector->x;
    speedVector->y = newSpeedVector->y;
    speedVector->z = newSpeedVector->x;
}

void NewtonGravitation(struct Coordinates* myPosition, struct Coordinates* bodyPosition, struct Coordinates * newAcceleration, double u){

    double rx = bodyPosition->x -  myPosition->x;
    double ry = bodyPosition->y -  myPosition->y;
    double rz = bodyPosition->z -  myPosition->z;
    //total distance
    double distance = pow(rx*rx+ry*ry+rz*rz,3/2);

    newAcceleration->x += ((*object)->body->u * rx)/distance;
    newAcceleration->y += ((*object)->body->u * ry)/distance;
    newAcceleration->z += ((*object)->body->u * rz)/distance;
}


void Step(struct List** object, struct monitor *mon, struct condition computation_section, struct condition saving_section, void *method, int numberOfThreads){

        Coordinates* objectSpeedVector      = (*object)->body->speedVector;
        Coordinates* objectCoordinates      = (*object)->body->coordinates;

        Coordinates* newSpeedVector = createCoordinateSet(  (*object)->body->speedVector->x,
                                                            (*object)->body->speedVector->y,
                                                            (*object)->body->speedVector->z);

        Coordinates* newCoordinates = createCoordinateSet(  (*object)->body->coordinates->x,
                                                            (*object)->body->coordinates->y,
                                                            (*object)->body->coordinates->z);

        Coordinates* newAcceleration = createCoordinateSet(0,0,0);

    while(1){
        //simulationLock
        monitor_enter(mon);
        if(condition_check(computation_section, numberOfThreads-1)){
            condition_signal(computation_section);
            monitor_exit(mon);
        }
        else{
            condition_wait(computation_section);
        }
        //FIXME
        method(object, newCoordinates, newSpeedVector, objectCoordinates, newAcceleration); //calculates new accelerations

        monitor_enter(mon);
        if(condition_check(saving_section, numberOfThreads-1)){
            condition_signal(saving_section);
            monitor_exit(mon);
        }
        else{
            condition_wait(saving_section);
        }
        save(objectCoordinates, objectSpeedVector, newCoordinates, newSpeedVector);
    }
}

void SimpleSimulation(struct List** object, struct Coordinates* newCoordinates, struct Coordinates* newSpeedVector, struct Coordinates* objectCoordinates,struct Coordinates* newAcceleration){
    //it does not conserve the energy!
    int position = (*object)->position;
    (*object) = (*object)->next;
    while((*object)->position != position){
        NewtonGravitation(objectCoordinates, (*object)->body->coordinates, newAcceleration, (*object)->body->u);
        (*object) = (*object)->next;
        }
        //new coordinates using the old value for the speed.
        newCoordinates->x += (*object)->body->speedVector->x *timeStep;
        newCoordinates->y += (*object)->body->speedVector->y *timeStep;
        newCoordinates->z += (*object)->body->speedVector->z*timeStep;
        //new speedVector adding V.old + a.new *dt
        newSpeedVector->x = (*object)->body->speedVector->x + newAcceleration->x *timeStep;
        newSpeedVector->y = (*object)->body->speedVector->y + newAcceleration->y *timeStep;
        newSpeedVector->z = (*object)->body->speedVector->z + newAcceleration->z *timeStep;

        newAcceleration->x = 0;
        newAcceleration->y = 0;
        newAcceleration->z = 0;
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
