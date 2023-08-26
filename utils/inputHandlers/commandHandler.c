#include "../../headers.h"


// This was discussed with Anika Roy (2021113008)
struct CommandList executeCommands(char *input) {
    // Maintain a list of commands 
    struct Command commands[4096];
    int numCommands = 0;

    // Save a copy of the raw input
    char * rawInput = (char *) malloc(sizeof(char) * (strlen(input) + 1));
    strcpy(rawInput, input);

    // Split the input by ";&" using strtok
    char *token = strtok(input, ";&");

    while (token != NULL) {
        // printf("%s\n", token);
        commands[numCommands].printProcId = 1;
        commands[numCommands].command_details = token;

        numCommands++;
        token = strtok(NULL, ";&");
    }

    // Check if last command is whitespace and remove it
    int isWhitepace = 1;
    for (int j=0; j<strlen(commands[numCommands - 1].command_details); j++) {
        if (!isspace(commands[numCommands - 1].command_details[j])) {
            isWhitepace = 0;
            break;
        }
    }

    if (isWhitepace) {
        numCommands--;
    }

    // Go over every command and remove any leading or trailing whitespace
    for (int i = 0; i < numCommands; i++) {
        char * command_details = commands[i].command_details;

        // Remove leading whitespace
        while (isspace(command_details[0])) {
            command_details++;
        }

        // Remove trailing whitespace
        int len = strlen(command_details);
        while (isspace(command_details[len - 1])) {
            command_details[len - 1] = '\0';
            len--;
        }

        commands[i].command_details = command_details;
    }

    // Check using the original input string for
    // commands that dont "&" follwing them
    for (int i = 0; i < numCommands; i++) {
        char * command_details = commands[i].command_details;

        // printf("comdeets : %s\n", command_details);

        // Search for the command location
        // in the raw input string
        char * command_location = strstr(rawInput, command_details);

        // print the character after the command_location that is not white space
        char * next_char = command_location + strlen(command_details);
        while (isspace(*next_char)) {
            next_char++;
        }

        // Check if the next character is a "&"
        if (*next_char != '&') {
            commands[i].printProcId = 0;
        }        

    }

    // Taken from `man strtok`
    // char *str1, *str2, *token, *subtoken;
    // char *saveptr1, *saveptr2;
    // int i, j;

    // for (j = 0, str1 = input; ;str1 = NULL) {
    //     token = strtok_r(str1, ";", &saveptr1);
    //     if (token == NULL)
    //         break;

        
    //     for (i = 1, str2 = token; ; i++, str2 = NULL) {
    //         subtoken = strtok_r(str2, "&", &saveptr2);
    //         // printf("%dth subtoken : %s\n", i, subtoken);
    //         if (subtoken == NULL) {
    //             commands[j - 1].printProcId = 0;
    //             break;
    //         }

    //         commands[j].printProcId = 1;
    //         commands[j].command_details = subtoken;

    //         j++;
    //     }

    // }

    // if (isspace(commands[j - 1].command_details[0])) {
    //     j--;
    // }

    // // Print the list of commands
    // // for (int i = 0; i <= numCommands; i++) {
    // //     printf("\nCommand %d : %s | procID : %d\n", i, commands[i].command_details, commands[i].printProcId);
    // // }

    // // numCommands++;
    // numCommands = j;

    // // Print the list of commands
    for (int i = 0; i < numCommands; i++) {
        printf("\nCommand %d : %s | procID : %d\n", i, commands[i].command_details, commands[i].printProcId);
    }

    struct CommandList cl;
    commands[numCommands].command_details = NULL; // To avoid segfaults
    cl.commands = commands;
    cl.num_commands = numCommands;

    return cl;
}
