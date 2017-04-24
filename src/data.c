#include <stdio.h>
#include <stdlib.h>

#include <error.h>

const char * TOKENIZER_STR = ";";

int SaveSimulationData(char *str){
  FILE * fp;
  fp = fopen(str+".sdata", "r");
  if(!fp){
    return ERROR_FILE_DOES_NOT_EXIST;
  }
  //TODO: loaddata into a data structure

}

// int LoadPersonalData(char *str){
//     FILE * fp;
//     fp = fopen(str+".pdata", "r");
//     if(!fp){
//       return ERROR_FILE_DOES_NOT_EXIST;
//     }
// }

int LoadSimulationData(){
  FILE * fp;
  char str[512];
  fp = fopen(str+".sdata", "r");
  if(!fp){
    return ERROR_FILE_DOES_NOT_EXIST;
  }
  //load preliminary simulation data

  if((fgets(str, 60, fp)==NULL){
    return ERROR_CORRUPTED_FILE;
  }
  char * token = strtok(str, TOKENIZER_STR);
  printf("Resuming simulation from %s",token);
  token = strtok(str, TOKENIZER_STR);
  //TODO: whatever other info to load
  //Loads situation of celestial objects onto the simulation;
  while(fgets(str, 60, fp)!=NULL){
    char * name = strtok(str, TOKENIZER_STR);
    char * referenceBodyStr = strtok(str, TOKENIZER_STR);
    CelestialBody* referenceBody = GetCelestialBody(referenceBodyStr);

    double mass;
    token = strtok(str, TOKENIZER_STR);
    sscanf(token, "%lf", &mass);
    double xp, yp, zp, xs, ys, zs; //suffix p stands for position, s for speed.

    token = strtok(str, TOKENIZER_STR);
    sscanf(token, "%lf", &xp);
    token = strtok(str, TOKENIZER_STR);
    sscanf(token, "%lf", &yp);
    token = strtok(str, TOKENIZER_STR);
    sscanf(token, "%lf", &zp);
    token = strtok(str, TOKENIZER_STR);
    sscanf(token, "%lf", &xs);
    token = strtok(str, TOKENIZER_STR);
    sscanf(token, "%lf", &ys);
    token = strtok(str, TOKENIZER_STR);
    sscanf(token, "%lf", &zs);

    struct Coordinates* positionVector = createCoordinateSet(xp,yp,zp);
    struct Coordinates* speedVector    = createCoordinateSet(xs,ys,zs);

    struct List* list = CreateListElement();
    CreateCelestialBody(list, referenceBody, name, mass, positionVector, speedVector);
    PushElement(celestialBodiesHead, list);
  }

}
