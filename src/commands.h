#ifndef COMMANDS
#define COMMANDS

#include "various.h"

#define CALCULATE_ORBITAL_VELOCITY  "orbitalVelocity\n"   //launches the function to calculate the orbital velocity at a given position in                                                               //in the orbit.
#define EXIT_COMMAND	         			"exit\n"	             //Command to leave.
#define HELP									 			"help\n"		//Gives a fast overview and displays all the commands.
#define CALCULATE_DELTA_V		   			"deltaV\n"
#define SHOW_CELESTIAL_OBJECTS 			"showObjects\n"
#define SAVE_SIMULATION    		 			"save\n"
#define LOAD_SIMULATION				 			"load\n"
#define ADD_CELESTIAL_BODY		 			"addBody\n"
#define REMOVE_CELESTIAL_BODY				"removeBody\n"
#define SHOW_DETAILS                        "info\n"

void* ExecuteCommand(void* line);

int OrbitalVelocityCommand();

int CalculateDeltaV();

void PrintAllCommands();

int AddCelestialBody(struct List **head);

int RemoveCelestialBody(struct List **list);

#endif
