#ifndef VARIOUS
#define VARIOUS

#define EXIT 1

#define KERROR    "\x1B[31m"  //red
#define KNORMAL   "\x1B[0m"   //white
#define KOUTPUT   "\x1B[32m"  //green
#define KDATA     "\x1B[34m"  //blue
#define KEVIDENCE "\x1B[33m"  //yellow

const char * DELIMITER;
const char * WELCOME_TEXT;

typedef struct ThreadData{
	char * line;
	int  * returnCode;
} ThreadData;

typedef struct CelestialBody{
	char * name;
  double mass;
	double radius;		//compared to Earth or Kerbin
	double geeASL; 		//in Kerbin (or Earth) gravities

	double rotationPeriod; 	//in seconds
	//orbit
	//struct* referenceBody;
	double inclination;	//relative to the sun's equator
	double eccentricity;
	double perihelion;
	double aphelion;
	double orbitalPeriod;
	//Atmospheric values
	double atmosphereHeight;
	double u;
} CelestialBody;

typedef struct Engine{
		double siAsl;
		double trustAsl;
		double siVacuum;
		double trustVacuum;
	} Engine;

typedef struct Stage{
		Engine engine;
		double fuelWeight; // should be divided into oxidizer and oxidant/fuel
		double emptyWeight; //weight of the
		//TODO: much more!
	} Stage;

typedef struct Vehicle{
		char * name;
		Stage *stage;
		//TODO: add all the position values. Orientation will be neglected
		//since it is not the purpose of this software to simulate a flight
		//but merely to design routes.
	} Vehicle;

	//TODO: TEST

	struct Coordinates{
		double x, y , z;
	};

	struct Object{
		//int id;
		char* name;
		CelestialBody* referenceBody;
		struct Coordinates* coordinates;
		struct Coordinates* speedVector;
		double mass;
	};

	struct Element{
		//list object. Every object used in the simulation will be of type Object
		struct Element* next;
		struct Object * object;
	};

	//TODO: END TESTING



typedef struct ObjectList{
	  struct Element* firstElement, *lastElement, *currentElement;
	} ObjectList;


//Function Prototypes

double RequestDouble(double min, double max, char * message);

CelestialBody* RequestCelestialBody(int bodyClass, char * message);

char* RequestString(char* message);



void PrintHelloMessage();

ObjectList* CreateObjectList();

struct Element* CreateElement();

struct Object* CreateObject();

int ListIsEmpty(ObjectList* objectList);

void PushElement(ObjectList* objectList, struct Element* newElement);

void PopElement(ObjectList* objectList);

void NextCurrentObject();

struct Object * GetCurrentObject(ObjectList* objectList);

void ResetCurrentObject(ObjectList* objectList);

int SeekObject(ObjectList* objectList, struct Object* objectToSeek);

//double [] PartitionEllipse(int numberOfPoints);



#endif
