#include "errorHandler.h"

void errorHandler(const char * newError, char ** errorString) {
    *errorString = (char *) malloc(strlen(newError) + 1);
    if (*errorString != NULL) {
        strcpy(*errorString, newError);
    }
}