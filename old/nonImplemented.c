
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
