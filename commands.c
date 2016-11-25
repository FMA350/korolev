//Various
#include <stdio.h>
#include <float.h>
#include <string.h>

//Header
#include "commands.h"
#include "error.h"
#include "various.h"
#include "orbitalFunctions.h"


//COMMANDS
const char * CALCULATE_ORBITAL_VELOCITY   = "orbitalVelocity\n";   //launches the function to calculate the orbital velocity at a given position in                                                               //in the orbit.
const char * EXIT_COMMAND	                = "exit\n";	             //Command to leave.
const char * HELP													= "help\n";							 //Gives a fast overview and displays all the commands.


const char * DELIMITER 	= " ";		//To delimit the text given by the user
const int PARTITION_PARTS = 10; //to choose in how many parts the the orbit should be partitioned in.

void* ExecuteCommand(void* d){
	ThreadData *data = (ThreadData*)d;
	
	char * commandToken;
	commandToken = (char*) strtok((char*)data->line, DELIMITER);

	if(strcmp(commandToken, EXIT_COMMAND) == 0){
		//retrun the close value;
		printf("EXITEXITEXIT\n");
		data->returnCode = (int*)EXIT;
		printf("%d\n", data->returnCode);
		return NULL;
	}

  if(strcmp(commandToken, CALCULATE_ORBITAL_VELOCITY) == 0){
		data->returnCode = (int*)OrbitalVelocityCommand();
		return NULL;
  }
	else{
		//command was not regognized, return 99.
		data->returnCode =  (int*)ERROR_COMMAND_NOT_RECOGNIZED;
		return NULL;
	}
  return NULL;
 }



int OrbitalVelocityCommand(){
 //(double distance, double aphelion, double perihelion, double u)
 double distance;
 double aphelion;
 double perihelion;
 CelestialBody referenceBody;
	//NOTE: Here a method returning a super class would be so damn sweet!
	perihelion = RequestDouble(0, DBL_MAX, "Please Insert perihelion value for the desired orbit");
	aphelion   = RequestDouble(perihelion, DBL_MAX, "Please Insert Aphelion value for the desired orbit");
  distance	 = RequestDouble(perihelion, aphelion, "Please insert the distance the ship is right now from the orbiting body");
	referenceBody = RequestCelestialBody(0, "Please insert the referenceBody");
	double speed = CalculateOrbitalSpeed(distance, aphelion, perihelion, referenceBody.u);
	printf(KOUTPUT"**PROGRAM OUTPUT:**\n");
	printf("  The orbital speed for the distance" KDATA " %g m" KOUTPUT " is : "KDATA"%g"KOUTPUT" m/s \n",distance, speed);
	printf("  The orbital speed at the aphelion is : "KDATA"%g"KOUTPUT" m/s \n", CalculateOrbitalSpeed(aphelion, aphelion, perihelion, referenceBody.u));
	printf("  The orbital speed at the perihelion is : "KDATA"%g"KOUTPUT" m/s \n", CalculateOrbitalSpeed(perihelion, aphelion, perihelion, referenceBody.u));
	double h = (aphelion - perihelion)/(double)PARTITION_PARTS;
	printf("Showing different speeds for different places in the orbit\n"
	      "Jumping of %g m (meters) every time\n",h);
	double newDistance;
	for(int i = 1; i< PARTITION_PARTS; i++){
		newDistance = (perihelion + h*i);
		printf(KOUTPUT"  The orbital speed at the distance "KDATA"%g"KOUTPUT" is : "KDATA"%g"KOUTPUT" m/s \n"KNORMAL, newDistance, CalculateOrbitalSpeed(newDistance, aphelion, perihelion, referenceBody.u));
	}
return 0;
}
