#ifndef SIMULATION
#define SIMULATION


void Euler(struct List* object, struct Coordinates* newCoordinates, struct Coordinates* newSpeedVector, struct Coordinates* newAcceleration);

void RungeKutta(struct List* object, struct Coordinates* newCoordinates, struct Coordinates* newSpeedVector,struct Coordinates* newAcceleration);

void Save(struct Coordinates* coordinates, struct Coordinates* speedVector, struct Coordinates* newCoordinates, struct Coordinates* newSpeedVector);

void NewtonGravitation(struct Coordinates* myPosition, struct Coordinates* bodyPosition, struct Coordinates* newAcceleration, double u);

void* SimulationMain(void* data);

void* SimulationCommander(void* data);


#endif
