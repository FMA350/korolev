#ifndef COMMANDS
#define COMMANDS

#include "various.h"

#define CALCULATE_ORBITAL_VELOCITY  "orbitalVelocity"   //launches the function to calculate the orbital velocity at a given position in                                                               //in the orbit.
#define EXIT_COMMAND	         	"exit"	             //Command to leave.
#define HELP						"help"	//Gives a fast overview and displays all the commands.
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
