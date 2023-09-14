#include "headers.h"
#include "prompt.h"

// #include "utils/customStructs.h"

// #include "utils/pathHandler.h"
// #include "utils/flagHandler.h"
// #include "utils/errorHandler.h"
// #include "utils/inputHandlers/commandExecutor.h"
// #include "utils/inputHandlers/commandHandler.h"
// #include "utils/inputHandlers/commandArgsHandler.h"

#include "warpHandler/warp.h"
#include "peekHandler/peek.h"
#include "seekHandler/seek.h"

#include "procloreHandler/proclore.h"
#include "pingHandler/ping.h"
#include "activitiesHandler/activities.h"

#include "pasteventsHandler/pastevents.h"

#include "systemCommands/backgroundManager/backgroundProcHandler.h"
#include "systemCommands/foregroundManager/foregroundProcHandler.h"

#include "redirPipeHandler/pipeHandler.h"
#include "redirPipeHandler/redirectionHandler.h"

#include "iManHandler/iManHandler.h"
#include "neonateHandler/neonate.h"

#include "fgbgHandler/fg.h"
#include "fgbgHandler/bg.h"
// USAGE OF CHATGPT
// A lot of the signal handling came
// from chatgpt
// and this https://www.gnu.org/software/libc/manual/html_node/Process-Completion.html

struct ProcessList globalProcessList;

void setup_processes_list() {
    // Initialize the global ProcessList variable
    globalProcessList.numProcesses = 0;
    globalProcessList.backgroundProcesses = (struct ProcessDetails *) malloc(sizeof(struct ProcessDetails) * MAX_BACKGROUND_PROCESSES);
    if (globalProcessList.backgroundProcesses == NULL) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, MEMORY_ALLOC_ERROR);
        fprintf(stderr, RESET_COLOR);
        exit(EXIT_FAILURE);
    }

    // Malloc for every background Process
    for (int i = 0; i < MAX_BACKGROUND_PROCESSES; i++) {
        globalProcessList.backgroundProcesses[i].commandName = (char *) malloc(sizeof(char) * 128);
        if (globalProcessList.backgroundProcesses[i].commandName  == NULL) {
            fprintf(stderr, RED_COLOR);
            fprintf(stderr, MEMORY_ALLOC_ERROR);
            fprintf(stderr, RESET_COLOR);
            exit(EXIT_FAILURE);
        }

        globalProcessList.backgroundProcesses[i].status = (char *) malloc(sizeof(char) * 128);
        if (globalProcessList.backgroundProcesses[i].status  == NULL) {
            fprintf(stderr, RED_COLOR);
            fprintf(stderr, MEMORY_ALLOC_ERROR);
            fprintf(stderr, RESET_COLOR);
            exit(EXIT_FAILURE);
        }
    }
}

// int sigchldReceived = 0;
void sigchld_handler(int signo) {
    if (signo == SIGCHLD){
        update_background_status(&globalProcessList);
    } else if (signo == SIGCONT) {
        update_background_status(&globalProcessList);
    } else if (signo == SIGSTOP) {
        update_background_status(&globalProcessList);
    }
}

