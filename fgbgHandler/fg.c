#include "fg.h"

int remove_pid_from_bg_list(pid_t target_pid, struct ProcessList *pl) {
    int found = 0;
    for (int i = 0; i < pl->numProcesses; i++) {
        if (pl->backgroundProcesses[i].pid == target_pid) {
            found = 1;
            for (int j = i; j < pl->numProcesses - 1; j++) {
                pl->backgroundProcesses[j] = pl->backgroundProcesses[j + 1];
            }
            pl->numProcesses--;
            break;
        }
    }

    return !found;
}

// https://www.gnu.org/software/libc/manual/html_node/Foreground-and-Background.html
int send_process_to_foreground(char ** command_args, pid_t shell_pid, struct ProcessList *pl) {
    pid_t target_pid = atoi(command_args[1]);
    pid_t shell_pgid = getpgid(shell_pid);
    pid_t target_pgid = getpgid(target_pid);
    
    // Check if the target_pid is valid
    // https://stackoverflow.com/questions/9152979/check-if-process-exists-given-its-pid
    if (kill(target_pid, 0)) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, "ERROR : Process with PID [%d] doesn't exist\n", target_pid);
        fprintf(stderr, RESET_COLOR);
        return 1;
    }

    // Set the process group ID to shell's pgid
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp (0, target_pgid);

    // Wait for the job to report
    int status;
    waitpid(target_pid, &status, WUNTRACED);

    // Put the shell in the foreground again
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(0, shell_pgid);

    // Remove this pid from bgprocslist
    remove_pid_from_bg_list(target_pid, pl);

    return 0;
}
