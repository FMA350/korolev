#ifndef SIMULATION
#define SIMULATION

void Save(struct Coordinates* coordinates, struct Coordinates* speedVector, struct Coordinates* newCoordinates, struct Coordinates* newSpeedVector);

void NewtonGravitation(struct Coordinates* myPosition, struct Coordinates* bodyPosition, struct Coordinates * newAcceleration, double u);

void Step(struct List** object, struct monitor *mon, struct condition computation_section, struct condition saving_section, void *method, int numberOfThreads);

#endif
