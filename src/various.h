#ifndef VARIOUS
#define VARIOUS

#define EXIT 1

#define KERROR    "\x1B[31m"  //red
#define KNORMAL   "\x1B[0m"   //white
#define KOUTPUT   "\x1B[32m"  //green
#define KDATA     "\x1B[34m"  //blue
#define KEVIDENCE "\x1B[33m"  //yellow

// typedef struct Engine{
// 		double siAsl;
// 		double trustAsl;
// 		double siVacuum;
// 		double trustVacuum;
// 	} Engine;
// typedef struct Stage{
// 		Engine engine;
// 		double fuelWeight; // should be divided into oxidizer and oxidant/fuel
// 		double emptyWeight; //weight of the
// 		//TODO: much more!
// 	} Stage;
// typedef struct Vehicle{
// 		char * name;
// 		Stage *stage;
// 		//TODO: add all the position values. Orientation will be neglected
// 		//since it is not the purpose of this software to simulate a flight
// 		//but merely to design routes.
// 	} Vehicle;
//
// 	//TODO: TEST
//
// 	struct Coordinates{
// 		double x, y , z;
// 	};
//
// 	struct Object{
// 		//int id;
// 		char* name;
// 		CelestialBody* referenceBody;
// 		struct Coordinates* coordinates;
// 		struct Coordinates* speedVector;
// 		double mass;
// 	};
//
// 	struct Element{
// 		//list object. Every object used in the simulation will be of type Object
// 		struct Element* next;
// 		struct Object * object;
// 	};
//
// 	//TODO: END TESTING

//static const double GravConstG;

const char * DELIMITER;
const char * WELCOME_TEXT;

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
		char * line;
		int  * returnCode;
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

/*****Function Prototypes*****/

double RequestDouble(double min, double max, char * message);

CelestialBody* RequestCelestialBody(int celestialBodyClass, char * message);

char* RequestString(char* message);

void PrintHelloMessage();

void PrintAllBodies(struct List* list);

struct List* CreateListElement();

// struct Element* CreateElement();
//
CelestialBody* CreateCelestialBody(struct List* holder);

int IsListEmpty(struct List* list);

void PushElement(struct List* listHead, struct List* newElement);

void UpdatePosition(struct List* list);

struct List* SetToBeginning(struct List* list);

struct List* RemoveList(struct List* listHead);

#endif
