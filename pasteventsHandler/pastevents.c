#include "../headers.h"

int showHistory() {
    // Read the pastevents.log file and output it in reverse order
    FILE *fp = fopen(PAST_EVENTS_PATH, "r");

    // If the file does not exist, print an error message
    if (fp == NULL) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, "ERROR : pastevents.log does not exist\n");
        fprintf(stderr, RESET_COLOR);
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

int clearHistory() {
    // Clear the log file
    FILE *fp = fopen(PAST_EVENTS_PATH, "w");
    if (fp == NULL) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, "ERROR : pastevents.log does not exist\n");
        fprintf(stderr, RESET_COLOR);
        return 1;
    }
    
    fclose(fp);
}

int addEventToHistory(char * input) {
    // Write the entire input onto the
    // pastevents.log file without trailing newlines
    // Autocompleted by ChatGPT3.5-Turbo

    // Get the last line from the log file
    FILE *fp = fopen(PAST_EVENTS_PATH, "r");
    if (fp == NULL) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, "ERROR : pastevents.log does not exist\n");
        fprintf(stderr, RESET_COLOR);
        return 2;
    }

    // Get the last line of the log
    char lastLine[4096];
    while (fgets(lastLine, sizeof(lastLine), fp)) {
        // Remove the newline character at the end
        lastLine[strcspn(lastLine, "\n")] = '\0';
    }

    fclose(fp);

    // Open the log file
    fp = fopen(PAST_EVENTS_PATH, "a");
    if (fp == NULL) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, "ERROR : pastevents.log does not exist\n");
        fprintf(stderr, RESET_COLOR);
        return 2;
    }

    // Remove trailing whitespace from input
    input[strcspn(input, "\n")] = '\0';

    // Write the input to the file if it doesnt match the last line
    if (strcmp(input, lastLine) != 0)
        fprintf(fp, "\n%s", input);

    fclose(fp);

    // If the number of items in the log file is greater than 15
    // then we will remove the first line
    // Autocompleted by ChatGPT3.5-Turbo

    // Open the log file
    fp = fopen(PAST_EVENTS_PATH, "r");
    if (fp == NULL) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, "ERROR : pastevents.log does not exist\n");
        fprintf(stderr, RESET_COLOR);
        return 2;
    }

    // Read the log file
    char history[16][4096];
    // Read the contents of the file into the array
    // Each string will be newline separated
    // There will be a maximum of 15 strings
    // Autocompleted by ChatGPT3.5-Turbo
    int numLines = 0;
    while (numLines < 16 && fgets(history[numLines], sizeof(history[numLines]), fp)) {
        // Remove the newline character at the end
        history[numLines][strcspn(history[numLines], "\n")] = '\0';
        numLines++;
    }
    

    fclose(fp);

    // If the number of lines is greater than 15
    // then we will remove the first line
    if (numLines >= 16) {
        // Open the log file
        fp = fopen(PAST_EVENTS_PATH, "w");
        if (fp == NULL) {
            fprintf(stderr, RED_COLOR);
            fprintf(stderr, "ERROR : pastevents.log does not exist\n");
            fprintf(stderr, RESET_COLOR);
            return 2;
        }

        // Write the contents of the array to the file
        for (int i = 1; i < numLines - 1; i++) {
            fprintf(fp, "%s\n", history[i]);
        }
        fprintf(fp, "%s", history[numLines - 1]);

        fclose(fp);
    }
    
    return 0;
}

int getIndexInHistory(int fetchIndex, char ** command_details) {
    // We will open the log file and read the contents
    // and the fetchIndex will fetch the file at the
    // ith position in the reverse file
    // Autocompleted by ChatGPT3.5-Turbo


    // Open the log file
    FILE *fp = fopen(PAST_EVENTS_PATH, "r");
    if (fp == NULL) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, "ERROR : pastevents.log does not exist\n");
        fprintf(stderr, RESET_COLOR);
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
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, "ERROR : Invalid index\n");
        fprintf(stderr, RESET_COLOR);
        return 1;
    }

    // Copy the command_details to the command_details pointer
    strcpy(*command_details, history[fetchIndex]);

    return 0;
}

// Function to replace "pastevents execute i" with history entry
int replacePastEventCommands(char *input) {
    int modified = 0; // Flag to check if any replacement was made

    // Open the log file
    FILE *fp = fopen(PAST_EVENTS_PATH, "r");
    if (fp == NULL) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, "ERROR : pastevents.log does not exist\n");
        fprintf(stderr, RESET_COLOR);
        return 2;
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


    for (int i = 0; i < numLines; i++) {
        char pastEventCmd[50];
        sprintf(pastEventCmd, "pastevents execute %d", i + 1);

        char *pos = strstr(input, pastEventCmd);
        if (pos != NULL) {
            // Replace the occurrence with history entry
            strcpy(pos, history[i]);
            modified = 1; // Flag that a replacement was made
        }
    }

    return modified;
}