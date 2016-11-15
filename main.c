#include <stdio.h>


#include "commands.h"
#include "error.h"


char* ReadLine();
int main(){

  //printHelloMessage();
  char* line;
  int returnCode;
  while(1){
    //get the order
    line = (char*)ReadLine();
    returnCode = ExecuteCommand(line);
    ErrorHandling(returnCode);
  }
}


  char* ReadLine(){
    char *line = NULL;
    size_t len = 64;
    ssize_t read;
    read = getline(&line, &len, stdin);
    //TODO: check why the EOF character sends the program to hell
    printf("%zu\n", read);
    return line;
  }
