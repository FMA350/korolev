#include <stdio.h>
#include "various.h"

CelestialBody celestialBody = {
 {
	name             = "Sun";
	mass						 = 1000000;
	radius           = 6371000; //mean radius in meters
	geeASL           = 9.807;		//m/s^2
	rotationPeriod   = 86164.090; //seconds, sideral day
	//referenceBody    = celestialBody[0][0];
	inclination      = 7.155; //degrees
	eccentricity     = 	0.0167086;
	perihelion       = 147095000000; //meters
	aphelion 		     = 152100000000; //meters
	orbitalPeriod    = 365.256363004; //Days
	atmosphereHeight = 100000;			//Kerman line
	u 							 = 1.32712440018E20;
 },
 {
	name             = "Mercury";
	mass						 = 5.97237E24; //5.97237×10^24
	radius           = 6371000; //mean radius in meters
	geeASL           = 9.807;		//m/s^2
	rotationPeriod   = 86164.090; //seconds, sideral day
	//referenceBody    = celestialBody[0][0];
	inclination      = 7.155; //degrees
	eccentricity     = 	0.0167086;
	perihelion       = 147095000000; //meters
	aphelion 		     = 152100000000; //meters
	orbitalPeriod    = 365.256363004; //Days
	atmosphereHeight = 100000;			//Kerman line
	u 							 = 3.986004418E14;
 },
 {
	name             = "Venus";
	mass						 = 5.97237E24; //5.97237×10^24
	radius           = 6371000; //mean radius in meters
	geeASL           = 9.807;		//m/s^2
	rotationPeriod   = 86164.090; //seconds, sideral day
	//referenceBody    = celestialBody[0][0];
	inclination      = 7.155; //degrees
	eccentricity     = 	0.0167086;
	perihelion       = 147095000000; //meters
	aphelion 		     = 152100000000; //meters
	orbitalPeriod    = 365.256363004; //Days
	atmosphereHeight = 100000;			//Kerman line
	u 							 = 3.986004418E14;
 },
 {
	name             = "Earth";
	mass						 = 5.97237E24 ; //5.97237×10^24
	radius           = 6371000; //mean radius in meters
	geeASL           = 9.807;		//m/s^2
	rotationPeriod   = 86164.090; //seconds, sideral day
	//referenceBody    = celestialBody[0][0];
	inclination      = 7.155; //degrees
	eccentricity     = 	0.0167086;
	perihelion       = 147095000000; //meters
	aphelion 		     = 152100000000; //meters
	orbitalPeriod    = 365.256363004; //Days
	atmosphereHeight = 100000;			//Kerman line
	u 							 = 3.986004418E14;
 }
}

double RequestDouble(double min, double max, char* message){
		printf("**Program Request**\n"
					  "Double Value Required: %s \n"
						"Values expected: Min: %d, Max: %d \n", message, min, max);
		char *line = NULL;
		size_t len = 64;
		ssize_t read;
		double value;
		while(true){
			printf("_-_-_ Insert value: ");
			read = getline(&line, &len, stdin);
			double acceptableValue = sscanf(line, "f", value);
			if ((value >= min)&&(value <=max)){
				printf("**Value is Acceptable**\n");
				return value;
			}
			printf("**ERROR-ERROR-ERROR Value was not acceptable\n");
		}
		return 0.0;
}

CelestialBody requestCelestialBody(int celestialBodyClass, char* message){
	//celestialBodyClass will specify what kind of class if acceptable.
	//special values may be implemented to specifiy certain classes but not others
	printf("**Program Request**\n"
					"Celestial Body Required: %s \n"
					"Class expected: %d \n", message, celestialBodyClass);
	char *line = NULL;
	size_t len = 64;
	ssize_t read;
	double value;
	while(true){
		printf("_-_-_ Insert value: ");
		read = getline(&line, &len, stdin);
		//TODO: change sizeOf cause it cannot be for all the different obj classes (stupid c)
		for (int i = 0; i < sizeof(celestialBody[celestialBodyClass])/sizeof(CelestialBody); i++){
				if(strcmp(line, celestialBody[celestialBodyClass][i].name)){
					printf("**Value Acceptable**\n");
					return celestialBody[celestialBodyClass][i];
				}
		}
		printf("**ERROR-ERROR-ERROR Value was not acceptable\n");
	}
}
