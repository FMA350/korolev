#include <stdio.h>
#include <stdlib.h>

#include <error.h>


int LoadSimulationData(){
  FILE * fp;
  fp = fopen("simulationData.txt", "r");
  if(!fp){
    return ERROR_FILE_DOES_NOT_EXIST;
  }
  //TODO: loaddata into a data structure

}

int LoadPersonalData(){
    FILE * fp;
    fp = fopen("userData.txt", "r");
}

int SaveNewObject(){

}
