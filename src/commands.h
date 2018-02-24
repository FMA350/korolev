#ifndef COMMANDS
#define COMMANDS

#include "various.h"

typedef struct SimulationThreadData{
    char *simulationName; //for saving purposes.
    struct List* object;
    struct CelestialBody* body;

    monitor mon;
    condition computation_section;
    condition saving_section;

    int numberOfThreads;
    int thread_id;

    //auxiliary coordinates for the simulation
    struct Coordinates* newVelocity;
    struct Coordinates* newAcceleration;
    struct Coordinates* newCoordinates;
    //only used by Runge-Kutta
    struct Coordinates* k1r;
    struct Coordinates* k2r;
    struct Coordinates* k3r;
    struct Coordinates* k4r;
    struct Coordinates* k1v;
    struct Coordinates* k2v;
    struct Coordinates* k3v;
    struct Coordinates* k4v;
    //only used by Euler
    struct Coordinates* temp;
}SimulationThreadData;

int CreateSimulationThreadData(SimulationThreadData* data, char* simulationName, struct List** celestialBodiesHead, monitor mon, condition computation_section, condition saving_section, int threadNumber, int i);

#define CALCULATE_ORBITAL_VELOCITY  "orbitalVelocity"   //launches the function to calculate the orbital velocity at a given position in                                                               //in the orbit.
#define EXIT_COMMAND	         	"exit"	            //Command to leave.
#define HELP						"help"	            //Gives a fast overview and displays all the commands.
#define CALCULATE_DELTA_V		   	"deltaV"
#define SHOW_CELESTIAL_OBJECTS 		"showObjects"
#define SAVE_SIMULATION    		 	"save"
#define LOAD_SIMULATION				"load"
#define ADD_CELESTIAL_BODY		 	"addBody"
#define REMOVE_CELESTIAL_BODY		"removeBody"
#define INFO                        "info"
#define SIMULATE                    "simulate"
#define CHANGEMODE                  "changeMode"

int ExecuteCommand(char* line);

int OrbitalVelocityCommand();

int CalculateDeltaV();

void PrintAllCommands();

int AddCelestialBody(struct List **head);

int RemoveCelestialBody(struct List **list);

int Simulate(struct List** celestialBodiesHead);

#endif
