#include "headers.h"

struct Command {
    char * command_details;
    int printProcId;
} command;

int main()
{
    // Get the working directory of starting directory
    char starting_directory[4096];
    getcwd(starting_directory, 4096);

    // Keep accepting commands
    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt(starting_directory);
        char input[4096];
        fgets(input, 4096, stdin);

        // Maintain a list of commands 
        struct Command commands[4096];
        int numCommands = 0;

        // Taken from `man strtok`
        char *str1, *str2, *token, *subtoken;
        char *saveptr1, *saveptr2;
        int j;

        for (j = 1, str1 = input; ; j++, str1 = NULL) {
            token = strtok_r(str1, ";", &saveptr1);
            if (token == NULL)
                break;

            for (str2 = token; ; str2 = NULL) {
                subtoken = strtok_r(str2, "&", &saveptr2);
                if (subtoken == NULL)
                    break;

                commands[numCommands].command_details = subtoken;
                commands[numCommands].printProcId = 1;
                numCommands++;
            }

            commands[numCommands - 1].printProcId = 0;
        }

        // Execute the commands
        for (j = 0; j < numCommands; j++) {
            // Split each command by whitespace
            char *command_details = commands[j].command_details;
            char *command_args[4096];

            int i = 0;
            for (char *token = strtok(command_details, " \n"); token != NULL; token = strtok(NULL, " \n")) {
                command_args[i] = token;
                i++;
            }

            // The first arg can be used to select the right function
            int erroneousFlag = 0;
            char * errorString;

            // Check if exit command was entered
            if (strncmp(command_args[0], "exit\0", 5) == 0) {
                if (command_args[1] != NULL) {
                    erroneousFlag = 1;
                } else {
                    exit(EXIT_SUCCESS);
                }
            } 

            // Check if warp command was entered
            else if ((strncmp(command_args[0], "warp\0", 5) == 0)) {
                if (command_args[1] == NULL) {
                    command_args[1] = "~";
                    command_args[2] = NULL;
                } else {
                    erroneousFlag = warp(command_args, errorString, starting_directory);
                }
            }
            else 
            {
                printf("\033[31mERROR : '%s' is not a valid command\033[0m\n", command_args[0]);
            }

            if (erroneousFlag) {
                printf("\033[31mERROR: \033[0m%s\n", errorString);
            }
        }
        // End of loop, we clear the commands list
        memset(commands, 0, sizeof(commands));
        j = 0;
    }
}
