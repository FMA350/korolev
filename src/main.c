#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "commands.h"
#include "error.h"
#include "various.h"

struct List* celestialBodiesHead = NULL;
char* ReadLine();

int main(){

  PrintHelloMessage();
  char* line;
  int returnCode;
  while(1){
    line = (char*)ReadLine();
    pthread_t td;
    //create a quick struct that contains both the returnCode pointer and the line value.
    ThreadData data; //ThreadData is defined in various.h
      data.returnCode = returnCode;
      data.line = line;
    printf("%s\n",data.line);

    if(pthread_create(&td, NULL, &ExecuteCommand, &data)){
      //Could not create the thread
      printf(KERROR "***ERROR-ERROR-ERROR***\n"
                    "   Internal program Error, thread could not lanch \n" KNORMAL);
      exit(1);
    }

    if(pthread_join(td, NULL)){
      //could not wait the thread to finish
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
