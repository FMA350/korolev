#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <error.h>
#include <various.h>

static const char * TOKENIZER_STR = ";";
// static const char * LAST_LINE_TOK = "@";
extern int sim_iteration;
extern int start_iteration;

void PrintHeader(char * simulationName, struct List** list){
    FILE * fp;
    char str[512];
    strcpy(str,simulationName);
    strcat(str,".slog");
    fp = fopen(str, "a");
    time_t rawtime = time(NULL);
    time ( &rawtime );
    struct tm *timeinfo = localtime ( &rawtime );
    strftime(str, 512,"%c", timeinfo);
    fprintf(fp,"%s%s%d%s%d\n",str, TOKENIZER_STR, sim_iteration, TOKENIZER_STR, GetListSize(*list)); //time;initialIteration;objects
    //GetListSize sets the list at the beggining already.
    fclose(fp);
}

void PrintState(char * simulationName, struct List** list){
    int position = (*list)->position;
    SetToBeginning(list);
    FILE * fp;
    char str[512];
    strcpy(str,simulationName);
    strcat(str,".slog");
    fp = fopen(str, "a");
    do{
        if(!(*list)->body->referenceBody){
            fprintf(fp,"null%s", TOKENIZER_STR);
        }
        else{
           fprintf(fp, "%s%s", (*list)->body->referenceBody->name, TOKENIZER_STR);
        }
        fprintf(fp, "%d%s%s%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s%lf%s\n",
                 sim_iteration,       TOKENIZER_STR,
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

    while((*list)->position != position){
        (*list) = (*list)->next;
    }
}

int SaveSimulationData(char * simulationName, struct List** list){
  FILE * fp;
  char str[512];
  fp = fopen(strcat(simulationName,".sdata"), "w");
  time_t rawtime = time(NULL);
  time ( &rawtime );
  struct tm *timeinfo = localtime ( &rawtime );
  strftime(str, 512,"%c", timeinfo);
  fprintf(fp,"%s%s%d%s%d\n",str, TOKENIZER_STR, sim_iteration, TOKENIZER_STR, GetListSize(*list)); //time;iteratioNumber;objects
  //GetListSize sets the list at the beggining already.
  do{
      if(!(*list)->body->referenceBody){
          fprintf(fp,"null%s", TOKENIZER_STR);
      }
      else{
         fprintf(fp, "%s%s", (*list)->body->referenceBody->name, TOKENIZER_STR);
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
  int numberOfObjects = 0;
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
  token = strtok(NULL, TOKENIZER_STR);
  sscanf(token, "%d", &start_iteration);
  token = strtok(NULL, TOKENIZER_STR);
  sscanf(token, "%d", &numberOfObjects);



  //Load last situation of celestial objects onto the simulation;

  while(fgets(str, 512, fp)!=NULL){
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
    printf("Object loaded: %s\n",element->body->name);
}
  printf("simulation loaded\n");
  return 0;
}
