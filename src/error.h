#ifndef ERROR
#define ERROR


//exit defined in Various.h
#define EXIT 1
//List
#define ERROR_LIST_EMPTY             29
//data.c
#define ERROR_CORRUPTED_FILE         40
#define ERROR_FILE_DOES_NOT_EXIST    44
//Bodies
#define ERROR_BODY_NOT_FOUND         54

#define ERROR_COMMAND_NOT_RECOGNIZED 94
#define ERROR_WRONG_ARGUMENT_TYPE    96
#define ERROR_WRONG_ARGUMENT_VALUE   97
#define ERROR_TOO_MANY_ARGUMENTS     98
#define ERROR_TOO_FEW_ARGUMENTS      99


//Functions prototypes
void ErrorHandling(int errorCode);

#endif
