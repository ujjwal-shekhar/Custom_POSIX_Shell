#include "headers.h"

// USAGE OF CHATGPT
// A lot of the signal handling came
// from chatgpt
// and this https://www.gnu.org/software/libc/manual/html_node/Process-Completion.html

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

        struct CommandList cl = executeCommands(input);
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

            char * errorString;
            struct PipedCommandDetails pcd;

            pipe_split(command_details, command_args, isBackground, &errorString, starting_directory, &previous_directory, &pcd);

            // Print the piped command details
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

            if (isBackground) {
                // perror("Background processes not supported");
                int pipe_fd[pcd.num_piped_commands-1][2];
                if (pcd.num_piped_commands == 1) {
                    // No pipes, execute the command directly
                    struct CommandArgs ca = pcd.ca[0];
                    ca.command_args[ca.num_args] = NULL;
                    char* commandName = ca.command_args[0];
                    int num_args = ca.num_args;

                    printf("backgnopipes\n");
                    pid_t pid = fork();
                
                    if (pid < 0) {
                        perror("fork");
                    } else if (pid == 0) {
                        // Make the last entry of command_args as NULL
                        command_args[num_args] = NULL;

                        // Open /dev/null for reading and writing
                        int null_fd = open("/dev/null", O_RDWR);

                        // Check if the file descriptor is valid
                        if (null_fd == -1) {
                            errorHandler("\033[31mFailed to open /dev/null\033[0m", &errorString);
                        }

                        // Redirect standard input, output, and error to /dev/null
                        if (ca.input_fd != -1)
                            dup2(ca.input_fd, STDIN_FILENO);
                        else
                            dup2(null_fd, STDIN_FILENO);

                        if (ca.output_fd != -1)
                            dup2(ca.output_fd, STDOUT_FILENO);
                        else
                            dup2(null_fd, STDOUT_FILENO);

                        dup2(null_fd, STDERR_FILENO);

                        // Close the /dev/null file descriptor
                        close(null_fd);

                        // Call execvp
                        erroneousFlag = execvp(commandName, ca.command_args);

                        if (erroneousFlag == -1) {
                            printf("\033[31mERROR: '%s' is not a valid command\033[0m\n", commandName);
                            erroneousFlag = 1;
                            exit(EXIT_FAILURE);
                        }
                        
                    } else { // Parent process block
                        add_background_process(pid, command_details);
                        printf("[%d]\n", pid); // Print the PID of the background process
                    }
                } else { // Most of this code is purely ChatGPT-ed
                    for (int job=0; job<pcd.num_piped_commands; job++) {
                        struct CommandArgs ca = pcd.ca[job];
                        ca.command_args[ca.num_args] = NULL;
                        char* commandName = ca.command_args[0];

                        if (job < pcd.num_piped_commands - 1)
                            pipe(pipe_fd[job]);

                        pid_t pid = fork();

                        if (pid < 0) {
                            perror("fork");
                        } else if (pid == 0) {
                            if (job != 0) {
                                dup2(pipe_fd[job-1][0], 0);
                                // close(pipe_fd[job-1][1]);
                            } else if (ca.input_fd != -1) {
                                dup2(ca.input_fd, 0);
                            }

                            if (job != pcd.num_piped_commands - 1) {
                                dup2(pipe_fd[job][1], 1);
                                // close(pipe_fd[job][0]);
                            } else if (ca.output_fd != -1) {
                                dup2(ca.output_fd, 1);
                            }


                            execvp(commandName, ca.command_args);

                            exit(EXIT_FAILURE);
                        } 
                        else{
                            if (job != 0) {
                                close(pipe_fd[job-1][0]);
                            } else if (pcd.ca->input_fd != -1 && pcd.ca->input_fd != 0) {
                                close(pcd.ca->input_fd);
                            }
                            if (job != pcd.num_piped_commands - 1) {
                                close(pipe_fd[job][1]);
                            } else if (pcd.ca->output_fd != -1 && pcd.ca->output_fd != 1) {
                                close(pcd.ca->output_fd);
                            }
                            wait(NULL);
                        }
                    }
                
                }

            } else {
                int pipe_fd[pcd.num_piped_commands-1][2];
                if (pcd.num_piped_commands == 1) {
                    // No pipes, execute the command directly
                    struct CommandArgs ca = pcd.ca[0];
                    ca.command_args[ca.num_args] = NULL;
                    char* commandName = ca.command_args[0];
                    int num_args = ca.num_args;

                    if (checkUserCommand(commandName)) {
                        erroneousFlag = executeCommand(commandName, num_args, ca.command_args, &errorString, starting_directory, &previous_directory, &prevCommDetails);
                    
                        if (erroneousFlag == 2) {
                            dontAddToHistory = 1;
                            erroneousFlag = 1;
                        }

                    } else {
                        pid_t pid = fork();
                    
                        if (pid < 0) {
                        perror("fork");
                        } else if (pid == 0) {
                            // Child process code
                            // Make the last entry of command_args as NULL
                            ca.command_args[num_args] = NULL;

                            // Call execvp
                            execvp(commandName, ca.command_args);

                            // Error handling
                            printf("\033[31mERROR : %s is not a valid command\033[0m\n", commandName);
                            // printf("hi hcild\n");
                            exit(EXIT_FAILURE);
                            
                        } else {
                            time_t start = time(NULL);
                            int status;
                            if (waitpid(pid, &status, 0) > 0) {
                                // Set error flag if EXITFAILURE
                                if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                                    errorOccured = 1;
                                }
                                time_t end = time(NULL);

                                // Calculate time taken
                                time_taken = difftime(end, start);

                                // Set the command Name in case the prompt might need it
                                strcpy(prevCommandName, commandName);

                                // Time taken to be reset next cycle or noot
                                resetTimeTaken = 0;
                            } else {
                                perror("waitpid");
                                errorOccured = 1;
                            }
                        }
                    }                
                } else { // Most of this code is purely ChatGPT-ed
                    for (int job=0; job<pcd.num_piped_commands; job++) {

                        if (job < pcd.num_piped_commands - 1)
                            pipe(pipe_fd[job]);

                        pid_t pid = fork();

                        if (pid < 0) {
                            perror("fork");
                        } else if (pid == 0) {
                            struct CommandArgs ca = pcd.ca[job];
                            ca.command_args[ca.num_args] = NULL;
                            char* commandName = ca.command_args[0];

                            if (job != 0) {
                                dup2(pipe_fd[job-1][0], 0);
                                // close(pipe_fd[job-1][1]);
                            } else if (ca.input_fd != -1) {
                                dup2(ca.input_fd, 0);
                            }

                            if (job != pcd.num_piped_commands - 1) {
                                dup2(pipe_fd[job][1], 1);
                                // close(pipe_fd[job][0]);
                            } else if (ca.output_fd != -1) {
                                dup2(ca.output_fd, 1);
                            }

                            execvp(commandName, ca.command_args);

                            exit(EXIT_FAILURE);
                        } 
                        else{
                            if (job != 0) {
                                close(pipe_fd[job-1][0]);
                            } else if (pcd.ca->input_fd != -1 && pcd.ca->input_fd != 0) {
                                close(pcd.ca->input_fd);
                            }
                            if (job != pcd.num_piped_commands - 1) {
                                close(pipe_fd[job][1]);
                            } else if (pcd.ca->output_fd != -1 && pcd.ca->output_fd != 1) {
                                close(pcd.ca->output_fd);
                            }
                            wait(NULL);
                        }
                    }
                }
            }
        }

        if ((!dontAddToHistory) && (!errorOccured)) {
            addEventToHistory(rawInput);
        }

         check_background_processes();
    }
}