void signal_setup() {
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, MEMORY_ALLOC_ERROR);
        fprintf(stderr, RESET_COLOR);
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

    // Store the past command details
    char * prevCommDetails = (char *) malloc(4096 * sizeof(char));
    if (prevCommDetails == NULL) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, MEMORY_ALLOC_ERROR);
        fprintf(stderr, RESET_COLOR);
        exit(EXIT_FAILURE);
    }
    prevCommDetails[0] = '\0';

    // Setup the signal handlers
    signal_setup();    
    
    // Setup the processList
    setup_processes_list();

    // Keep accepting commands
    while (1)
    {
        // // Ignore Ctrl + C
        // signal(SIGINT, SIG_IGN);

        // // Ignore Ctrl + Z
        // signal(SIGSTOP, SIG_IGN);

        // Call the signal handler
        if (resetTimeTaken) {
            time_taken = 0.0;
            prevCommandName[0] = '\0';
        }

        // Print appropriate prompt with username, systemname and directory before accepting input
        char input[4096];
        prompt(starting_directory, prevCommandName, time_taken);
        fgets(input, 4096, stdin);


        replacePastEventCommands(input);

        char rawInput[4096]; // Save the input for later use
        strcpy(rawInput, input);

        int dontAddToHistory = 0;
        int errorOccured = 0;

        // struct CommandList cl = tokenizeInput(input);
        struct CommandList cl;
        int status = tokenizeInput(input, &cl);

        // TODO : Handle the error val returned
        // if (status == 0) {
        //     // Successfully tokenized input
        //     // Access the result in cl.num_commands and cl.commands
        // } else {
        //     // Handle error based on the value of 'status'
        // }
        int numCommands = cl.num_commands;
        struct Command *commands = cl.commands;

        int j;
        int erroneousFlag = 0;

        // Execute the commands if in foreground
        for (j = 0; j < numCommands; j++) {    
            int isBackground = commands[j].printProcId;

            // Split each command by whitespace
            char *command_details = commands[j].command_details;
            char *command_args[4096];

            struct PipedCommandDetails pcd;

            pipe_split(command_details, command_args, isBackground, starting_directory, &previous_directory, &pcd);

            // // Print the piped command details
            // printf("Piped Command Details\n");
            // printf("Number of piped commands : %d\n", pcd.num_piped_commands);
            // for (int i = 0; i < pcd.num_piped_commands; i++) {
            //     struct CommandArgs ca = pcd.ca[i];
            //     printf("Command %d : ", i);
            //     for (int j = 0; j < ca.num_args; j++) {
            //         printf("%s ", ca.command_args[j]);
            //     }
            //     printf("\nInput fd : %d, Output fd : %d\n", ca.input_fd, ca.output_fd);
            //     printf("\n");
            // }

            struct CommandArgs ca = pcd.ca[0];
            ca.command_args[ca.num_args] = NULL;
            char* commandName = ca.command_args[0];
            int num_args = ca.num_args;    

            pid_t shell_pid = getpid();

            // TODO: fprintf to the stderr

            if (isBackground) {
                execute_background_process(shell_pid, ca, commandName, num_args, command_details, &globalProcessList);
            } else {
                if (pcd.num_piped_commands == 1) {
                    if (checkUserCommand(commandName)) {
                        erroneousFlag = executeCommand(commandName, num_args, ca.command_args, starting_directory, &previous_directory, &prevCommDetails, &globalProcessList);
                    
                        if (erroneousFlag == 2) {
                            dontAddToHistory = 1;
                            erroneousFlag = 1;
                        }

                    } else {
                        pid_t pid = fork();
                    
                        if (pid < 0) {
                            perror("fork");
                        } else if (pid == 0) { // Child process code
                            // // Reset Ctrl + C
                            // signal(SIGINT, SIG_DFL);

                            // // Reset Ctrl + Z
                            // signal(SIGSTOP, SIG_DFL);
                            
                            // Set the input and output to input_fd and output_fd
                            if (ca.input_fd != -1) {
                                dup2(ca.input_fd, 0);
                            }
                            if (ca.output_fd != -1) {
                                dup2(ca.output_fd, 1);
                            }

                            // Set the process group leader
                            setpgid(0, 0);

                            // Make the last entry of command_args as NULL
                            ca.command_args[num_args] = NULL;

                            // Call execvp
                            execvp(commandName, ca.command_args);

                            // printf("\033[31mERROR : %s is not a valid command\033[0m\n", commandName);
                            fprintf(stderr, RED_COLOR);
                            fprintf(stderr, "ERROR : %s is not a valid command\n", commandName);
                            fprintf(stderr, RESET_COLOR);

                            exit(EXIT_FAILURE);                            
                            // Error handling

                        } else {
                            // Close the input and output file descriptors
                            if (ca.input_fd != 0) {
                                close(ca.input_fd);
                            }
                            if (ca.output_fd != 1) {
                                close(ca.output_fd);
                            }

                            // Maintain amount of time taken
                            time_t start = time(NULL);
                            int status;
                            if (waitpid(pid, &status, 0) > 0) {
                                // Restore stdin and stdout
                                dup2(STDIN_FILENO, 0);
                                dup2(STDOUT_FILENO, 1);

                                // Set error flag if EXITFAILURE
                                if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                                    errorOccured = 1;
                                }
                                time_t end = time(NULL);

                                // Calculate time taken
                                time_taken = difftime(end, start);

                                // Set the command Name in case the prompt might need it
                                strcpy(prevCommandName, commandName);

                                // Time taken to be reset next cycle or not
                                resetTimeTaken = 0;
                            } else {
                                perror("waitpid");
                                errorOccured = 1;
                            }
                        }
                    }                
                } else { // Most of this code is purely ChatGPT-ed
                    execute_foreground_process(shell_pid, pcd, commandName, num_args, command_details);
                }
            }
        }

        if ((!dontAddToHistory) && (!errorOccured)) {
            addEventToHistory(rawInput);
        }

        check_background_processes(&globalProcessList);
    }
}
