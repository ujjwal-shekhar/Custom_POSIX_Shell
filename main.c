#include "headers.h"

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

        // Continue if input is only whitespace characters

        struct CommandList cl = executeCommands(input);
        int numCommands = cl.num_commands;
        struct Command *commands = cl.commands;

        int j;

        // Execute the commands
        for (j = 0; j < numCommands; j++) {
            // Split each command by whitespace
            char *command_details = commands[j].command_details;
            char *command_args[4096];

            // Autocompleted by Github copilot
            struct CommandArgs ca = parseCommandArgs(command_details, command_args);

            // To handle and display errors
            int erroneousFlag = 0;
            char * errorString;

            // Print the command details
            // printf("the comm deets now: %s\n", ca.command_details);

            int num_args = ca.num_args;
            char* commandName = ca.command_args[0];


            // Check if exit command was entered
            if (strcmp(commandName, "exit") == 0) {
                if (num_args > 1) {
                    erroneousFlag = 1;
                } else {
                    exit(EXIT_SUCCESS);
                }
            }


            // Check if warp command was entered
            else if (strncmp(commandName, "warp\0", 5) == 0) {
                int notWhiteSpace = 0;
                // Check if the entire command_arg[1] is a whitespace
                for (int k = 0; k < strlen(command_args[1]); k++) {
                    if (!isspace(command_args[1][k])) {
                        notWhiteSpace = 1;
                        break;
                    }
                }

                if (command_args[1] == NULL) {
                    command_args[1] = "~";
                    command_args[2] = NULL;
                } else if (notWhiteSpace == 0) {
                    command_args[1] = "~";
                    command_args[2] = NULL;
                } else if (command_args[1] == "-") {
                    // Add code for pwd later
                }
                erroneousFlag = warp(command_args, errorString, starting_directory);
            }

            // Check if `peek` command was entered
            else if (strncmp(commandName, "peek\0", 5) == 0) {
                // Check if there were no more than 4 arguments
                if (num_args > 4) {
                    erroneousFlag = 1;
                } else {
                    erroneousFlag = peek(command_args, &errorString, starting_directory);
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
    }
}
