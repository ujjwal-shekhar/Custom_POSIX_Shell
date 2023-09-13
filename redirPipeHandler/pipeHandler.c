#include "../headers.h"

int splitByPipe(char *command_details, char **command_details_split) {
    int num_commands = 0;
    for (char *token = strtok(command_details, "|"); 
         token != NULL; token = strtok(NULL, "|")) 
    {
        int start = 0;
        int end = strlen(token) - 1;
        while (isspace(token[start])) start++;
        while (isspace(token[end])) end--;

        // If the token is only whitespace, then ignore it
        if (start > end) continue;

        // Add the token to the command_args array
        token[end + 1] = '\0';

        command_details_split[num_commands] = token + start;
        num_commands++;
    }

    return num_commands;
}

int pipe_split(char *command_details, char **command_args, int isBackground, char **errorString, char starting_directory[], char **previous_directory, struct PipedCommandDetails *pcd) {
    // Split the command details by "|"
    char *command_details_split[4096];
    int num_commands = splitByPipe(command_details, command_details_split);

    // Allocate memory for PipedCommandDetails
    pcd->num_piped_commands = num_commands;
    pcd->ca = (struct CommandArgs *)malloc(num_commands * sizeof(struct CommandArgs));

    // Iterate through each command
    for (int i = 0; i < num_commands; i++) {
        struct CommandArgs *ca = &(pcd->ca[i]);
        char *currCommandDetails = command_details_split[i];
        
        // Allocate memory for command_args
        ca->command_args = (char **)malloc(4096 * sizeof(char *));
        
        // Parse the command arguments
        parseCommandArgs(currCommandDetails, ca->command_args, ca);

        // Set input and output descriptors
        int null_fd = open("/dev/null", O_RDWR);
        if (isBackground) {
            ca->input_fd = -1;
            ca->output_fd = -1;
            if (i == 0) {
                ca->input_fd = null_fd;
            } 
            if (i == num_commands - 1) {
                ca->input_fd = null_fd;
            }
        } else {
            ca->input_fd = -1;
            ca->output_fd = -1;
            if (i == 0) {
                ca->input_fd = STDIN_FILENO;
            }
            
            if (i == num_commands - 1) {
                ca->output_fd = STDOUT_FILENO;
            }
        }
        close(null_fd);


        // Check for redirection
        if (strstr(ca->command_details, "<") != NULL) {
            replaceInputRedirect(ca, errorString, starting_directory, previous_directory);
        } 
        if (strstr(ca->command_details, ">") != NULL) {
            replaceOutputRedirect(ca, errorString, starting_directory, previous_directory);
        }
        if (strstr(ca->command_details, ">>") != NULL) {
            ca->outputAppend = 1;
            replaceOutputRedirectAppend(ca, errorString, starting_directory, previous_directory);
        }
    }

    return 0;
}