//header
#include "error.h"

//various
#include <stdio.h>
#include <stdlib.h>
#include "various.h"



void ErrorHandling(int* ec){
//Error handling function. it is executed every time the ExecuteCommand function returns a zero.
//TODO: maiale di quel, non riesco a gestire sto schifo.
int errorCode = (int)ec;
printf("lastdebug: %d\n", errorCode);
  switch(errorCode){
    //TODO: default case
    case EXIT :
              printf(KEVIDENCE"   Thanks for using Korolev Space Utility Software!\n"KNORMAL);
			        //exit program
			        exit(0);
              break;
    case ERROR_TOO_MANY_ARGUMENTS:
              printf(KERROR"  ***ERROR! Too many arguments for such function!\n"KNORMAL);
              break;
    case ERROR_COMMAND_NOT_RECOGNIZED: {
              printf(KERROR"***ERROR! Command was not recognized by the\n"
                           "   Korolev Space trajectories and Guidance\n"
                           "   program!\n"KNORMAL);
              break;
            }
  }
}
