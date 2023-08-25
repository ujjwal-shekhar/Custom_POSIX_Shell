#include "../../headers.h"

struct ProcessDetails backgroundProcesses[MAX_BACKGROUND_PROCESSES + 1];
int numBackgroundProcesses = 0;

void check_background_processes() {
    int status;
    int completed_processes = 0;

    for (int i = 0; i < numBackgroundProcesses; i++) {
        if (!backgroundProcesses[i].completed) {
            pid_t pid = waitpid(backgroundProcesses[i].pid, &status, WNOHANG);
            if (pid == -1) {
                printf("Error in waitpid\n");
            } else if (pid == backgroundProcesses[i].pid) {
                backgroundProcesses[i].completed = 1;
                // printf("[%d] Done\t%s\n", i + 1, backgroundProcesses[i].commandName);
                completed_processes++;

                if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                    printf("%s exited normally (%d)\n", backgroundProcesses[i].commandName, backgroundProcesses[i].pid);
                } else if (WIFEXITED(status) && WEXITSTATUS(status)) {
                    if (WEXITSTATUS(status) == ((1 << 7) - 1)) {
                        printf("execv failed\n");
                    } else {
                        printf("%s exited normally (%d)\n", backgroundProcesses[i].commandName, backgroundProcesses[i].pid);
                    }
                } else {
                    printf("%s exited abnormally (%d)\n", backgroundProcesses[i].commandName, backgroundProcesses[i].pid);
                }
            }
        }
    }

    // Clean up completed processes from the list
    int new_index = 0;  
    for (int i = 0; i < numBackgroundProcesses; i++) {
        if (!backgroundProcesses[i].completed) {
            backgroundProcesses[new_index] = backgroundProcesses[i];
            new_index++;
        }
    }
    numBackgroundProcesses -= completed_processes;
}

void add_background_process(pid_t pid, const char *command) {
    if (numBackgroundProcesses < MAX_BACKGROUND_PROCESSES) {
        backgroundProcesses[numBackgroundProcesses].pid = pid;
        backgroundProcesses[numBackgroundProcesses].completed = 0;
        backgroundProcesses[numBackgroundProcesses].commandName = strdup(command);
        numBackgroundProcesses++;
    } else {
        printf("Reached maximum number of background processes.\n");
    }
}
