#include <stdio.h>
#include <string.h>

#include "various.h"

static CelestialBody celestialBody[4] = {
 {
	//name             =
  "Sun",
	//mass						 =
  1000000,
	//radius           =
  6371E3, //mean radius in meters
	//geeASL           =
  9.807,		//m/s^2
	//rotationPeriod   =
  86164.090, //seconds, sideral day
	////referenceBody    = celestialBody[0][0];
	//inclination      =
  7.155, //degrees
	//eccentricity     =
  	0.0167086,
	//perihelion       =
  147095000000, //meters
	//aphelion 		     =
  152100000000, //meters
	//orbitalPeriod    =
  365.256363004, //Days
	//atmosphereHeight =
  10000,			//Kerman line
	//u 							 =
  1.32712440018E20
 },
 {
  //name             =
  "Mercury",
	//mass						 =
  5.97237E24, //5.97237×10^24
	//radius           =
  6371000, //mean radius in meters
	//geeASL           =
  9.807,		//m/s^2
	//rotationPeriod   =
  86164.090, //seconds, sideral day
	////referenceBody  = celestialBody[0][0],
	//inclination      =
  7.155, //degrees
	//eccentricity     =
  	0.0167086,
	//perihelion       =
  147095000000, //meters
	//aphelion 		     =
  152100000000, //meters
	//orbitalPeriod    =
  365.256363004, //Days
	//atmosphereHeight =
  100000,			//Kerman line
	//u 							 =
  3.986004418E14
 },
 {
	//name             =
  "Venus",
	//mass						 =
  5.97237E24, //5.97237×10^24
	//radius           =
  6371000, //mean radius in meters
	//geeASL           =
  9.807,		//m/s^2
	//rotationPeriod   =
  86164.090, //seconds, sideral day
	//referenceBody    = celestialBody[0][0];
	//inclination      =
  7.155, //degrees
	//eccentricity     =
  	0.0167086,
	//perihelion       =
  147095000000, //meters
	//aphelion 		     =
  152100000000, //meters
	//orbitalPeriod    =
  365.256363004, //Days
	//atmosphereHeight =
  100000,			//Kerman line
	//u 							 =
  3.986004418E14
 },
 {
	//name             =
   "Earth",
	//mass						 =
   5.97237E24, //5.97237×10^24
	//radius           =
   6371000, //mean radius in meters
	//geeASL           =
   9.807,		//m/s^2
	//rotationPeriod   =
   86164.090, //seconds, sideral day
	//referenceBody    = celestialBody[0][0];
	//inclination      =
   7.155, //degrees
	//eccentricity     =
   	0.0167086,
	//perihelion       =
   147095000000,//meters
	//aphelion 		     =
   152100000000, //meters
	//orbitalPeriod    =
   365.256363004, //Days
	//atmosphereHeight =
   100000,			//Kerman line
	//u 							 =
   3.986004418E14
 }
};

double RequestDouble(double min, double max, char* message){
		printf("**Program Request**\n"
					  "Double Value Required: %s \n"
						"Values expected: Min: %g, Max: %g \n", message, min, max);
		char *line = NULL;
		size_t len = 64;
		ssize_t read;
		double value =0;
		while(1){
			printf("_-_-_ Insert value: ");
			read = getline(&line, &len, stdin);
      int arguments = sscanf(line, "%lg", &value);
			if ((value >= min)&&(value <= max)&&(arguments==1)){
				printf("**Value %G is Acceptable**\n", value);
				return value;
			}
			printf(KERROR"**ERROR-ERROR-ERROR Value was not acceptable\n"KNORMAL);
		}
}

CelestialBody RequestCelestialBody(int celestialBodyClass, char* message){
	//celestialBodyClass will specify what kind of class if acceptable.
	//special values may be implemented to specifiy certain classes but not others
	printf("**Program Request**\n"
					"Celestial Body Required: %s \n"
					"Class expected: %i \n", message, celestialBodyClass);
	char *line = NULL;
	size_t len = 64;
	ssize_t read;
	double value;
	while(1){
		printf("_-_-_ Insert value: ");
		read = getline(&line, &len, stdin);
    printf("getline contains: %s\n",line);
    strtok(line, "\n");
		for (int i = 0; i < sizeof(celestialBody)/sizeof(CelestialBody); i++){
        if(strcmp(line, celestialBody[i].name) == 0){
					printf("**Value Acceptable**\n");
          printf("Celestial Body choosen: %s\n", celestialBody[i].name);
					return celestialBody[i];
				}
		}
		printf(KERROR"**ERROR-ERROR-ERROR Value was not acceptable\n"KNORMAL);
	}
}

void PrintHelloMessage(){

  printf(KOUTPUT"\n\n\n                                               Korolev Space Guidance System 0.0.1\n\n\n"KEVIDENCE
  "                  :\n"
  "                  :\n"
  "                  :\n"
  "                  :\n"
  "   .              :\n"
  "    '.            :           .'\n"
  "      '.          :         .'\n"
  "        '.   .-''''''-.   .'                                   .'':\n"
  "          '.'          '.'                               .-''''-.'         .---.          .----.        .-'''-.\n"
  "           :            :                _    _        .'     .' '.    ...'     '...    .'      '.    .'       '.\n"
  "   .........            .........    o  (_)  (_)  ()   :    .'    :   '..:.......:..'   :        :    :         :   o\n"
  "           :            :                              :  .'      :       '.....'       '.      .'    '.       .'\n"
  "            :          :                             .'.'.      .'                        `''''`        `'''''`\n"
  "             '........'                              ''   ``````\n"
  "            .'    :   '.\n"
  "          .'      :     '.\n"
  "        .'        :       '.\n"
  "      .'          :         '.\n"
  "         Dana'97  :\n"
  "                  :\n"
  "                  :\n"
  "                  :\n"KNORMAL);
}
