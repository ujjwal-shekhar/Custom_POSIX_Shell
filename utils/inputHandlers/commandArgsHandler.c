#include "../../headers.h"

struct CommandArgs parseCommandArgs(char *command_details, char ** command_args) {
    struct CommandArgs ca;
    ca.num_args = 0;
    ca.command_details = command_details;
    ca.command_args = command_args;

    // Initialize the command_args array
    for (int i=0; i<4096; i++) {
        ca.command_args[i] = NULL;
    }

    for (char *token = strtok(command_details, " \v\r\f\t\n"); 
         token != NULL; token = strtok(NULL, " \v\r\f\t\n")) 
    {
        ca.command_args[ca.num_args] = token;
        ca.num_args++;
    }

    return ca;
}
