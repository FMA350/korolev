#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "commands.h"
#include "error.h"
#include "various.h"
#include "simulation.h"

//GLOBAL VARIABLES
struct List* celestialBodiesHead = NULL;
int sim_iteration = 0;
int start_iteration = 0;
int simulationMethodFlag = 0;
void (*SimulationMethod)(struct List*, struct Coordinates*, struct Coordinates*,struct Coordinates*);

int main(){
    PrintHelloMessage();
    SimulationMethod = &RungeKutta;

    char* line;
    int returnCode = 0;
    while(1){
        line = RequestString("waiting for a Command",1);
        returnCode = ExecuteCommand(line);
        ErrorHandling(returnCode);
    }
}
