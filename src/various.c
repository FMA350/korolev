#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "various.h"
#include "error.h"
#include "orbitalFunctions.h"

static const double GravConstG = 6.67408e-11;
extern struct List * celestialBodiesHead;

double RequestInt(int min, int max, char* message){
		printf("**Program Request**\n"
					  "Int Value Required: %s \n"
						"Values expected: Min: %d, Max: %d \n", message, min, max);
		char *line = NULL;
		size_t len = 64;
		ssize_t read;
		int value = 0;
		while(1){
			printf("_-_-_ Insert value: ");
			read = getline(&line, &len, stdin);
      int arguments = sscanf(line, "%d", &value);
			if ((value >= min)&&(value <= max)&&(arguments == 1)){
				printf("**Value %d is Acceptable**\n", value);
				return value;
			}
			printf(KERROR"**ERROR-ERROR-ERROR Value was not acceptable\n"KNORMAL);
		}
}

double RequestDouble(double min, double max, char* message){
		printf("**Program Request**\n"
					  "Double Value Required: %s \n"
						"Values expected: Min: %g, Max: %g \n", message, min, max);
		char *line = NULL;
		size_t len = 64;
		ssize_t read;
		double value =0;
		while(1){
			printf("_-_-_ Insert value: ");
			read = getline(&line, &len, stdin);
      int arguments = sscanf(line, "%lg", &value);
			if ((value >= min)&&(value <= max)&&(arguments==1)){
				printf("**Value %G is Acceptable**\n", value);
				return value;
			}
			printf(KERROR"**ERROR-ERROR-ERROR Value was not acceptable\n"KNORMAL);
		}
}

char* RequestString(char* message, int nullIsAcceptable){
  printf("**Program Request**\n"
					"String Required: %s \n", message);
  char *line = NULL;
  size_t len = 64;
  ssize_t read;
  double value;
  while(1){
  printf("_-_-_ Insert value: ");
  read = getline(&line, &len, stdin);
	if(strlen(line)!=1){
		line = strtok(line, "\n");
		if((nullIsAcceptable)&&strcmp(line,"null") == 0){
			return NULL;
		}

		printf(KOUTPUT"getline contains:"KDATA" %s\n"KNORMAL,line);
		return line;
	}
	printf(KERROR"**ERROR-ERROR-ERROR Value was not acceptable\n"KNORMAL);
	}
}

CelestialBody* RequestCelestialBody(char* message, struct List *list){
	printf("**Program Request**\n"
					"Celestial Body Required: %s \n", message);
	  char *line = RequestString("Insert the name of the celestial body you are looking for",1);
		CelestialBody* celestialBody;
		celestialBody = GetCelestialBody(line, list);
    return celestialBody;
}

struct Coordinates* RequestCoordinateSet(char* message){
  printf("**Program Request**\n"
         "BuildCoordinatesSet function\n"
         "%s\n", message);
         double x = RequestDouble(-DBL_MAX, DBL_MAX, "x");
         double y = RequestDouble(-DBL_MAX, DBL_MAX, "y");
         double z = RequestDouble(-DBL_MAX, DBL_MAX, "z");
				 struct Coordinates *coordinates = createCoordinateSet(x,y,z);
         return coordinates;
}


CelestialBody* GetCelestialBody(char* name,struct List* list){
	if(!list) return NULL;
	if(!name) return NULL;
	SetToBeginning(&list);
	int numberOfBodies = list->previous->position;
	for (int i = 0; i <= numberOfBodies; i++){
		if(strcmp(name, list->body->name) == 0){
			printf(KOUTPUT"Body %s found!\n"KNORMAL,name);
			return list->body;
		}
		else{
			list = list->next;
		}
	}
	printf(KERROR"Could not find specified body %s, returning null\n"KNORMAL, name);
	return NULL;
}

int GetListSize(struct List *list){
	SetToBeginning(&list);
	if(list)
		return (list->previous->position)+1;
	else
		return 0;
}

