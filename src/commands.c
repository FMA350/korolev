//Various
#include <stdio.h>
#include <float.h>
#include <string.h>
#include <limits.h>

//Header
#include "commands.h"

#include "error.h"
#include "various.h"
#include "orbitalFunctions.h"
#include "data.h"
#include "monitor.h"
#include "simulation.h"

extern struct List * celestialBodiesHead;
extern int sim_iteration;
extern int start_iteration;

const char * DELIMITER 	= " ";		//To delimit the text given by the user
const int PARTITION_PARTS = 10; //to choose in how many parts the the orbit should be partitioned in.

//COMMANDS


int ExecuteCommand(char* line){
	char *commandToken = line;

	if(strcmp(commandToken, EXIT_COMMAND) == 0){
		return EXIT;
	}

  if(strcmp(commandToken, CALCULATE_ORBITAL_VELOCITY) == 0){
		return OrbitalVelocityCommand();
  }

	if(strcmp(commandToken, ADD_CELESTIAL_BODY) == 0){
		return AddCelestialBody(&celestialBodiesHead);
	}

	if(strcmp(commandToken, SHOW_CELESTIAL_OBJECTS) == 0){
		PrintAllBodies(celestialBodiesHead);
		return 0;
	}

	if(strcmp(commandToken, HELP) == 0){
		PrintAllCommands();
		return 0;
	}

	if(strcmp(commandToken, REMOVE_CELESTIAL_BODY) == 0){
		return RemoveCelestialBody(&celestialBodiesHead);
	}

	if(strcmp(commandToken, CALCULATE_DELTA_V) == 0){
		return CalculateDeltaV();
	}
	if(strcmp(commandToken, SAVE_SIMULATION) == 0){
		return SaveSimulationData(RequestString("Name of the save file",0),&celestialBodiesHead);

	}
	if(strcmp(commandToken, LOAD_SIMULATION) == 0){
		return LoadSimulationData(RequestString("Name of the save file",0),&celestialBodiesHead);
	}
	if(strcmp(commandToken, INFO) == 0){
	 	PrintDetails(RequestString("Name of the planet",0),celestialBodiesHead);
		return 0;
	}
	if(strcmp(commandToken, SIMULATE) == 0){
		return Simulate(&celestialBodiesHead);
	}
	else{
		//command was not regognized, return 99.
		return ERROR_COMMAND_NOT_RECOGNIZED;
	}
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
	printf(KEVIDENCE"		***List of all commands***\n");
	printf(KEVIDENCE"CALCULATE_ORBITAL_VELOCITY "KDATA"= %s\n",CALCULATE_ORBITAL_VELOCITY);
	printf(KEVIDENCE"EXIT_COMMAND "KDATA"= %s\n",EXIT_COMMAND);
	printf(KEVIDENCE"HELP: displays this guide "KDATA"= %s\n",HELP);
	printf(KEVIDENCE"CALCULATE_DELTA_V "KDATA"= %s\n",CALCULATE_DELTA_V);
	printf(KEVIDENCE"SHOW_CELESTIAL_OBJECTS: displays all the objects in the main list "KDATA"= %s\n",SHOW_CELESTIAL_OBJECTS);
	printf(KEVIDENCE"INFO: shows details of an object, like its coordinates vector "KDATA"= %s\n",INFO);
	printf(KEVIDENCE"SAVE_SIMULATION "KDATA"= %s\n",SAVE_SIMULATION);
	printf(KEVIDENCE"LOAD_SIMULATION "KDATA"= %s\n",LOAD_SIMULATION);
	printf(KEVIDENCE"ADD_CELESTIAL_BODY"KDATA"=%s\n"KNORMAL,ADD_CELESTIAL_BODY);
	printf(KEVIDENCE"REMOVE_CELESTIAL_BODY"KDATA"=%s\n"KNORMAL,REMOVE_CELESTIAL_BODY);
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

int Simulate(struct List** celestialBodiesHead){
	start_iteration = sim_iteration;
	int threadNumber = GetListSize(*celestialBodiesHead);
	if(threadNumber == 0){
		printf(KERROR"No objects in the list, aborting!"KNORMAL);
		return ERROR_LIST_EMPTY;
	}
	SetToBeginning(celestialBodiesHead);
	printf("Initial system energy: %G", CalculateSystemEnergy((*celestialBodiesHead)));

	ThreadData *data = malloc(sizeof(ThreadData)*threadNumber);
	int max_iteration = RequestInt(0, INT_MAX, "Insert the iterations of the simulation");
	char * simulationName = RequestString("Name of the simulation",0);
	PrintHeader(simulationName, celestialBodiesHead); //adds .slog to simulationName
	monitor mon = monitor_create();
	//conditions are created with all locks on.
	condition computation_section = condition_create(mon, threadNumber-1, sim_iteration, sim_iteration+max_iteration);
	condition saving_section	  = condition_create(mon, threadNumber-1, sim_iteration, sim_iteration+max_iteration);

	//FIXME: this works both with and without malloc.
	//in both cases, the addresses of pthreads are the same.
	//Needs further investigation.
	pthread_t * td = malloc(sizeof(pthread_t)*(threadNumber+1));
	SetToBeginning(celestialBodiesHead);
	for(int i = 0; i < threadNumber; i++){
		data[i].simulationName = simulationName;
		data[i].object = (*celestialBodiesHead);
		data[i].body   =  &(*(*celestialBodiesHead)->body);
		printf("in commands, passing body: %s\n",data[i].body->name);
		data[i].mon    = mon;
		data[i].computation_section = computation_section;
		data[i].saving_section      = saving_section;

		data[i].numberOfThreads = threadNumber;
		data[i].thread_id = i;

		if(pthread_create(&td[i], NULL, &SimulationMain, &data[i])){
		  //Could not create the thread
		  printf(KERROR "***ERROR-ERROR-ERROR***\n"
		                "   Internal program Error, thread could not lanch \n" KNORMAL);
		  exit(1);
		}
		else{
			printf("thread %d body %s OK \n\n", data[i].thread_id, data[i].body->name);
			(*celestialBodiesHead) = (*celestialBodiesHead)->next;
		}
	}
	//launch simulation commander
	//note: this thread uses the data of the last created thread, since it just
	//needs an hook for the conditions and the monitor
	//SimulationCommander() is found in simulation.c

	if(pthread_create(&td[threadNumber], NULL, &SimulationCommander, &data[0])){
	  //Could not create the thread
	  printf(KERROR "***ERROR-ERROR-ERROR***\n"
					"   Internal program Error, thread could not lanch \n" KNORMAL);
	  exit(1);
	}
	printf("Simulation bootstrap completed...\n\n\n\n\n");

	pthread_join(td[threadNumber],NULL);
	printf(KDATA"simulation over at step %d\n\n\n"KNORMAL, condition_current_iteration(data->computation_section));


	condition_destroy(computation_section);
	condition_destroy(saving_section);
	monitor_destroy(mon);
	free(data);
	free(td);
	printf(KDATA"cleanup completed\n\n\n" KNORMAL);
	printf("final system energy: %G", CalculateSystemEnergy((*celestialBodiesHead)));
	return 0;
}
