#include "headers.h"

// int sigchldReceived = 0;
void sigchld_handler(int signo) {
    if(signo==SIGCHLD){
        update_background_status();
        // sigchldReceived = 1;

    }
}

int main()
{
    // Get the working directory of starting directory
    char starting_directory[4096];
    getcwd(starting_directory, 4096);

    // Store the previous working directory
    char * previous_directory;
    previous_directory = (char * ) malloc(sizeof(char) * 4096);
    previous_directory[0] = '\0';

    // Previous foreground processes to store
    char prevCommandName[128]; prevCommandName[0] = '\0';
    double time_taken = 0.0;
    int resetTimeTaken = 0;

    // Rerun pastevent
    int rerunPastEvent = 0;

    // Store the past command details
    char * prevCommDetails = (char *) malloc(4096 * sizeof(char));
    if (prevCommDetails == NULL) {
        printf("\033[31mFailed to allocate memory\033[0m\n");
        exit(EXIT_FAILURE);
    }
    prevCommDetails[0] = '\0';
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);
    // Keep accepting commands
    while (1)
    {

        // Call the signal handler
        // signal(SIGCHLD, sigchld_handler);
        
        
        // Check for background processes
        // if (sigchldReceived) {
       
            // sigchldReceived = 0;
        // }

        if (resetTimeTaken) {
            time_taken = 0.0;
            prevCommandName[0] = '\0';
        }

        // Print appropriate prompt with username, systemname and directory before accepting input
        char input[4096];
        if (!rerunPastEvent) {
            prompt(starting_directory, prevCommandName, time_taken);
            fgets(input, 4096, stdin);
        }
        else {
            // printf("Rerunning past event\n");
            strcpy(input, prevCommDetails);
            rerunPastEvent = 0;
            prevCommDetails[0] = '\0';
        }
        resetTimeTaken = 1;

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

            int num_args = ca.num_args;
            char* commandName = ca.command_args[0];
            int isBackground = commands[j].printProcId;

            if (isBackground) {
                // Check if command is a valid system command
                pid_t pid = fork();
                if (pid < 0) { // Failed fork
                    // perror("Fork failed");
                    errorHandler("Fork failed", &errorString);
                } else if (pid == 0) { // Child process block
                    // Make the last entry of command_args as NULL
                    command_args[num_args] = NULL;

                    // Open /dev/null for reading and writing
                    int null_fd = open("/dev/null", O_RDWR);

                    // Check if the file descriptor is valid
                    if (null_fd == -1) {
                        errorHandler("\033[31mFailed to open /dev/null\033[0m", &errorString);
                    }

                    // Redirect standard input, output, and error to /dev/null
                    dup2(null_fd, STDIN_FILENO);
                    dup2(null_fd, STDOUT_FILENO);
                    dup2(null_fd, STDERR_FILENO);

                    // Close the /dev/null file descriptor
                    close(null_fd);

                    // Call execvp
                    erroneousFlag = execvp(commandName, command_args);

                    if (erroneousFlag == -1) {
                        // printf("\033[31mERROR: ''\033[0m%s\n", errorString);
                        // free(errorString);
                        // errorOccured = 1;
                        printf("\033[31mERROR: '%s' is not a valid command\033[0m\n", commandName);
                        exit(EXIT_FAILURE);
                    }
                } else { // Parent process block
                    add_background_process(pid, command_details);
                    printf("[%d]\n", pid); // Print the PID of the background process
                }
            } else {
                pid_t pid = fork();

                char * userCommandsList[] = {"warp\0", "peek\0", "pastevents\0", "proclore\0", "exit\0", "seek\0", NULL};
                clock_t start_time, end_time;  // Variables to store start and end times
                
                // Check if the commandName is one of the the userCommandsList
                int isUserCommand = 0;
                for (int i = 0; (userCommandsList[i] != NULL); i++) {
                    if (strcmp(commandName, userCommandsList[i]) == 0) {
                        isUserCommand = 1;
                        break;
                    }
                }

                if (pid == 0) { // Child process block
                    if (isUserCommand) {
                        erroneousFlag = executeCommand(commandName, num_args, command_args, &errorString, starting_directory, &previous_directory, &prevCommDetails);
                    
                        if (erroneousFlag == 2) {
                            // A past events execute successfully completed 
                            // and the command details are stored in prevCommDetails
                            printf("The command details are : %s\n", prevCommDetails);
                            rerunPastEvent = 1;
                            erroneousFlag = 0;

                            // continue;
                            // Store the command_details in another temp array
                        }
                    }
                    else {
                        // Make the last entry of command_args as NULL
                        command_args[num_args] = NULL;

                        // Call execvp
                        erroneousFlag = execvp(commandName, command_args);
                        printf("\033[31mERROR : %s is not a valid command\033[0m\n", commandName);
                        exit(EXIT_FAILURE);
                        erroneousFlag = 0;
                    }
                } else if (pid < 0) { // Error in forking block
                    perror("fork");
                } else { // Parent process block
                    // Start timer
                    time_t start = time(NULL);
                    int status;
                    if (waitpid(pid, &status, 0) > 0) {
                        // if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                        //     printf("Foreground process executed successfully\n");
                        // } else {
                        //     printf("Foreground process terminated abnormally or with an error\n");
                        // }
                        // End timer
                        time_t end = time(NULL);

                        // Calculate time taken
                        time_taken = difftime(end, start);

                        // Set the command Name in case the prompt might need it
                        strcpy(prevCommandName, commandName);

                        // Time taken to be reset next cycle or noot
                        resetTimeTaken = 0;
                
                        // printf("Time taken : %lf\n", time_taken);
                    } else {
                        perror("waitpid");
                        // Handle waitpid error
                    }
                }
                
                if (erroneousFlag) {
                    printf("\033[31mERROR: \033[0m%s\n", errorString);
                    free(errorString);
                    errorOccured = 1;
                }
            }
        }

        // addEventToHistory(cl, dontAddToHistory);

        // if ((!dontAddToHistory) && (!errorOccured)) {
        //     addEventToHistory()
        // }
         check_background_processes();
    }
}
