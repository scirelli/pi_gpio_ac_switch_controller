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

#define VARIABLE_NAME "switch_1"
#define ON "on"
#define OFF "off"

#define ERROR_CODE_1 1
#define ERROR_CODE_2 2
#define ERROR_CODE_3 3

#define PIN_17 0

void printHeaders(void);
void printSuccess(void);
void printFailure(int errorCode);
void initWirePi();

int main(int argsc, char *argsv[])
{
    char delim[2] = ",",
         subDelim[2] = "=",
         *queryStr = getenv("QUERY_STRING");
     char *queryVariable,
          *qVarName,
          *qVarValue;

    setlocale(LC_ALL, "");
    printHeaders();
    if(queryStr == NULL){
        printFailure(ERROR_CODE_3);
    }else{
        queryVariable = strtok(queryStr, delim);
        qVarName = strtok(queryVariable, subDelim);
        qVarValue = strtok(NULL, subDelim);

        if(strcmp(qVarName, VARIABLE_NAME) != 0){
            printFailure(ERROR_CODE_1);
        }else{
            initWirePi();

            if(strcmp(qVarValue, ON) == 0){
                digitalWrite (PIN_17, HIGH);// On
                printSuccess();
            }else if(strcmp(qVarValue, OFF) == 0){
                digitalWrite (PIN_17, LOW);// On
                printSuccess();
            }else{
                printFailure(ERROR_CODE_2);
            }
        }
    }

    return EXIT_SUCCESS;
}

void printHeaders(void)
{
    printf("%s\n", "Content-Type: application/json");
    printf("%s\n", "Cache-Control: no-cache");
    printf("%s\n", "pragma: no-cache");
    printf("%s\n","");
}

void printSuccess(void)
{
    printf("%s\n", SUCCESS);
}

void printFailure(int errorCode)
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
    }

    printf(ERROR_MSG, msg);
}

void initWirePi() 
{
    wiringPiSetup();
    pinMode(PIN_17, OUTPUT);
}
