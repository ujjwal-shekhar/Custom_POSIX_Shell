#include "proclore.h"

#define MAX_PATH_LEN 64

// Implement pinfo command
// Information required to print :
// pid
// Process Status (R/R+/S/S+/Z)
// Process group
// Virtual Memory
// Executable path of process

// Mostly written with the help of
// ChatGPT and https://stackoverflow.com/questions/41224738/how-to-get-the-executable-path-of-a-process-in-linux
// and https://man7.org/linux/man-pages/man5/proc.5.html 

int proclore(char **command_args, char starting_directory[], char **previous_directory) {
    int pid;
    // Check if the command_args[1] is NULL
    if (command_args[1] == NULL) {
        // Get the pid of the current process
        pid = getpid();
    } else {
        // Get the pid from the command_args[1]
        pid = atoi(command_args[1]);
    }

    char statPath[MAX_PATH_LEN + 1];
    char statmPath[MAX_PATH_LEN + 1];
    char exePath[MAX_PATH_LEN + 1];

    snprintf(statPath, sizeof(statPath), "/proc/%d/stat", pid);
    snprintf(statmPath, sizeof(statmPath), "/proc/%d/statm", pid);
    snprintf(exePath, sizeof(exePath), "/proc/%d/exe", pid);

    FILE *statFile = fopen(statPath, "r");
    FILE *statmFile = fopen(statmPath, "r");

    if (statFile == NULL || statmFile == NULL) {
        perror("Error opening stat or statm file");
        exit(EXIT_FAILURE);
    }

    // Read and parse the stat file
    char statBuffer[MAX_PATH_LEN + 1];
    if (fgets(statBuffer, sizeof(statBuffer), statFile) == NULL) {
        perror("Error reading stat file");
        exit(EXIT_FAILURE);
    }

    char processStatus;
    int processGroup;
    unsigned long vmSize;

    sscanf(statBuffer, "%*d %*s %c %d", &processStatus, &processGroup);

    // Read and parse the statm file
    if (fscanf(statmFile, "%lu", &vmSize) != 1) {
        perror("Error reading statm file");
        exit(EXIT_FAILURE);
    }

    // Get the executable path
    char realPath[MAX_PATH_LEN + 1];
    ssize_t len = readlink(exePath, realPath, sizeof(realPath));
    if (len == -1) {
        perror("Error reading executable path");
        exit(EXIT_FAILURE);
    }
    realPath[len] = '\0';

    // Print the collected information
    printf("Process ID (PID): %d\n", pid);
    printf("Process Status: %c", processStatus);

    // Check if the process is a foreground process
    // by comparing the process group with the process id
    if (processGroup == pid) {
        printf("+\n");
    } else {
        printf("\n");
    }

    printf("Process Group: %d\n", processGroup);
    printf("Virtual Memory Size: %lu\n", vmSize);
    printf("Executable Path: %s\n", realPath);

    fclose(statFile);
    fclose(statmFile);

    return 0;
}
