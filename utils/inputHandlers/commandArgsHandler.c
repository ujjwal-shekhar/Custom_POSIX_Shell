#include "../../headers.h"

int parseCommandArgs(char *command_details, char ** command_args, struct CommandArgs *ca) {
    // struct CommandArgs ca;
    ca->num_args = 0;
    // ca->command_details = command_details;
    // Copy the command_details here
    ca->command_details = (char *) malloc(strlen(command_details) + 1);
    strcpy(ca->command_details, command_details);
    
    ca->command_args = command_args;

    // Initialize the command_args array
    for (int i=0; i<4096; i++) {
        ca->command_args[i] = NULL;
    }

    for (char *token = strtok(command_details, " \v\r\f\t\n"); 
         token != NULL; token = strtok(NULL, " \v\r\f\t\n")) 
    {
        // Manually Trim whitespace from both sides
        int start = 0;
        int end = strlen(token) - 1;
        while (isspace(token[start])) start++;
        while (isspace(token[end])) end--;

        // If the token is only whitespace, then ignore it
        if (start > end) continue;

        // Add the token to the command_args array
        token[end + 1] = '\0';

        ca->command_args[ca->num_args] = token + start;
        ca->num_args++;
    }

    return 0;
}
