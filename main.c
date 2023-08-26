#include "headers.h"

int main()
{
    // Get the working directory of starting directory
    char starting_directory[4096];
    getcwd(starting_directory, 4096);

    // Keep accepting commands
    while (1)
    {
        check_background_processes();

        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt(starting_directory);
        char input[4096];
        fgets(input, 4096, stdin);

        // Check if the word "pastevents" occurs in the input
        int dontAddToHistory = (strstr(input, "pastevents") != NULL);
        int errorOccured = 0;

        struct CommandList cl = executeCommands(input);
        int numCommands = cl.num_commands;
        struct Command *commands = cl.commands;

        // Print the commandList
        for (int i = 0; i < numCommands; i++) {
            printf("Command in main %d : %s\n", i, commands[i].command_details);
        }


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
            int isBackground = commands[j].printProcId;

            if (isBackground) {
                pid_t pid = fork();
                if (pid < 0) { // Failed fork
                    perror("Fork failed");
                    errorHandler("Fork failed", &errorString);
                } else if (pid == 0) { // Child process block
                    // Make the last entry of command_args as NULL
                    command_args[num_args] = NULL;

                    // Call execvp
                    erroneousFlag = execvp(commandName, command_args);

                    if (erroneousFlag == -1) {
                        printf("\033[31mERROR: \033[0m%s\n", errorString);
                        free(errorString);
                        errorOccured = 1;
                    }
                } else { // Parent process block
                    add_background_process(pid, command_details);
                    printf("[%d]\n", pid); // Print the PID of the background process
                }
            } else {
                // printf("Not BG SOMEHOW WHAT");
                char * userCommandsList[] = {"warp\0", "peek\0", "pastevents\0", "proclore\0", NULL};
                
                // Check if the commandName is one of the the userCommandsList
                int isUserCommand = 0;
                for (int i = 0; userCommandsList[i] != NULL; i++) {
                    if (strcmp(commandName, userCommandsList[i]) == 0) {
                        isUserCommand = 1;
                        break;
                    }
                }

                if (isUserCommand)
                    erroneousFlag = executeCommand(commandName, num_args, command_args, &errorString, starting_directory);
                else {
                    // Make the last entry of command_args as NULL
                    command_args[num_args] = NULL;

                    // Call execvp
                    erroneousFlag = execvp(commandName, command_args);
                    printf("execvp returned %d\n", erroneousFlag);
                    erroneousFlag = (erroneousFlag == -1);
                }

                if (erroneousFlag) {
                    printf("\033[31mERROR: \033[0m%s\n", errorString);
                    free(errorString);
                    errorOccured = 1;
                }
            }
        }

        // if ((!dontAddToHistory) && (!errorOccured)) {
        //     addEventToHistory()
        // }
    }
}
