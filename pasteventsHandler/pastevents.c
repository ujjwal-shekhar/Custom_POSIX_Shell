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

int addEventToHistory(char * command_details, char * commandName, char **errorString, int dontAddToHistory) {
    // If we are not supposed to add to history then we won't
    if (dontAddToHistory) return 0;

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

    // Check if there are already 15 lines
    if (numLines == 15) {
        // Open the log file for writing
        fp = fopen("/home/ujjwal-shake-her/M23_Code_Thingies/OSN/A1/pasteventsHandler/pastevents.log", "w");
        if (fp == NULL) {
            errorHandler("\033[31mpastevents.log does not exist\033[0m", errorString);
            return 1;
        }

        // Write all the lines except the first one to the log file again
        for (int i = 1; i < numLines; i++) {
            fprintf(fp, "%s\n", history[i]);
        }

        fclose(fp);

    }

    // Append the command_details to the log file
    fp = fopen("/home/ujjwal-shake-her/M23_Code_Thingies/OSN/A1/pasteventsHandler/pastevents.log", "a");
    if (fp == NULL) {
        errorHandler("\033[31mpastevents.log does not exist\033[0m", errorString);
        return 1;
    }

    fprintf(fp, "%s\n", command_details);

    fclose(fp);
}

int getIndexInHistory(char **errorString, int fetchIndex) {
    return 0;
}