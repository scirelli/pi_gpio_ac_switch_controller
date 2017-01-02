#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argsc, char *argsv[])
{
    char delim[2] = ",",
         subDelim[2] = "=",
         *queryStr = getenv("QUERY_STRING");
     char *queryVariable,
          *qVarName,
          *qVarValue;
    
    queryVariable = strtok(queryStr, delim);
    qVarName = strtok(queryVariable, subDelim);
    qVarValue = strtok(NULL, subDelim);

    printf("%s\n", "Content-Type: text/html; charset=utf-8");
    printf("%s\n", "Cache-Control: no-cache");
    printf("%s\n", "pragma: no-cache");
    printf("%s\n","");

    printf("%s\n", "<html>");
        printf("%s\n", "<head>");
            printf("%s\b", "<title>Testing CGI</title>");
        printf("%s\n", "</head>");
        printf("%s\n", "<body>");
            printf("%s\n", "<b>Standard Env vars:</b>");
            printf("<p>QUERY_STRING: '%s'</p>\n", queryStr);
            printf("Value: %s\n", qVarValue);
        printf("%s\n", "</body>");
    printf("%s\n", "</html>");

    return EXIT_SUCCESS;
}
