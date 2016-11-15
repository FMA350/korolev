#ifndef VARIOUS
#define VARIOUS

#define EXIT 1

const char * DELIMITER;
const char * WELCOME_TEXT;

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


//Function Prototypes

RequestDouble(double min, double max, char * message);

GetCelestialBodyConstant(char * body);

PartitionEllipse(int numberOfPoints);


#endif
