#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "various.h"

static struct List* celestialBodiesHead;

static const double GravConstG = 6.67408e-11;

// static CelestialBody celestialBody[4] = {
//  {
// 	//name             =
//   "Sun",
// 	//mass						 =
//   1000000,
// 	//radius           =
//   6371E3, //mean radius in meters
// 	//geeASL           =
//   9.807,		//m/s^2
// 	//rotationPeriod   =
//   86164.090, //seconds, sideral day
// 	////referenceBody    = celestialBody[0][0];
// 	//inclination      =
//   7.155, //degrees
// 	//eccentricity     =
//   	0.0167086,
// 	//perihelion       =
//   147095000000, //meters
// 	//aphelion 		     =
//   152100000000, //meters
// 	//orbitalPeriod    =
//   365.256363004, //Days
// 	//atmosphereHeight =
//   10000,			//Kerman line
// 	//u 							 =
//   1.32712440018E20
//  },
//  {
//   //name             =
//   "Mercury",
// 	//mass						 =
//   5.97237E24, //5.97237×10^24
// 	//radius           =
//   6371000, //mean radius in meters
// 	//geeASL           =
//   9.807,		//m/s^2
// 	//rotationPeriod   =
//   86164.090, //seconds, sideral day
// 	////referenceBody  = celestialBody[0][0],
// 	//inclination      =
//   7.155, //degrees
// 	//eccentricity     =
//   	0.0167086,
// 	//perihelion       =
//   147095000000, //meters
// 	//aphelion 		     =
//   152100000000, //meters
// 	//orbitalPeriod    =
//   365.256363004, //Days
// 	//atmosphereHeight =
//   100000,			//Kerman line
// 	//u 							 =
//   3.986004418E14
//  },
//  {
// 	//name             =
//   "Venus",
// 	//mass						 =
//   5.97237E24, //5.97237×10^24
// 	//radius           =
//   6371000, //mean radius in meters
// 	//geeASL           =
//   9.807,		//m/s^2
// 	//rotationPeriod   =
//   86164.090, //seconds, sideral day
// 	//referenceBody    = celestialBody[0][0];
// 	//inclination      =
//   7.155, //degrees
// 	//eccentricity     =
//   	0.0167086,
// 	//perihelion       =
//   147095000000, //meters
// 	//aphelion 		     =
//   152100000000, //meters
// 	//orbitalPeriod    =
//   365.256363004, //Days
// 	//atmosphereHeight =
//   100000,			//Kerman line
// 	//u 							 =
//   3.986004418E14
//  },
//  {
// 	//name             =
//    "Earth",
// 	//mass						 =
//    5.97237E24, //5.97237×10^24
// 	//radius           =
//    6371000, //mean radius in meters
// 	//geeASL           =
//    9.807,		//m/s^2
// 	//rotationPeriod   =
//    86164.090, //seconds, sideral day
// 	//referenceBody    = celestialBody[0][0];
// 	//inclination      =
//    7.155, //degrees
// 	//eccentricity     =
//    	0.0167086,
// 	//perihelion       =
//    147095000000,//meters
// 	//aphelion 		     =
//    152100000000, //meters
// 	//orbitalPeriod    =
//    365.256363004, //Days
// 	//atmosphereHeight =
//    100000,			//Kerman line
// 	//u 							 =
//    3.986004418E14
//  }
// };

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

void RequestAddCelestialBody(){
	CelestialBody *referenceBody = RequestCelestialBody  (0, "Reference body for the CelestialBody");
	char *name  = RequestString("CelestialBody Name");
	double mass = RequestDouble(-DBL_MAX, DBL_MAX, "Mass of the CelestialBody");
	Coordinates *positionVector = RequestCoordinateSet("Position of the CelestialBody(meters)");
	Coordinates *speedVector    = RequestCoordinateSet("Velocity vector for the CelestialBody (m/s)");
	struct List* list = CreateListElement();
	CreateCelestialBody(list, referenceBody, name, mass, positionVector, speedVector);
	PushElement(celestialBodiesHead, list);
}

CelestialBody* RequestCelestialBody(char* message){
	printf("**Program Request**\n"
					"Celestial Body Required: %s \n", message);
		PrintAllBodies(celestialBodiesHead);
	  char *line = RequestString("CelestialBody name, type enter for no reference body.");
		CelestialBody* celestialBody;
		celestialBody=GetCelestialBody(line)
    return celestialBody;
}

struct Coordinates* RequestCoordinateSet(char* message){
  printf("**Program Request**\n"
         "BuildCoordinatesSet function\n"
         "%s\n", message);
         double x = RequestDouble(-DBL_MAX, DBL_MAX, "x");
         double y = RequestDouble(-DBL_MAX, DBL_MAX, "y");
         double z = RequestDouble(-DBL_MAX, DBL_MAX, "z");
				 struct Coordinates *coordinates = createCoordinateSet(x,y,z);
         return coordinates;
}


CelestialBody* GetCelestialBody(char* name){
	celestialBodiesHead = SetToBeginning(celestialBodiesHead);
	int numberOfBodies = celestialBodiesHead->previous->position;
	for (int i = 0; i <= numberOfBodies; i++){
		if(strcmp(name, celestialBodiesHead->body->name) == 0){
			return celestialBodiesHead->body;
		}
		else{
			celestialBodiesHead = celestialBodiesHead->next;
		}
	}
	return NULL;
}


