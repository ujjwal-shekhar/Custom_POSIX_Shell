#include "../../headers.h"


// int command_selector(commandName, num_args, command_args, &errorString, starting_directory, &previous_directory, &prevCommDetails) {
//     if (checkUserCommand(commandName)) {
//         erroneousFlag = executeCommand(commandName, num_args, command_args, &errorString, starting_directory, &previous_directory, &prevCommDetails);

//         if (erroneousFlag == 2) {
//             dontAddToHistory = 1;
//             erroneousFlag = 1;
//         }

//         exit(EXIT_SUCCESS);
//     } else {
//         // Make the last entry of command_args as NULL
//         command_args[num_args] = NULL;

//         // Call execvp
//         execvp(commandName, command_args);

//         // Error handling
//         printf("\033[31mERROR : %s is not a valid command\033[0m\n", commandName);
//         exit(EXIT_FAILURE);
//     }
// }

// int spawn_proc(int in, int out, char* commandName, int num_args, char *command_args[], char ** errorString, char starting_directory[], char ** previous_directory, char ** prevCommDetails) {
//     pid_t pid = fork();  

//     if (isBackground) {
//         perror("skill issue rn");
//     } else {
//         if (pid == 0) {
//             if (in != 0) {
//                 dup2(in, 0);
//                 close(in);
//             } 

//             if (out != 1) {
//                 dup2(out, 1);
//                 close(out);
//             }

//             return command_selector(commandName, num_args, command_args, &errorString, starting_directory, &previous_directory, &prevCommDetails);
//         } else if (pid > 0) {
//             clock_t start_time, end_time;  // Variables to store start and end times
//             time_t start = time(NULL);
//             int status;
//             if (waitpid(pid, &status, 0) > 0) {
//                 // Set error flag if EXITFAILURE
//                 if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
//                     errorOccured = 1;
//                 }
//                 time_t end = time(NULL);

//                 // Calculate time taken
//                 time_taken = difftime(end, start);

//                 // Set the command Name in case the prompt might need it
//                 strcpy(prevCommandName, commandName);

//                 // Time taken to be reset next cycle or noot
//                 resetTimeTaken = 0;
//             } else {
//                 perror("wait");
//                 return 1;
//             }
//         } else {
//             perror("fork");
//             return 1;
//         }
//     }
// }

int checkUserCommand(char * commandName) {
    char * userCommandsList[] = {"warp\0", "peek\0", "pastevents\0", "proclore\0", "exit\0", "seek\0", "ping\0", "activities\0", "iMan\0", NULL};
    
    // Check if the commandName is one of the the userCommandsList
    int isUserCommand = 0;
    for (int i = 0; (userCommandsList[i] != NULL); i++) {
        if (strcmp(commandName, userCommandsList[i]) == 0) {
            isUserCommand = 1;
            break;
        }
    }

    return isUserCommand;
}

int executeCommand(char* commandName, int num_args, char *command_args[], char ** errorString, char starting_directory[], char ** previous_directory, char ** prevCommDetails) {
    // Check if exit command was entered
    if (strcmp(commandName, "exit") == 0) {
        if (num_args > 1) {
            return 1;
        } else {
            exit(EXIT_SUCCESS);
        }
    }

    // Check if iMan was entered
    else if (strncmp(commandName, "iMan\0", 5) == 0) {
        // Check if there were no more than 2 arguments
        if (num_args > 2) {
            return 1;
        } else {
            return iman(command_args);
        }
    }

    // Check if proclore command was entered
    else if (strncmp(commandName, "proclore\0", 9) == 0) {
        // Check if there were no more than 2 arguments
        if (num_args > 2) {
            return 1;
        } else {
            return proclore(command_args, errorString, starting_directory, previous_directory);
        }
    }

    // Check if proclore command was entered
    else if (strncmp(commandName, "activities\0", 11) == 0) {
        // Check if there were no arguments
        if (num_args > 1) {
            return 1;
        } else {
            return activities(command_args);
        }
    }

    // Check if proclore command was entered
    else if (strncmp(commandName, "ping\0", 5) == 0) {
        // Check if there were no more than 3 arguments
        if (num_args > 3) {
            return 1;
        } else {
            return ping(command_args);
        }
    }

    // Check if warp command was entered
    else if (strncmp(commandName, "warp\0", 5) == 0) {
        int notWhiteSpace = 0;
        // Check if the entire command_arg[1] is a whitespace
        for (int k = 0; k < strlen(command_args[1]); k++) {
            if (!isspace(command_args[1][k])) {
                notWhiteSpace = 1;
                break;
            }
        }

        if (command_args[1] == NULL) {
            command_args[1] = ".";
            command_args[2] = NULL;
        } else if (notWhiteSpace == 0) {
            command_args[1] = ".";
            command_args[2] = NULL;
        }
        return warp(command_args, errorString, starting_directory, previous_directory);
    }

    // Check if `peek` command was entered
    else if (strncmp(commandName, "peek\0", 5) == 0) {
        // Check if there were no more than 4 arguments
        if (num_args > 4) {
            return 1;
        } else {
            return peek(command_args, errorString, starting_directory, previous_directory);
        }
    }

    // Check if `seek` command was entered
    else if (strncmp(commandName, "seek\0", 5) == 0) {
        // Check if there were no more than 4 arguments
        if (num_args > 6) {
            return 1;
        } else {
            return seek(command_args, errorString, starting_directory, previous_directory);
        }
    }


    // Check if `pastevents` command was entered
    else if (strncmp(commandName, "pastevents\0", 11) == 0) {
        // Check if there were no more than 3 arguments
        if (num_args > 3) {
            return 1;
        } else {
            // If only one arg : display the whole history
            if (num_args == 1) {
                showHistory(errorString);
                return 2;
            }

            // If two args, then purge the file
            else if ((num_args == 2) && (strncmp(command_args[1], "purge\0", 6) == 0)) {
                clearHistory(errorString);
                return 2;
            }

            // If three args, then execute at index
            else if ((num_args == 3) && (strncmp(command_args[1], "execute\0", 8) == 0)) {
                // Check if the input string contains only digits
                // int isOnlyDigits = 1;
                // for (int i = 0; command_args[2][i] != '\0'; i++) {
                //     if (!isdigit(command_args[2][i]) && command_args[2][i] != '\n') {
                //         isOnlyDigits = 0;
                //         break;
                //     }
                // }
                // if (!isOnlyDigits) {
                //     errorHandler("\033[31mWrong Syntax used\nCorrect Usage : pastevents <purge / execute index>\033[0m\n", errorString);
                //     return 1;
                // }

                // int errorFlag = getIndexInHistory(errorString, atoi(command_args[2]) - 1, prevCommDetails);
            
                // // printf("The command details are : %s\n", *prevCommDetails);
                // if (errorFlag != 1) {
                //     return 2;
                // } else {
                //     return 1;
                // }
                return 2; // All the pastevents execute that are valid must have already been handled
            }

            else {
                printf("The second arg :%s:%d\n", command_args[1], num_args);
                errorHandler("\033[31mWrong Syntax used\nCorrect Usage : pastevents <purge / execute index>\033[0m\n", errorString);
                return 1;
            }
        }
    }

    else 
    {
        printf("\033[31mERROR : '%s' is not a valid command\033[0m\n", command_args[0]);
        return 0;
    }

    return 0;
}