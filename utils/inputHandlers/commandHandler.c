#include "commandHandler.h"

// This was discussed with Anika Roy (2021113008)
int tokenizeInput(char *input, struct CommandList *cl) {
    // Validate input and initialize numCommands
    if (input == NULL || cl == NULL) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, "Invalid input to tokenizeInput\n");
        fprintf(stderr, RESET_COLOR);
        return 1; // Error: Invalid input
    }

    // Maintain a list of commands 
    struct Command commands[4096];
    int numCommands = 0;

    // Save a copy of the raw input
    char *rawInput = (char *)malloc(sizeof(char) * (strlen(input) + 1));
    if (rawInput == NULL) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, MEMORY_ALLOC_ERROR);
        fprintf(stderr, RESET_COLOR);
        exit(EXIT_FAILURE);
    }
    strcpy(rawInput, input);

    // Split the input by ";&" using strtok
    char *token = strtok(input, ";&");

    while (token != NULL) {
        commands[numCommands].printProcId = 1;
        commands[numCommands].command_details = token;

        numCommands++;
        token = strtok(NULL, ";&");
    }

    // Check if last command is whitespace and remove it
    int isWhitespace = 1;
    for (int j = 0; j < strlen(commands[numCommands - 1].command_details); j++) {
        if (!isspace(commands[numCommands - 1].command_details[j])) {
            isWhitespace = 0;
            break;
        }
    }

    if (isWhitespace) {
        numCommands--;
    }

    // Go over every command and remove any leading or trailing whitespace
    for (int i = 0; i < numCommands; i++) {
        char *command_details = commands[i].command_details;

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

    // Check using the original input string for commands that don't "&" following them
    for (int i = 0; i < numCommands; i++) {
        char *command_details = commands[i].command_details;

        // Search for the command location in the raw input string
        char *command_location = strstr(rawInput, command_details);

        // Print the character after the command_location that is not white space
        char *next_char = command_location + strlen(command_details);
        while (isspace(*next_char)) {
            next_char++;
        }

        // Check if the next character is a "&"
        if (*next_char != '&') {
            commands[i].printProcId = 0;
        }
    }

    // Initialize the struct CommandList and populate it
    cl->commands = (struct Command *)malloc(sizeof(struct Command) * numCommands);
    if (cl->commands == NULL) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, MEMORY_ALLOC_ERROR);
        fprintf(stderr, RESET_COLOR);
        exit(EXIT_FAILURE);
    }

    cl->num_commands = numCommands;
    for (int i = 0; i < numCommands; i++) {
        cl->commands[i] = commands[i];
    }

    // Free the memory for rawInput
    free(rawInput);

    return 0; // Success
}
