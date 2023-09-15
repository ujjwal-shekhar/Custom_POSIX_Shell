#include "foregroundProcHandler.h"

int execute_foreground_process(pid_t shell_pid, struct PipedCommandDetails pcd, char* commandName, int num_args, char* command_details, char starting_directory[], char ** previous_directory, char ** prevCommDetails, struct ProcessList *pl) {
    int pipe_fd[pcd.num_piped_commands-1][2];
    for (int job=0; job<pcd.num_piped_commands; job++) {
        if (job < pcd.num_piped_commands - 1)
            pipe(pipe_fd[job]);

        struct CommandArgs ca = pcd.ca[job];
        ca.command_args[ca.num_args] = NULL;
        char* commandName = ca.command_args[0];

        // if (checkUserCommand(commandName)) {
        //     perror("I haven't done this lol");
        //     return 1;

        // } else {
            pid_t pid = fork();

            if (pid < 0) {
                perror("fork");
            } else if (pid == 0) {
                // // Reset Ctrl + C
                // signal(SIGINT, SIG_DFL);

                // // Reset Ctrl + Z
                // signal(SIGSTOP, SIG_DFL);


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
                if (checkUserCommand(commandName)) {
                    int erroneousFlag = executeCommand(commandName, num_args, ca.command_args, starting_directory, previous_directory, prevCommDetails, pl);
                } else {
                    execvp(commandName, ca.command_args);
                }

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
        // }

    }

    return 0;
}