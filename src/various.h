#ifndef VARIOUS
#define VARIOUS

#include "monitor.h"

#define KERROR    "\x1B[31m"  //red
#define KNORMAL   "\x1B[0m"   //white
#define KOUTPUT   "\x1B[32m"  //green
#define KDATA     "\x1B[34m"  //blue
#define KEVIDENCE "\x1B[33m"  //yellow

const char * DELIMITER;
const char * WELCOME_TEXT;

//static struct List* celestialBodiesHead;

struct Coordinates{
	double x;
	double y;
	double z;
	};

struct List{
	   struct List* next;
		 struct List* previous;
		 int position;
		 struct CelestialBody* body;
	};

	typedef struct ThreadData{
		//char * line; //type of simulation to use
		char *simulationName; //for saving purposes.
		struct List** object;
		struct CelestialBody* body;
		monitor mon;
		condition computation_section;
		condition saving_section;

		int numberOfThreads;
		int thread_id;
	} ThreadData;

	typedef struct CelestialBody{
		struct CelestialBody* referenceBody;
		char * name;
	    double mass;
		struct Coordinates* coordinates;
		struct Coordinates* speedVector;
		double u;
		/*****NON_ESSENTIAL*****/
		//double radius;		//compared to Earth or Kerbin
		//double geeASL; 		//in Kerbin (or Earth) gravities
		//double rotationPeriod; 	//in seconds
		//orbit
		//double inclination;	//relative to the sun's equator
		//double eccentricity;
		//double perihelion;
		//double aphelion;
		//double orbitalPeriod;
		//Atmospheric values
		//double atmosphereHeight;
	} CelestialBody;

/*****Request Functions Prototypes*****/
double RequestInt(int min, int max, char* message);

double RequestDouble(double min, double max, char * message);

CelestialBody* RequestCelestialBody(char * message, struct List *list);

char* RequestString(char* message, int nullIsAcceptable);

struct Coordinates* RequestCoordinateSet(char* message);

CelestialBody* GetCelestialBody(char* name, struct List *list);

struct List * GetList(char *name, struct List *list);

int GetListSize(struct List *list);

/***** Print Functions Prototypes*****/

void PrintHelloMessage();

void PrintAllBodies(struct List* list);

void PrintDetails(char* name, struct List* list);

/***** List Functions Prototypes*****/

struct List* CreateListElement();



void CreateCelestialBody(struct List* holder, CelestialBody* referenceBody,
	 																 char* name, double mass,
																	 struct Coordinates* positionVector,
																	 struct Coordinates* speedVector);

struct Coordinates* createCoordinateSet(double x,double y,double z);

int IsListEmpty(struct List* list);

int PushElement(struct List **listHead, struct List **newElement);

void UpdatePosition(struct List **list);

void SetToBeginning(struct List **list);

int RemoveList(struct List **list);

#endif
