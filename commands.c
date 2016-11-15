//Various
#include <stdio.h>
#include <float.h>

//Header
#include "commands.h"
#include "error.h"
#include "various.h"



//COMMANDS
const char * CALCULATE_ORBITAL_VELOCITY   = "orbitalVelocity\n";   //launches the function to calculate the orbital velocity at a given position in                                                               //in the orbit.
const char * EXIT_COMMAND	                = "exit\n";	             //Command to leave.
const char * HELP													= "help\n";							 //Gives a fast overview and displays all the commands.


const char * DELIMITER 	= " ";		//To delimit the text given by the user
const int PARTITION_PARTS = 10; //to choose in how many parts the the orbit should be partitioned in.

int ExecuteCommand(char* line){
	//TODO: Segmentation fault.
	//parse the input, then check if the syntax is correct
	char * commandToken;
	commandToken = strtok(line, DELIMITER);
  //EXIT Check
	if(strcmp(commandToken, EXIT_COMMAND) == 0){
		//retrun the close value;
		return EXIT;
	}

  if(strcmp(commandToken, CALCULATE_ORBITAL_VELOCITY) == 0){
		int error = OrbitalVelocityCommand();
		return error;
  }
	else{
		//command was not regognized, return 99.
		return ERROR_COMMAND_NOT_RECOGNIZED;
	}
  return 0;
 }

int OrbitalVelocityCommand(){
 //(double distance, double aphelion, double perihelion, double u)
 double distance;
 double aphelion;
 double perihelion;
 CelestialBody referenceBody;
	//NOTE: Here a method returning a super class would be so damn sweet!
	perihelion = requestDouble(0, DBL_MAX, "Please Insert perihelion value for the desired orbit");
	aphelion   = requestDouble(perihelion, DBL_MAX, "Please Insert Aphelion value for the desired orbit");
  distance	 = requestDouble(perihelion, aphelion, "Please insert the distance the ship is right now from the orbiting body");
	referenceBody = requestCelestialBody( "Please insert the referenceBody");
	double speed = CalculateOrbitalSpeed(distance, aphelion, perihelion, referenceBody.u);
	printf("**PROGRAM OUTPUT:**\n");
	printf("  The orbital speed for the distance %d is : %d \n",distance, speed);
	printf("  The orbital speed at the aphelion is : %d \n", CalculateOrbitalSpeed(aphelion, aphelion, perihelion, celestialBody.u));
	printf("  The orbital speed at the perihelion is : %d \n", CalculateOrbitalSpeed(perihelion, aphelion, perihelion, celestialBody.u));
	double h = (aphelion - perihelion)/PARTITION_PARTS;
	for(int i = 1; i< PARTITION_PARTS; i++){
		printf("  The orbital speed at the distance %d is : %d \n",(aphelion + h*i), CalculateOrbitalSpeed(aphelion + h*i, aphelion, perihelion, celestialBody.u));
	}
return 0;
}