struct List * GetList(char *name, struct List *list){
	SetToBeginning(&list);
	if(!list) return NULL;
	int numberOfBodies = list->previous->position;
	for (int i = 0; i <= numberOfBodies; i++){
		if(strcmp(name, list->body->name) == 0){
			printf(KOUTPUT"Body %s found!\n"KNORMAL,name);
			return list;
		}
		else{
			list = list->next;
		}
	}
	printf(KERROR"Could not find specified list with body %s, returning null"KNORMAL, name);
	return NULL;
}


//PRINT METHODS
void PrintHelloMessage(){

  printf(KOUTPUT"\n\n\n                                               Korolev Space Guidance System 0.0.1\n\n\n"KEVIDENCE
  "                  :\n"
  "                  :\n"
  "                  :\n"
  "                  :\n"
  "   .              :\n"
  "    '.            :           .'\n"
  "      '.          :         .'\n"
  "        '.   .-''''''-.   .'      "KNORMAL"     "KEVIDENCE"     "KDATA"     "KERROR"    "KEVIDENCE"           .'':\n"
  "          '.'          '.'        "KNORMAL"     "KEVIDENCE"     "KDATA"     "KERROR"    "KEVIDENCE"     .-''''-.'         .---.      "KDATA"    .----.     ""   .-'''-.\n"KEVIDENCE
  "           :            :         "KNORMAL"     "KEVIDENCE"  _  "KDATA"  _  "KERROR"    "KEVIDENCE"   .'     .' '.    ...'     '...  "KDATA"  .'      '.   "" .'       '.\n"KEVIDENCE
  "   .........            ......... "KNORMAL"   o "KEVIDENCE" (_) "KDATA" (_) "KERROR" () "KEVIDENCE"   :    .'    :   '..:.......:..' "KDATA"  :        :   "" :         : "KEVIDENCE"  o\n"
  "           :            :         "KNORMAL"     "KEVIDENCE"     "KDATA"     "KERROR"    "KEVIDENCE"   :  .'      :       '.....'     "KDATA"  '.      .'   "" '.       .'\n"KEVIDENCE
  "            :          :          "KNORMAL"     "KEVIDENCE"     "KDATA"     "KERROR"    "KEVIDENCE" .'.'.      .'                    "KDATA"    `''''`     ""   `'''''`\n"KEVIDENCE
  "             '........'           "KNORMAL"     "KEVIDENCE"     "KDATA"     "KERROR"    "KEVIDENCE" ''   ``````\n"
  "            .'    :   '.\n"
  "          .'      :     '.\n"
  "        .'        :       '.\n"
  "      .'          :         '.\n"
  "                  :\n"
  "                  :\n"
  "  Dana97          :\n"
  "  fma350          :\n"KNORMAL);
}

void PrintAllBodies(struct List* list){
	printf("List of availeable bodies \n\n");
	if(!list){
		printf("No bodies are present!\n");
		return;
	}
	SetToBeginning(&list);
	int numberOfBodies = list->previous->position;
	for(int i = 0; i <= numberOfBodies; i++){
		if(list->body->referenceBody!=NULL){
		printf(KDATA"Body number %i:"KEVIDENCE" %s, referenceBody: %s mass: %G\n"KNORMAL, i, list->body->name, list->body->referenceBody->name, list->body->mass);
		}
		else{
			printf(KDATA"Body number %i:"KEVIDENCE" %s, referenceBody: N.A. mass: %G\n"KNORMAL, i, list->body->name, list->body->mass);
		}
		printf("list->previous = %s, list->next = %s\n\n",list->previous->body->name, list->next->body->name);

		list = list->next;
	}
}

void PrintDetails(char* name, struct List* list){
	CelestialBody *body = GetCelestialBody(name, list);
	if(!body){
		printf(KERROR"body could not be found!\n"KNORMAL);
		return;
	}
	printf(KOUTPUT"Mass of the object:"KDATA" %G kg\n",body->mass );

	printf(KOUTPUT"Position vector (m):\n"KDATA);
	printf("x = %G\n",body->coordinates->x);
	printf("y = %G\n",body->coordinates->y);
	printf("z = %G\n",body->coordinates->z);
	printf(KOUTPUT"Velocity vector(m/s):\n"KDATA);
	printf("x = %G\n",body->speedVector->x);
	printf("y = %G\n",body->speedVector->y);
	printf("z = %G\n",body->speedVector->z);
	printf(KOUTPUT"Constant U:"KDATA"%G\n"KNORMAL, body->u );

}

