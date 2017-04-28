#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <error.h>
#include <various.h>

static const char * TOKENIZER_STR = ";";

int SaveSimulationData(char * simulationName, struct List** list){
  FILE * fp;
  char str[512];
  fp = fopen(strcat(simulationName,".sdata"), "w");
  if(!fp){
    return ERROR_FILE_DOES_NOT_EXIST;
    //Needed?
  }
  time_t rawtime = time(NULL);
  time ( &rawtime );
  struct tm *timeinfo = localtime ( &rawtime );
  strftime(str, 512,"%c", timeinfo);
  fprintf(fp,"%s\n",str);
  SetToBeginning(list);
  do{
      if(!(*list)->body->referenceBody){
          fprintf(fp,"null%s", TOKENIZER_STR);
      }
      else{
         fprintf(fp, "%s%s", (*list)->body->referenceBody->name, TOKENIZER_STR); //TODO: this may not work since referenceBody may be null
      }
      fprintf(fp, "%s%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s\n",
               (*list)->body->name, TOKENIZER_STR,
               (*list)->body->mass, TOKENIZER_STR,
               (*list)->body->coordinates->x, TOKENIZER_STR,
               (*list)->body->coordinates->y, TOKENIZER_STR,
               (*list)->body->coordinates->z, TOKENIZER_STR,
               (*list)->body->speedVector->x, TOKENIZER_STR,
               (*list)->body->speedVector->y, TOKENIZER_STR,
               (*list)->body->speedVector->z, TOKENIZER_STR
           );
           (*list) = (*list)->next;
  }while((*list)->position != 0);
  fclose(fp);
  printf("Simulation saved");
}

int LoadSimulationData(char * simulationName, struct List** list){
  FILE * fp;
  char str[512];
  fp = fopen(strcat(simulationName,".sdata"), "r");
  if(!fp){
    return ERROR_FILE_DOES_NOT_EXIST;
  }
  if(fgets(str, 512, fp)==NULL){
    return ERROR_CORRUPTED_FILE;
  }
  //load preliminary simulation data

  char * token = strtok(str, TOKENIZER_STR);
  printf("Resuming simulation from %s",token);
  //token = strtok(NULL, TOKENIZER_STR);
  //TODO: whatever other info to load
  //Loads situation of celestial objects onto the simulation;

  while(fgets(str, 512, fp)!=NULL){
 //while(fscanf(fp, 512, fp)!=NULL){
    CelestialBody* referenceBody;
    char * referenceBodyStr = strtok(str, TOKENIZER_STR);
    if(strcmp(referenceBodyStr, "null")){
        referenceBody = NULL;
    }
    else{
        referenceBody = GetCelestialBody(referenceBodyStr, (*list));
    }
    char * name = strtok(NULL, TOKENIZER_STR);
    double mass;
    token = strtok(NULL, TOKENIZER_STR);
    sscanf(token, "%lf", &mass);
    double xp, yp, zp, xs, ys, zs; //suffix p stands for position, s for speed.

    token = strtok(NULL, TOKENIZER_STR);
    sscanf(token, "%lf", &xp);
    token = strtok(NULL, TOKENIZER_STR);
    sscanf(token, "%lf", &yp);
    token = strtok(NULL, TOKENIZER_STR);
    sscanf(token, "%lf", &zp);
    token = strtok(NULL, TOKENIZER_STR);
    sscanf(token, "%lf", &xs);
    token = strtok(NULL, TOKENIZER_STR);
    sscanf(token, "%lf", &ys);
    token = strtok(NULL, TOKENIZER_STR);
    sscanf(token, "%lf", &zs);

    struct Coordinates* positionVector = createCoordinateSet(xp,yp,zp);
    struct Coordinates* speedVector    = createCoordinateSet(xs,ys,zs);

    struct List* element = CreateListElement();
    CreateCelestialBody(element, referenceBody, name, mass, positionVector, speedVector);
    PushElement(list, &element);
    printf("Object loaded: %s\n",(*list)->body->name);
}
  printf("simulation loaded\n");
  return 0;
}
