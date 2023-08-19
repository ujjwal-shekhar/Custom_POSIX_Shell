#include "../headers.h"

struct FlagInfo flagHandler(char * command_arg, char ** errorString, char starting_directory[], const char * prefix) {
    // Check if the command has the prefix
    if (command_arg[0] != '-') {
        struct FlagInfo fi;
        fi.isFlag = 0;
        return fi;
    }

    // Remove the prefix
    char * temp = (char *) malloc(sizeof(char) * 4096);
    if (temp == NULL) {
        errorHandler("\033[31mMemory allocation failed\033[0m", errorString);
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
        struct FlagInfo fi;
        fi.isFlag = 0;
        return fi;
    }

    struct FlagInfo fi;
    fi.isFlag = 1;
    fi.flags = command_arg;

    return fi;
}