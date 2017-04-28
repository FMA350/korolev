//Various
#include <stdio.h>
#include <float.h>
#include <string.h>

//Header
#include "commands.h"

#include "error.h"
#include "various.h"
#include "orbitalFunctions.h"
#include "data.h"

extern struct List * celestialBodiesHead;
//extern int SaveSimulationData(char * simulationName, struct List** list);
//extern int LoadSimulationData(char * simulationName, struct List** list);

const char * DELIMITER 	= " ";		//To delimit the text given by the user
const int PARTITION_PARTS = 10; //to choose in how many parts the the orbit should be partitioned in.

//COMMANDS


void* ExecuteCommand(void* d){
	ThreadData *data = (ThreadData*)d;
	char * commandToken;
	commandToken = (char*) strtok((char*)data->line, DELIMITER); //FIXME

	if(strcmp(commandToken, EXIT_COMMAND) == 0){
		data->returnCode = (int)EXIT;
		return NULL;
	}

  if(strcmp(commandToken, CALCULATE_ORBITAL_VELOCITY) == 0){
		int error = OrbitalVelocityCommand();
		data->returnCode = (int)error; //TODO: solve the warning
		return NULL;
  }

	if(strcmp(commandToken, ADD_CELESTIAL_BODY) == 0){
		AddCelestialBody(&celestialBodiesHead);
		data->returnCode = 0;
		return NULL;
	}

	if(strcmp(commandToken, SHOW_CELESTIAL_OBJECTS) == 0){
		PrintAllBodies(celestialBodiesHead);
		data->returnCode = 0; //TODO: solve il warning
		return NULL;
	}

	if(strcmp(commandToken, HELP) == 0){
		PrintAllCommands();
		data->returnCode = 0; //TODO: solve il warning
		return NULL;
	}

	if(strcmp(commandToken, REMOVE_CELESTIAL_BODY) == 0){
		int error = RemoveCelestialBody(&celestialBodiesHead);
		data->returnCode = (int)error;
		return NULL;
	}

	if(strcmp(commandToken, CALCULATE_DELTA_V) == 0){
		int error = CalculateDeltaV();
		data->returnCode = (int)error;
		return NULL;
	}
	if(strcmp(commandToken, SAVE_SIMULATION) == 0){
		int error = SaveSimulationData(RequestString("Name of the save file",0),&celestialBodiesHead);
		data->returnCode = (int)error;
		return NULL;
	}
	if(strcmp(commandToken, LOAD_SIMULATION) == 0){
		int error = LoadSimulationData(RequestString("Name of the save file",0),&celestialBodiesHead);
		data->returnCode = (int)error;
		return NULL;
	}
	if(strcmp(commandToken, SHOW_DETAILS) == 0){
	 	PrintDetails(RequestString("Name of the planet",0),celestialBodiesHead);
		data->returnCode = 0;
		return NULL;
	}


	else{
		//command was not regognized, return 99.
		data->returnCode =  (int)ERROR_COMMAND_NOT_RECOGNIZED;
		return NULL;
	}
  return NULL;
 }

