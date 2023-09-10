#include "../../headers.h"

struct ProcessDetails backgroundProcesses[MAX_BACKGROUND_PROCESSES + 1];
int numBackgroundProcesses = 0;

struct ProcessDetails * get_background_processes() {   
    return backgroundProcesses;
}

int get_num_bg_processes() {
    return numBackgroundProcesses;
}

void update_background_status() {
    int status;
    
    // Dont delete anything here
    // This will be called from the main loop
    // by the signal handler
    // just update the completed and the status
    // without printing anything

    for (int i = 0; i < numBackgroundProcesses; i++) {
        // printf("Checking %d\n", i);
        // printf("The pid is %d\n", backgroundProcesses[i].pid);
        // printf("The completed is %d\n", backgroundProcesses[i].completed);
        
        pid_t pid = waitpid(backgroundProcesses[i].pid, &status, WNOHANG | WUNTRACED);
        if (pid == -1) {
            // printf("Error in waitpid\n");
        } else if(pid ==0){
            continue;
        } 
        else{
            backgroundProcesses[i].completed = 1;
            backgroundProcesses[i].normallyExited = (
                WIFEXITED(status) && !WEXITSTATUS(status)
            );
            // printf("backgroundProcesses[i].normallyExited: %d\n",backgroundProcesses[i].normallyExited);
        }
    }
}

void check_background_processes() {
    int status;
    int completed_processes = 0;

    for (int i = 0; i < numBackgroundProcesses; i++) {
        // printf("Checking %d\n", i);
        // printf("The pid is %d\n", backgroundProcesses[i].pid);
        // printf("The completed is %d\n", backgroundProcesses[i].completed);
        // printf("The NORMALLY EXITED is %d\n", backgroundProcesses[i].normallyExited);

        if (backgroundProcesses[i].completed) {
            // pid_t pid = waitpid(backgroundProcesses[i].pid, &status, WNOHANG);
            // printf("The pid is %d\n", pid);
            // if (pid == -1) {
            //     printf("Error in waitpid\n");
            // } else if (pid == backgroundProcesses[i].pid) {
            //     backgroundProcesses[i].completed = 1;
                // printf("[%d] Done\t%s\n", i + 1, backgroundProcesses[i].commandName);
                completed_processes++;

                if (backgroundProcesses[i].normallyExited) {
                    printf("%s exited normally (%d)\n", backgroundProcesses[i].commandName, backgroundProcesses[i].pid);
                } else {
                    printf("%s exited abnormally (%d)\n", backgroundProcesses[i].commandName, backgroundProcesses[i].pid);
                }
            // }
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
        backgroundProcesses[numBackgroundProcesses].normallyExited = 0;
        backgroundProcesses[numBackgroundProcesses].commandName = strdup(command);
        numBackgroundProcesses++;
    } else {
        printf("Reached maximum number of background processes.\n");
    }
}