//STRUCT CREATION METHODS
struct List* CreateListElement(){
  //return a pointer to an alloccated Object List space.
  struct List * element = (struct List*)malloc(sizeof(struct List));
  element->next = element;
  element->previous = element;
  return element;
}

void CreateCelestialBody(struct List* holder, CelestialBody* referenceBody,
	 																 char* name, double mass,
																	 struct Coordinates* positionVector,
																	 struct Coordinates* speedVector){
  //Links the newly created CelestialBody to the holder List element.

  struct CelestialBody* celestialBody = (struct CelestialBody*)malloc(sizeof(struct CelestialBody));
  celestialBody->referenceBody = referenceBody;
  celestialBody->name = (char*)malloc(sizeof(char*)*512);
  strcpy(celestialBody->name, name);
  celestialBody->mass          = mass;
  celestialBody->coordinates   = positionVector;
  celestialBody->speedVector   = speedVector;
  celestialBody->u = (celestialBody->mass)*GravConstG;
  holder->body = celestialBody;
}

struct Coordinates* createCoordinateSet(double x,double y,double z){
 	struct Coordinates *coordinates = (struct Coordinates*)malloc(sizeof(struct Coordinates));
	coordinates->x = x;
	coordinates->y = y;
	coordinates->z = z;
	return coordinates;
}

//LIST CONTROL METHODS

//FIXME
int IsListEmpty(struct List* list){
	if (!list) return 1;
	//TODO: TOTEST
  if ((list->next == list )&&(list->previous == list))
    return 1;
  else
    return 0;
}

int PushElement(struct List **listHead, struct List **newElement){
  if(!*listHead){
		printf("PushElement: celestialBodiesHead is empty\n");
    	(*newElement)->position = 0;
		(*newElement)->next = (*newElement); //TODO FIXME
		(*newElement)->previous = (*newElement);
		(*listHead) = (*newElement);
    return 0;
  }
  else{
	printf("PushElement: celestialBodiesHead has elements\n");
		SetToBeginning(listHead);
    	(*newElement)->position = (*listHead)->previous->position+1;
		(*newElement)->next = (*listHead);
    	(*newElement)->previous = (*listHead)->previous;

    (*listHead)->previous->next = *newElement;
		(*listHead)->previous 	= *newElement;
		return 0;
  }
}

void UpdatePosition(struct List **list){
	//if list is null
	if(!*list) return;
	//if list has only 1 object
	if((*list)->next == (*list)){
		(*list)->position = 0;
		return;
	}
	//if root was removed, we are now pointing to the last element.
	//next one will be the new root.
	if((*list)->next->position <= (*list)->position){
		(*list)->next->position = 0;
		(*list) = (*list)->next; //go to root element
	}
    while((*list)->next->position != 0){
    	if((*list)->position+1 != ((*list)->next->position)){
			printf("update position of %s\n",(*list)->next->body->name);
      		(*list)->next->position = (*list)->position+1;
    	}
    (*list) = (*list)->next;
  }
}
//FIXME
void SetToBeginning(struct List **list){
	if(!*list) return;
	while((*list)->position != 0){
		printf("looking for the beginning...\n");
		//PROBLEM IS HERE
    (*list) = (*list)->next;
	}
	return;
}

//FIXME
int RemoveList(struct List** list){
  if(!list){
    printf("Nothing to remove");
    return 0;
  }
  else if((*list)->next == (*list)){
		printf("Single element in the list\n");
    free((*list)->body);
		(*list)->next = NULL;
		(*list)->previous = NULL;
    free(*list);
		//free(list);
		(*list) = NULL;
    return 0;
  }
  else{
		//FIXME
		printf("multiple elements in the list\n");
    (*list)->previous->next = (*list)->next;
    (*list)->next->previous = (*list)->previous;
    struct List *temp = (*list)->previous;
    free((*list)->body);
    free((*list));
	(*list) = temp;
    UpdatePosition(list);
    return 0;
  }
}
