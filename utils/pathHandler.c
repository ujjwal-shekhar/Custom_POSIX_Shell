#include "pathHandler.h"

int pathHandler(char *command_arg, char starting_directory[], char **previous_directory, struct PathInfo *pi) {
    pi->isPath = 1;
    pi->path = NULL; // Initialize the path to NULL

    // Check if the command_arg has "~"
    // Replace "~" with starting directory
    if (command_arg[0] == '~') {
        char *temp = (char *)malloc(strlen(starting_directory) + strlen(command_arg));
        if (temp == NULL) {
            fprintf(stderr, RED_COLOR);
            fprintf(stderr, MEMORY_ALLOC_ERROR);
            fprintf(stderr, RESET_COLOR);
            exit(EXIT_FAILURE);
        }

        strcpy(temp, starting_directory);
        strcat(temp, command_arg + 1);
        strcpy(command_arg, temp);
        free(temp);
    } else if (command_arg[0] == '-') {
        // Replace the "-" symbol by the previous directory
        // If it is empty then don't replace
        if (strlen(*previous_directory) == 0) {
            pi->isPath = 0; // Error
            return 1;
        }

        char *temp = (char *)malloc(strlen(*previous_directory) + strlen(command_arg));
        if (temp == NULL) {
            fprintf(stderr, RED_COLOR);
            fprintf(stderr, MEMORY_ALLOC_ERROR);
            fprintf(stderr, RESET_COLOR);
            pi->isPath = 0; // Error
            exit(EXIT_FAILURE);
        }

        strcpy(temp, *previous_directory);
        strcat(temp, command_arg + 1);
        strcpy(command_arg, temp);
        free(temp);
    } else {
        // Check if the command_arg is completely blank
        // or has only whitespace. If so, then change it 
        // to the current working directory
        
        // Check if argument is only white space
        int isWhitespace = 1;
        for (int j = 0; j < strlen(command_arg); j++) {
            if (!isspace(command_arg[j])) {
                isWhitespace = 0;
                break;
            }
        }

        // Get current working directory
        char cwd[4096]; 
        getcwd(cwd, sizeof(cwd));

        if (isWhitespace) {
            strcpy(command_arg, cwd);
        }
    }

    if (access(command_arg, F_OK) != 0) {
        // Return with error
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, "ERROR: The path is not valid\n");
        fprintf(stderr, RESET_COLOR);
        pi->isPath = 0; // Error
        return 1;
    } else {
        pi->path = (char *)malloc(strlen(command_arg) + 1);
        if (pi->path == NULL) {
            fprintf(stderr, RED_COLOR);
            fprintf(stderr, MEMORY_ALLOC_ERROR);
            fprintf(stderr, RESET_COLOR);
            exit(EXIT_FAILURE);
        }

        strcpy(pi->path, command_arg);
        return 0;
    }
}
