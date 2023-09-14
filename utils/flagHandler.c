#include "../headers.h"

#include "../headers.h"

// Modify the flagHandler function to return an integer value
int flagHandler(char *command_arg, char starting_directory[], const char *prefix, struct FlagInfo *fi) {
    // Check if the command has the prefix
    if (command_arg[0] != '-' || command_arg[1] == '\0') {
        fi->isFlag = 0;
        fi->flags = NULL;
        return 0; // No error
    }

    // Remove the prefix
    char *temp = (char *)malloc(sizeof(char) * 4096);
    if (temp == NULL) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, MEMORY_ALLOC_ERROR);
        fprintf(stderr, RESET_COLOR);
        exit(EXIT_FAILURE);
    }
    strcpy(temp, command_arg + strlen(prefix));
    strcpy(command_arg, temp);
    free(temp);

    // Autocompleted by Github Copilot
    // Check if the command_arg is just whitespace
    int notWhiteSpace = 0;
    for (int k = 0; k < strlen(command_arg); k++) {
        if (!isspace(command_arg[k])) {
            notWhiteSpace = 1;
            break;
        }
    }

    if (notWhiteSpace == 0) {
        fi->isFlag = 0;
        fi->flags = NULL;
        return 0; // No error
    }

    fi->isFlag = 1;
    fi->flags = (char *)malloc(sizeof(char) * 4096);
    if (fi->flags == NULL) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, MEMORY_ALLOC_ERROR);
        fprintf(stderr, RESET_COLOR);
        exit(EXIT_FAILURE);
    }
    fi->flags[0] = '\0';
    strcpy(fi->flags, command_arg);

    // Add the prefix again
    temp = (char *)malloc(sizeof(char) * 4096);
    if (temp == NULL) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, MEMORY_ALLOC_ERROR);
        fprintf(stderr, RESET_COLOR);
        exit(EXIT_FAILURE);
    }
    strcpy(temp, prefix);
    strcat(temp, command_arg);
    strcpy(command_arg, temp);
    free(temp);

    return 0; // No error
}
