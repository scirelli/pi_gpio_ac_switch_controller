#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wiringPi.h>

#define SUCCESS "{\"success\":true}\n"
#define ERROR_MSG "{\"success\":false, \"msg\":\"%s\"}\n"
#define ERROR_1 "Variable name not found."
#define ERROR_2 "Not a valid value."
#define ERROR_3 "No variable provided."
#define ERROR_4 "WiringPi could not be setup."

#define VARIABLE_NAME "switch_1"
#define ON "on"
#define OFF "off"

#define ERROR_CODE_1 1
#define ERROR_CODE_2 2
#define ERROR_CODE_3 3
#define ERROR_CODE_4 4

#define PIN_17 0

void printHeaders(int);
int sprintSuccess(char *);
int sprintFailure(int errorCode, char *);
int initWirePi();

int main(int argsc, char *argsv[])
{
    char delim[2] = ",",
         subDelim[2] = "=",
         *queryStr = getenv("QUERY_STRING"),
         response[80];
     char *queryVariable,
          *qVarName,
          *qVarValue;

    memset(response, '\0', sizeof(response));

    setlocale(LC_ALL, "");
    if(queryStr == NULL){
        sprintFailure(ERROR_CODE_3, response);
    }else{
        queryVariable = strtok(queryStr, delim);
        qVarName = strtok(queryVariable, subDelim);
        qVarValue = strtok(NULL, subDelim);

        if(strcmp(qVarName, VARIABLE_NAME) != 0){
            sprintFailure(ERROR_CODE_1, response);
        }else{
            if(initWirePi() != 0){
                sprintFailure(ERROR_CODE_4, response);
            }

            if(strcmp(qVarValue, ON) == 0){
                digitalWrite (PIN_17, HIGH);// On
                sprintSuccess(response);
            }else if(strcmp(qVarValue, OFF) == 0){
                digitalWrite (PIN_17, LOW);// On
                sprintSuccess(response);
            }else{
                sprintFailure(ERROR_CODE_2, response);
            }
        }
    }

    printHeaders(strlen(response));
    printf("%s", response);

    return EXIT_SUCCESS;
}

void printHeaders(int bodyLength)
{
    printf("%s\n", "Cache-Control: no-cache");
    printf("%s\n", "pragma: no-cache");
    printf("%s\n", "HTTP/1.0 200 OK");
    printf("%s\n", "Content-Type: application/json");
    printf("Content-Length: %d\n", bodyLength);
    printf("%s\n","");
}

int sprintSuccess(char *response)
{
    return sprintf(response, SUCCESS);
}

int sprintFailure(int errorCode, char *response)
{
    char *msg = "";

    switch(errorCode){
        case ERROR_CODE_1:
            msg = ERROR_1;
            break;
        case ERROR_CODE_2:
            msg = ERROR_2;
            break;
        case ERROR_CODE_3:
            msg = ERROR_3;
            break;
        case ERROR_CODE_4:
            msg = ERROR_4;
            break;
    }

    return sprintf(response, ERROR_MSG, msg);
}

int initWirePi() 
{
    int status = wiringPiSetup();
    pinMode(PIN_17, OUTPUT);

    return status;
}
