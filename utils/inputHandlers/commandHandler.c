#include "../../headers.h"

struct CommandList executeCommands(char *input) {
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

            // Check if the subtoken is just whitespace
            for (int k = 0; k < strlen(subtoken); k++) {
                if (!isspace(subtoken[k])) {
                    commands[numCommands].command_details = subtoken;
                    commands[numCommands].printProcId = 1;
                    numCommands++;
                    break;
                }
                if (k == strlen(subtoken) - 1) {
                    continue;
                }
            }                
        }

        commands[numCommands - 1].printProcId = 0;
    }

    struct CommandList cl;
    cl.commands = commands;
    cl.num_commands = numCommands;

    return cl;
}
