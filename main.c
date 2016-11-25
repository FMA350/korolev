#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "commands.h"
#include "error.h"
#include "various.h"


char* ReadLine();
int main(){

  //printHelloMessage();
  char* line;
  int returnCode;
  while(1){
    //get the order
    line = (char*)ReadLine();
    pthread_t td;
    //create a quick struct that contains both the returnCode pointer and the line value.
    ThreadData data;
      data.returnCode = (int)&returnCode;
      data.line = line;
    if(pthread_create(&td, NULL, &ExecuteCommand, &data)){
      printf(KERROR "***ERROR-ERROR-ERROR***\n"
                    "   Internal program Error, thread could not lanch \n" KNORMAL);
            exit(1);
    }
    if(pthread_join(td, NULL)){
      printf(KERROR "***ERROR-ERROR-ERROR***\n"
                    "   Internal program Error, thread could not join \n" KNORMAL);
            exit(1);
    }
    else{
        ErrorHandling(data.returnCode);
    }

  }
}


  char* ReadLine(){
    char *line = NULL;
    size_t len = 64;
    ssize_t read;
    read = getline(&line, &len, stdin);
    //TODO: check why the EOF character sends the program to hell
    return line;
  }
