#include "../../headers.h"

struct CommandList executeCommands(char *input) {
    // Maintain a list of commands 
    struct Command commands[4096];
    int numCommands = 1;

    // Taken from `man strtok`
    char *str1, *str2, *token, *subtoken;
    char *saveptr1, *saveptr2;
    int i, j;

    for (j = 0, str1 = input; ;str1 = NULL) {
        token = strtok_r(str1, ";", &saveptr1);
        if (token == NULL)
            break;

        
        for (i = 1, str2 = token; ; i++, str2 = NULL) {
            subtoken = strtok_r(str2, "&", &saveptr2);
            // printf("%dth subtoken : %s\n", i, subtoken);
            if (subtoken == NULL) {
                commands[j - 1].printProcId = 0;
                break;
            }

            commands[j].printProcId = 1;
            commands[j].command_details = subtoken;

            j++;
        }

    }

    if (isspace(commands[j - 1].command_details[0])) {
        j--;
    }

    // Print the list of commands
    // for (int i = 0; i <= numCommands; i++) {
    //     printf("\nCommand %d : %s | procID : %d\n", i, commands[i].command_details, commands[i].printProcId);
    // }

    // numCommands++;
    numCommands = j;

    struct CommandList cl;
    cl.commands = commands;
    cl.num_commands = numCommands;

    return cl;
}
