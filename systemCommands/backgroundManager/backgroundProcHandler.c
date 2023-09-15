#include "backgroundProcHandler.h"

// #include "utils/errorHandler.h"
// #include "utils/inputHandlers/commandExecutor.h"
// #include "utils/inputHandlers/commandHandler.h"
// #include "utils/inputHandlers/commandArgsHandler.h"

void update_background_status(struct ProcessList *pl) {
    int status;
    // Dont delete anything here
    // This will be called from the main loop
    // by the signal handler
    // just update the completed and the status
    // without printing anything
    for (int i = 0; i < pl->numProcesses; i++) {  
        pid_t pid = waitpid(pl->backgroundProcesses[i].pid, &status, WNOHANG | WUNTRACED);
        if (pid == -1) {
            // printf("Error in waitpid\n");
        } else if(pid == 0){
            // printf("Process %d is running\n", pl->backgroundProcesses[i].pid);
            // printf("Process %d is running\n", pl->backgroundProcesses[i].pid);
            pl->backgroundProcesses[i].status = strdup("running");
            // continue;
        } 
        else{
            // pl->backgroundProcesses[i].completed = 1;
            pl->backgroundProcesses[i].normallyExited = (
                WIFEXITED(status) && !WEXITSTATUS(status)
            );

            // Set the status of backgroundProcesses
            // Running / Stopped / Exited
            if (WIFEXITED(status)) {
                // printf("Exited\n");
                pl->backgroundProcesses[i].status = strdup("finished");
            } else if (WIFSTOPPED(status)) {
                // printf("Stopped\n");
                pl->backgroundProcesses[i].status = strdup("stopped");
            } else if (WIFSIGNALED(status)) {
                // printf("Finished\n");
                pl->backgroundProcesses[i].status = strdup("finished");
            }
        }
    }
}

void check_background_processes(struct ProcessList *pl) {
    int status;
    int completed_processes = 0;

    for (int i = 0; i < pl->numProcesses; i++) {
        completed_processes++;
        if (strcmp(pl->backgroundProcesses[i].status, "finished") == 0) {
            if (pl->backgroundProcesses[i].normallyExited) {
                printf("%s exited normally (%d)\n", pl->backgroundProcesses[i].commandName, pl->backgroundProcesses[i].pid);
            } else {
                printf("%s exited abnormally (%d)\n", pl->backgroundProcesses[i].commandName, pl->backgroundProcesses[i].pid);
            }
        }
}

    // Clean up completed processes from the list
    int new_index = 0;  
    for (int i = 0; i < pl->numProcesses; i++) {
        if (strcmp(pl->backgroundProcesses[i].status, "finished")) {
            pl->backgroundProcesses[new_index] = pl->backgroundProcesses[i];
            new_index++;
        }
    }

    pl->numProcesses = new_index;
}

void add_background_process(pid_t pid, const char *command, struct ProcessList *pl) {
    if (pl->numProcesses < MAX_BACKGROUND_PROCESSES) {
        (pl->backgroundProcesses[pl->numProcesses]).pid = pid;
        (pl->backgroundProcesses[pl->numProcesses]).completed = 0;
        (pl->backgroundProcesses[pl->numProcesses]).normallyExited = 0;
        // (pl->backgroundProcesses[pl->numProcesses]).commandName = strdup(command);
        strcpy((pl->backgroundProcesses[pl->numProcesses]).commandName, command);
        strcpy((pl->backgroundProcesses[pl->numProcesses]).status, "running");
        pl->numProcesses++;
    } else {
        printf("Reached maximum number of background processes.\n");
    }
}

int execute_background_process(pid_t shell_pid, struct CommandArgs ca, char * commandName, int num_args, char* command_details, struct ProcessList *pl) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        // Reset Ctrl + C
        signal(SIGINT, SIG_DFL);

        // Reset Ctrl + Z
        signal(SIGTSTP, SIG_DFL);

        // Set the process group leader
        setpgid(0, 0); 

        // Open /dev/null for reading and writing
        int null_fd = open("/dev/null", O_RDWR);

        // Check if the file descriptor is valid
        if (null_fd == -1) {
            // errorHandler("\033[31mFailed to open /dev/null\033[0m", &errorString);
            perror("\033[31mFailed to open /dev/null\033[0m");
            exit(EXIT_FAILURE);
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
        execvp(commandName, ca.command_args);

        printf("\033[31mERROR: '%s' is not a valid command\033[0m\n", commandName);

        exit(EXIT_FAILURE);
    } else { // Parent process block
        add_background_process(pid, command_details, pl);
        printf("[%d]\n", pid); // Print the PID of the background process
    }

    return 0;
}