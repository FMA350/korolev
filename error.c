//header
#include "error.h"

//various
#include <stdio.h>
#include "various.h"


//ERROR Return Codes
//static const int ERROR_WRONG_ARGUMENT_TYPE  = 96;
//static const int ERROR_WRONG_ARGUMENT_VALUE = 97;
//static const int ERROR_TOO_MANY_ARGUMENTS = 98;
//static const int ERROR_COMMAND_NOT_RECOGNIZED = 99;



void ErrorHandling(int errorCode){
//Error handling function. it is executed every time the ExecuteCommand function returns a zero.

  switch(errorCode){

    case EXIT :
              printf("Thanks for using Korolev Space Utility Software \n");
			        //exit program
			        exit(0);
              break;
              //TODO: should I clean the memory? D:

    case ERROR_TOO_MANY_ARGUMENTS:
              printf("ERROR! Too many arguments for such function!\n");
              break;
    case ERROR_COMMAND_NOT_RECOGNIZED: {
              printf("ERROR! Command was not recognized by the\n"
                     "Korolev Space trajectories and Guidance\n"
                     "program!\n");
              break;
            }
  }
}