//PRINT METHODS
void PrintHelloMessage(){

  printf(KOUTPUT"\n\n\n                                               Korolev Space Guidance System 0.0.1\n\n\n"KEVIDENCE
  "                  :\n"
  "                  :\n"
  "                  :\n"
  "                  :\n"
  "   .              :\n"
  "    '.            :           .'\n"
  "      '.          :         .'\n"
  "        '.   .-''''''-.   .'      "KNORMAL"     "KEVIDENCE"     "KDATA"     "KERROR"    "KEVIDENCE"           .'':\n"
  "          '.'          '.'        "KNORMAL"     "KEVIDENCE"     "KDATA"     "KERROR"    "KEVIDENCE"     .-''''-.'         .---.      "KDATA"    .----.     ""   .-'''-.\n"KEVIDENCE
  "           :            :         "KNORMAL"     "KEVIDENCE"  _  "KDATA"  _  "KERROR"    "KEVIDENCE"   .'     .' '.    ...'     '...  "KDATA"  .'      '.   "" .'       '.\n"KEVIDENCE
  "   .........            ......... "KNORMAL"   o "KEVIDENCE" (_) "KDATA" (_) "KERROR" () "KEVIDENCE"   :    .'    :   '..:.......:..' "KDATA"  :        :   "" :         : "KEVIDENCE"  o\n"
  "           :            :         "KNORMAL"     "KEVIDENCE"     "KDATA"     "KERROR"    "KEVIDENCE"   :  .'      :       '.....'     "KDATA"  '.      .'   "" '.       .'\n"KEVIDENCE
  "            :          :          "KNORMAL"     "KEVIDENCE"     "KDATA"     "KERROR"    "KEVIDENCE" .'.'.      .'                    "KDATA"    `''''`     ""   `'''''`\n"KEVIDENCE
  "             '........'           "KNORMAL"     "KEVIDENCE"     "KDATA"     "KERROR"    "KEVIDENCE" ''   ``````\n"
  "            .'    :   '.\n"
  "          .'      :     '.\n"
  "        .'        :       '.\n"
  "      .'          :         '.\n"
  "                  :\n"
  "                  :\n"
  "  Dana97          :\n"
  "  fma350          :\n"KNORMAL);
}

void PrintAllBodies(struct List* list){
	printf("List of availeable bodies \n\n");
	celestialBodiesHead = SetToBeginning(celestialBodiesHead);
	int numberOfBodies = celestialBodiesHead->previous->position;
	for(int i = 0; i <= numberOfBodies; i++){
		printf(KDATA"Body number %i:"KEVIDENCE" %s\n"KNORMAL, i, list->body->name);
	}
}

//STRUCT CREATION METHODS
struct List* CreateListElement(){
  //return a pointer to an alloccated Object List space.
  struct List * element = (struct List*)malloc(sizeof(struct List));
  return element;
}

void CreateCelestialBody(struct List* holder, CelestialBody* referenceBody,
	 																 char* name, double mass,
																	 struct Coordinates* positionVector,
																	 struct Coordinates* speedVector){
  //Links the newly created CelestialBody to the holder List element.

  struct CelestialBody* celestialBody = (struct CelestialBody*)malloc(sizeof(struct CelestialBody));

  celestialBody->referenceBody = referenceBody;
  celestialBody->name          = name;
  celestialBody->mass          = mass;
  celestialBody->coordinates   = positionVector;
  celestialBody->speedVector   = speedVector;
  celestialBody->u = (celestialBody->mass)*GravConstG;
	holder->body = celestialBody;
}

struct Coordinates* createCoordinateSet(double x, y, z){
 	struct Coordinates *coordinates = (struct Coordinates*)malloc(sizeof(struct Coordinates));
	coordinates->x = x;
	coordinates->y = y;
	coordinates->z = z;
	return coordinates;
}

//LIST CONTROL METHODS

//FIXME
int IsListEmpty(struct List* list){
	if (!list) return 1;
	//TODO: TOTEST
  if ((list->next == list )&&(list->previous == list))
    return 1;
  else
    return 0;
}

void PushElement(struct List* listHead, struct List* newElement){
  if(!listHead){
    newElement->position = 0;
		newElement->next = newElement;
		newElement->previous = newElement;
    listHead  = newElement;
  }
  else{
    newElement->position = listHead->previous->position+1;

    newElement->next = listHead;
    newElement->previous = listHead->previous;
    listHead->previous->next = newElement;
    listHead->previous = newElement;
  }
}

void UpdatePosition(struct List* list){
  while(list->next->position != 0){
    if(list->position != (list->next->position-1)){
      list->next->position = list->position+1;
    }
    list = list->next;
  }
}

struct List* SetToBeginning(struct List* list){
	while(list->position != 0){
		list = list->next;
	}
	return list;
}

struct List* RemoveList(struct List* listHead){
  if(!listHead){
    printf("Nothing to remove");
    return NULL;
  }
  else if((listHead->next==NULL)||(listHead->previous==NULL)){
    free(listHead->body);
    free(listHead);
    return NULL;
  }
  else{
    //TODO FIXME
    listHead->previous->next = listHead->next;
    listHead->next->previous = listHead->previous;
    struct List * p = listHead->previous;
    free(listHead->body);
    free(listHead);
    UpdatePosition(p);
    return p;
  }
}
