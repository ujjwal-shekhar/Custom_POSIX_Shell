#include "../headers.h"

// USAGE OF GITHUB COPILOT:
// - All the if and else conditions were written by me (Ujjwal Shekhar)
// - Some portions of the logic was completed by Copilot, and the rest of it was by me
// - It will be hard to demarcate the exact lines written by me and Copilot
// However, the split of lines written solely by me versus Copilot should come to be
// around 40 : 60 (me : Copilot) 

// Returns 0 : if successful, 1 : if failed
int warp(char ** command_args, char * errorString, char starting_directory[]) {
    // Iterate over all command_args
    int i = 1;
    while (command_args[i] != NULL) {
        // Check if the character "~" occurs in the command
        // Replace that character with starting_directory
        if (strchr(command_args[i], '~') != NULL) {
            // Replace the character "~" with starting_directory
            char *new_directory = malloc(4096);
            strcpy(new_directory, starting_directory);
            strcat(new_directory, command_args[i] + 1);

            // Check if the path exists
            if (access(new_directory, F_OK) == 0) {
                // Change the directory
                if (chdir(new_directory) != 0) {
                    errorString = "\033[31mThere was some issue when using chdir\033[0m";
                    return 1;
                }
            } else {
                errorString = "\033[31mThe path is not valid\033[0m";
                return 1;
            }

            i++;
            continue;
        }

        // Handling special cases
        // If the argument is "-" then we got the OLD_PWD
        if (strcmp(command_args[i], "-") == 0) {
            // Check if the environment variable is set
            if (getenv("OLDPWD") == NULL) {
                errorString = "\033[31mOLDPWD is not set\033[0m";
                return 1;
            }

            // Change the directory
            if (chdir(getenv("OLDPWD")) != 0) {
                errorString = "\033[31mThere was some issue when using chdir\033[0m";
                return 1;
            }

        }

        // If the path has the character '/' then
        // we treat it as an absolute path
        else if (strchr(command_args[i], '/') != NULL) {
            // Check if the path exists
            if (access(command_args[i], F_OK) == 0) {
                // Change the directory
                if (chdir(command_args[i]) != 0) {
                    errorString = "\033[31mThere was some issue when using chdir\033[0m";
                    return 1;
                }
            } else {
                errorString = "\033[31mThe path is not valid\033[0m";
                return 1;
            }
        } else {
            // If the path does not have the character '/'
            // then we treat it as a relative path
            char current_directory[4096];
            getcwd(current_directory, 4096);

            // Concatenate the current directory with the relative path
            char new_directory[4096];
            strcpy(new_directory, current_directory);
            strcat(new_directory, "/");
            strcat(new_directory, command_args[i]);

            // Check if the path exists
            if (access(new_directory, F_OK) == 0) {
                // Change the directory
                if (chdir(new_directory) != 0) {
                    errorString = "\033[31mThere was some issue when using chdir\033[0m";
                    return 1;
                }
            } else {
                errorString = "\033[31mThe path is not valid\033[0m";
                return 1;
            }
        }        

        // Print the absolute the path of the curren working directory
        char current_directory[4096];
        getcwd(current_directory, 4096);
        printf("%s\n", current_directory);
        i++;
    }
}