#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

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

struct Coordinates* BuildCoordinatesSet(char* message){
  printf("**Program Request**\n"
         "BuildCoordinatesSet function\n"
         "%s\n", message);
         struct Coordinates* coordinates = (struct Coordinates*)malloc(sizeof(struct Coordinates));
         coordinates->x = RequestDouble(-DBL_MAX, DBL_MAX, "x");
         coordinates->y = RequestDouble(-DBL_MAX, DBL_MAX, "y");
         coordinates->z = RequestDouble(-DBL_MAX, DBL_MAX, "z");
         return coordinates;
}

CelestialBody* RequestCelestialBody(int celestialBodyClass, char* message){
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
          //TODO: CHECK that & works.
          return &celestialBody[i];
				}
		}
		printf(KERROR"**ERROR-ERROR-ERROR Value was not acceptable\n"KNORMAL);
	}
}

char* RequestString(char* message){
  printf("**Program Request**\n"
					"String requeired Required: %s \n", message);
  char *line = NULL;
  size_t len = 64;
  ssize_t read;
  double value;
  printf("_-_-_ Insert value: ");
  read = getline(&line, &len, stdin);
  printf("getline contains: %s\n",line);
  strtok(line, "\n");
  return line;
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

//OBJECT CONTROL METHODS


//LIST CONTROL METHODS
ObjectList* CreateObjectList(){
  //return a pointer to an alloccated Object List space.
  ObjectList* list = ( ObjectList*)malloc(sizeof(ObjectList));
  return list;
}

struct Element* CreateElement(){
  //returns a pointer to an alloccated Element space
  struct Element* element = (struct Element*)malloc(sizeof(struct Element));
  return element;
}

struct Object* CreateObject(){
  struct Object* object = (struct Object*)malloc(sizeof(struct Object));
  object->name = RequestString("Object Name");
  object->referenceBody = RequestCelestialBody(0, "Reference body for the object");
  object->coordinates = BuildCoordinatesSet("Position of the object(meters)");
  object->speedVector = BuildCoordinatesSet("Velocity vector for the Object (m/s) with respect to the reference Object");
  object->mass = RequestDouble(-DBL_MAX, DBL_MAX, "Mass of the object");
}

int ListIsEmpty(ObjectList* objectList){
  if (objectList->firstElement == NULL)
    return 1;
  else
    return 0;
}

void PushElement(ObjectList* objectList, struct Element* newElement){
  if(ListIsEmpty(objectList)){
    objectList->firstElement  = newElement;
    objectList->lastElement   = newElement;
  }
  else{
    objectList->lastElement->next = newElement;
    objectList->lastElement = newElement;
  }
}

void PopElement(ObjectList* objectList){
  struct Element*p = objectList->firstElement->next;
  free(objectList->firstElement);
  objectList->firstElement = p;
}

void NextCurrentObject(ObjectList* objectList){
  if(objectList->currentElement == NULL){
    objectList->currentElement = objectList->firstElement;
  }
  else{
    objectList->currentElement = objectList->currentElement->next;
  }
}

struct Object * GetCurrentObject(ObjectList* objectList){
  return objectList->currentElement->object;
}

void ResetCurrentObject(ObjectList* objectList){
  //reset the current object pointer to NULL, so that it
  //will output the first element of the Queue when
  // GetCurrentObject() will be called
  objectList->currentElement = NULL;
}

int SeekObject(ObjectList* objectList, struct Object* objectToSeek){
  //seeks an object and positions the currentElement on the
  //containing element, returning 0.
  //if the object cannot be found, returns -1.
  ResetCurrentObject(objectList);
  struct Object* toCompare;
  while((toCompare = GetCurrentObject) != NULL){
    if(strcmp(objectList->currentElement->object->name, objectToSeek->name)==0){
      //obj found, currentElement points to it.
      //printf("OBJECT FOUND!")
      return 0;
    }
    else{
      //increment the currentElement;
      NextCurrentObject(objectList);
    }
  }
  //list does not contain such an element!
  ResetCurrentObject(objectList);
  return -1;



}
