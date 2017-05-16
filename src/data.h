#ifndef DATA
#define DATA

void PrintHeader(char * simulationName, struct List** list);

void PrintState(char * simulationName, struct List** list);

//// void Afterworld(char * simulationName);

int LoadSimulationData(char * simulationName, struct List** list);

int SaveSimulationData(char * simulationName, struct List** list);

//int LoadPersonalData(char *str);

#endif
