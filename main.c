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

        // Check if the word "pastevents" occurs in the input
        int dontAddToHistory = (strstr(input, "pastevents") != NULL);
        int errorOccured = 0;

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

            erroneousFlag = executeCommand(commandName, num_args, command_args, &errorString, starting_directory, dontAddToHistory);

            if (erroneousFlag) {
                printf("\033[31mERROR: \033[0m%s\n", errorString);
                free(errorString);
                errorOccured = 1;
            }
        }

        // if ((!dontAddToHistory) && (!errorOccured)) {
        //     addEventToHistory()
        // }
    }
}
