#include "../headers.h"

struct PathInfo pathHandler(char * command_arg, char ** errorString, char starting_directory[]) {
    struct PathInfo pi;
    pi.isPath = 1;

    // Check if the command_arg has "~"
    // Replace "~" with starting directory
    if (command_arg[0] == '~') {
        char * temp = (char *) malloc(sizeof(char) * 4096);
        strcpy(temp, starting_directory);
        strcat(temp, command_arg + 1);
        strcpy(command_arg, temp);
        free(temp);
    }

    // TODO : Add for OLD PWD/- // IMPPPPPPPPPPPPPPPPPPP

    // Check if the path is accessible
    // Autocompleted by Github Copilot
    // printf("the comarg : %s\n", command_arg);
    if (access(command_arg, F_OK) != 0) {
        // Return with error
        errorHandler("\033[31mThe path is not valid\033[0m", errorString);
        pi.isPath = 0;
        return pi;
    } else {
        pi.isPath = 1;
        pi.path = (char *) malloc(sizeof(char) * (strlen(command_arg) + 1));
        if (pi.path == NULL) {
            // Return with error
            errorHandler("\033[31mMemory allocation failed\033[0m", errorString);
            pi.isPath = 0;
            return pi;
        }

        pi.path[0] = '\0';
        strcpy(pi.path, command_arg);
    }

    return pi;
}