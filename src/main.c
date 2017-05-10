#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "commands.h"
#include "error.h"
#include "various.h"


struct List* celestialBodiesHead = NULL;

int main(){

  PrintHelloMessage();
  char* line;
  int returnCode = 0;
  while(1){
    line = RequestString("waiting for a Command",1);
    returnCode = ExecuteCommand(line);
    ErrorHandling(returnCode);
  }
}
