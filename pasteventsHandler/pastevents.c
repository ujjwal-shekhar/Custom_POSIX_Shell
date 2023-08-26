#include "../headers.h"

int showHistory(char **errorString) {
    // Read the pastevents.log file and output it in reverse order
    FILE *fp = fopen("/home/ujjwal-shake-her/M23_Code_Thingies/OSN/A1/pasteventsHandler/pastevents.log", "r");

    // If the file does not exist, print an error message
    if (fp == NULL) {
        errorHandler("\033[31mpastevents.log does not exist\033[0m", errorString);
        return 1;
    }

    char history[15][4096];
    // Read the contents of the file into the array
    // Each string will be newline separated
    // There will be a maximum of 15 strings
    // Autocompleted by ChatGPT3.5-Turbo
    int numLines = 0;
    while (numLines < 15 && fgets(history[numLines], sizeof(history[numLines]), fp)) {
        // Remove the newline character at the end
        history[numLines][strcspn(history[numLines], "\n")] = '\0';
        numLines++;
    }

    fclose(fp);

    // Output the history in reverse order
    // Autocompleted by ChatGPT3.5-Turbo
    for (int i = numLines - 1; i >= 0; i--) {
        printf("%s\n", history[i]);
    }

    return 0;
}

int clearHistory(char **errorString) {
    // Clear the log file
    FILE *fp = fopen("/home/ujjwal-shake-her/M23_Code_Thingies/OSN/A1/pasteventsHandler/pastevents.log", "w");
    if (fp == NULL) {
        errorHandler("\033[31mpastevents.log does not exist\033[0m", errorString);
        return 1;
    }
    
    fclose(fp);
}

int addEventToHistory(struct CommandList cl, int dontAddToHistory) {
    // If we are not supposed to add to history then we won't
    // if (dontAddToHistory) return 0;

    // Make an empty command_details string
    // The plan is to append the each command in cl
    // to this with a ";" or "&" in between depending
    // on the print proc id flag
    // A special case to handle is that
    // the last command should not have a ";", it can
    // however have an "&"
    // Another thing to handle is that if "pastevents execute i"
    // is found, we will first replace that with the command
    // at index i in the history and then add it to the history
    // Apart from this we will also have to handle the case
    // where the command is `pastevents purge` or `pastevents`
    // In this case we will not add anything to the history
    // Autocompleted by ChatGPT3.5-Turbo
    char command_details[4096] = "";

    // If the command is `pastevents purge` or `pastevents`
    // then we will not add anything to the history
    int numCommands = cl.num_commands;
    struct Command *commands = cl.commands;

    // Let's iterate over every command and replace 
    // all occurences of pastevents execute i
    // with the command at index i in the history

    // char *command_details_temp = malloc(4096 * sizeof(char));
    // if (command_details_temp == NULL) {
    //     printf("Malloc failed\n");
    //     return 1;
    // }
    // command_details_temp[0] = '\0';

    // // Open the log file
    // FILE *fp = fopen("/home/ujjwal-shake-her/M23_Code_Thingies/OSN/A1/pasteventsHandler/pastevents.log", "r");
    // if (fp == NULL) {
    //     errorHandler("\033[31mpastevents.log does not exist\033[0m", errorString);
    //     return 1;
    // }

    // // Read the log file
    // char history[15][4096];
    // // Read the contents of the file into the array
    // // Each string will be newline separated
    // // There will be a maximum of 15 strings
    // // Autocompleted by ChatGPT3.5-Turbo
    // int numLines = 0;
    // while (numLines < 15 && fgets(history[numLines], sizeof(history[numLines]), fp)) {
    //     // Remove the newline character at the end
    //     history[numLines][strcspn(history[numLines], "\n")] = '\0';
    //     numLines++;
    // }

    // fclose(fp);

    // // Reverse the array
    // for (int i = 0; i < numLines / 2; i++) {
    //     char temp[4096];
    //     strcpy(temp, history[i]);
    //     strcpy(history[i], history[numLines - i - 1]);
    //     strcpy(history[numLines - i - 1], temp);
    // }

    // // Print the array
    // // for (int i = 0; i < numLines; i++) {
    // //     printf("History : %s\n", history[i]);
    // // }

    // int fetchIndex = -1;

    // // Check if the fetchIndex is valid
    // if ((fetchIndex >= numLines) || (fetchIndex < 0)) {
    //     // errorHandler("\033[31mInvalid index\033[0m", errorString);
    //     return 1;
    // }

    // // Copy the command_details to the command_details pointer
    // strcpy(command_details_temp, history[fetchIndex]);

    //
}

int getIndexInHistory(char **errorString, int fetchIndex, char ** command_details) {
    // We will open the log file and read the contents
    // and the fetchIndex will fetch the file at the
    // ith position in the reverse file
    // Autocompleted by ChatGPT3.5-Turbo


    // Open the log file
    FILE *fp = fopen("/home/ujjwal-shake-her/M23_Code_Thingies/OSN/A1/pasteventsHandler/pastevents.log", "r");
    if (fp == NULL) {
        errorHandler("\033[31mpastevents.log does not exist\033[0m", errorString);
        return 1;
    }

    // Read the log file
    char history[15][4096];
    // Read the contents of the file into the array
    // Each string will be newline separated
    // There will be a maximum of 15 strings
    // Autocompleted by ChatGPT3.5-Turbo
    int numLines = 0;
    while (numLines < 15 && fgets(history[numLines], sizeof(history[numLines]), fp)) {
        // Remove the newline character at the end
        history[numLines][strcspn(history[numLines], "\n")] = '\0';
        numLines++;
    }

    fclose(fp);

    // Reverse the array
    for (int i = 0; i < numLines / 2; i++) {
        char temp[4096];
        strcpy(temp, history[i]);
        strcpy(history[i], history[numLines - i - 1]);
        strcpy(history[numLines - i - 1], temp);
    }

    // Print the array
    // for (int i = 0; i < numLines; i++) {
    //     printf("History : %s\n", history[i]);
    // }

    // Check if the fetchIndex is valid
    if ((fetchIndex >= numLines) || (fetchIndex < 0)) {
        errorHandler("\033[31mInvalid index\033[0m", errorString);
        return 1;
    }

    // Copy the command_details to the command_details pointer
    strcpy(*command_details, history[fetchIndex]);

    
    return 0;
}