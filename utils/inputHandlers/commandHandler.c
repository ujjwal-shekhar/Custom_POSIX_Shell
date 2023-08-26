#include "../../headers.h"

struct CommandList executeCommands(char *input) {
    // Maintain a list of commands 
    struct Command commands[4096];
    int numCommands = 0;

    // Taken from `man strtok`
    char *str1, *str2, *token, *subtoken;
    char *saveptr1, *saveptr2;
    int i, j;

    for (j = 1, str1 = input; ; j++, str1 = NULL) {
        token = strtok_r(str1, ";", &saveptr1);
        if (token == NULL)
            break;

        
        for (i = 1, str2 = token; ; i++, str2 = NULL) {
            subtoken = strtok_r(str2, "&", &saveptr2);
            // printf("%dth subtoken : %s\n", i, subtoken);
            if (subtoken == NULL) {
                commands[numCommands - 1].printProcId = 0;
                break;
            }

            commands[numCommands].printProcId = 1;
            commands[numCommands].command_details = subtoken;

            numCommands++;
        }

    }

    if (isspace(commands[numCommands - 1].command_details[0])) {
        commands[numCommands - 1].printProcId = 0;
        numCommands--;
    }

    // Print the list of commands
    // for (int i = 0; i <= numCommands; i++) {
    //     printf("\nCommand %d : %s | procID : %d\n", i, commands[i].command_details, commands[i].printProcId);
    // }

    numCommands++;

    struct CommandList cl;
    cl.commands = commands;
    cl.num_commands = numCommands;

    return cl;
}