int OrbitalVelocityCommand(){
 //(double distance, double aphelion, double perihelion, double u)
 double distance;
 double aphelion;
 double perihelion;
 CelestialBody* referenceBody;
	//NOTE: Here a method returning a super class would be so damn sweet!
	perihelion = RequestDouble(0, DBL_MAX, "Please Insert perihelion value for the desired orbit");
	aphelion   = RequestDouble(perihelion, DBL_MAX, "Please Insert Aphelion value for the desired orbit");
  distance	 = RequestDouble(perihelion, aphelion, "Please insert the distance the ship is right now from the orbiting body");
	referenceBody = RequestCelestialBody("Please insert the referenceBody",celestialBodiesHead);
	double speed = CalculateOrbitalSpeed(distance, aphelion, perihelion, referenceBody->u);
	printf(KOUTPUT"**PROGRAM OUTPUT:**\n");
	printf("  The orbital speed for the distance" KDATA " %g m" KOUTPUT " is : "KDATA"%g"KOUTPUT" m/s \n",distance, speed);
	printf("  The orbital speed at the aphelion is : "KDATA"%g"KOUTPUT" m/s \n", CalculateOrbitalSpeed(aphelion, aphelion, perihelion, referenceBody->u));
	printf("  The orbital speed at the perihelion is : "KDATA"%g"KOUTPUT" m/s \n", CalculateOrbitalSpeed(perihelion, aphelion, perihelion, referenceBody->u));
	double h = (aphelion - perihelion)/(double)PARTITION_PARTS;
	printf("Showing different speeds for different places in the orbit\n"
	      "Jumping of %g m (meters) every time\n",h);
	double newDistance;
	for(int i = 1; i< PARTITION_PARTS; i++){
		newDistance = (perihelion + h*i);
		printf(KOUTPUT"  The orbital speed at the distance "KDATA"%g"KOUTPUT" is : "KDATA"%g"KOUTPUT" m/s \n"KNORMAL, newDistance, CalculateOrbitalSpeed(newDistance, aphelion, perihelion, referenceBody->u));
	}
 return 0;
}

int CalculateDeltaV(){
	//TODO
}

void PrintAllCommands(){
	printf(KEVIDENCE"List of all commands:");
	printf(KEVIDENCE"CALCULATE_ORBITAL_VELOCITY "KDATA"= %s",CALCULATE_ORBITAL_VELOCITY);
	printf(KEVIDENCE"EXIT_COMMAND "KDATA"= %s",EXIT_COMMAND);
	printf(KEVIDENCE"HELP "KDATA"= %s",HELP);
	printf(KEVIDENCE"CALCULATE_DELTA_V "KDATA"= %s",CALCULATE_DELTA_V);
	printf(KEVIDENCE"SHOW_CELESTIAL_OBJECTS "KDATA"= %s",SHOW_CELESTIAL_OBJECTS);
	printf(KEVIDENCE"SAVE_SIMULATION "KDATA"= %s",SAVE_SIMULATION);
	printf(KEVIDENCE"LOAD_SIMULATION "KDATA"= %s",LOAD_SIMULATION);
	printf(KEVIDENCE"ADD_CELESTIAL_BODY"KDATA"=%s"KNORMAL,ADD_CELESTIAL_BODY);
	printf(KEVIDENCE"REMOVE_CELESTIAL_BODY"KDATA"=%s"KNORMAL,REMOVE_CELESTIAL_BODY);
}

int AddCelestialBody(struct List **head){
	PrintAllBodies(*head);
	CelestialBody *referenceBody = RequestCelestialBody  ("CelestialBody name, type null for no reference body.",(*head));
	char *name  = RequestString("CelestialBody Name", 0);
	double mass = RequestDouble(-DBL_MAX, DBL_MAX, "Mass of the CelestialBody");
	struct Coordinates *positionVector = RequestCoordinateSet("Position of the CelestialBody(meters)");
	struct Coordinates *speedVector    = RequestCoordinateSet("Velocity vector for the CelestialBody (m/s)");
	struct List* list = CreateListElement();
	CreateCelestialBody(list, referenceBody, name, mass, positionVector, speedVector);
	printf("Creation of body %s succesful, adding it to the list\n", list->body->name);
	PushElement(head, &list);
	PrintAllBodies(*head);
	printf("Done\n");
	return 0;
}

int RemoveCelestialBody(struct List **list){
	char *name  = RequestString("Name of the celestial body to remove",0);
	struct List * element = GetList(name, (*list)); //FIXME
	if(element == NULL){
		printf("%s could not be found! aborting",name);
		return ERROR_BODY_NOT_FOUND;
	}
	else{
		int error = RemoveList(&element);
		PrintAllBodies(element);
		(*list) = element;
		return error;
	}
}
