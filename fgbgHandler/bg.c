#include "bg.h"

int start_process_in_background(char ** command_args, pid_t shell_pid) {
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

    // Send SIGCONT to the process
    if (kill(target_pid, SIGCONT) == -1) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, "ERROR : Failed to send SIGCONT to process with PID [%d]\n", target_pid);
        fprintf(stderr, RESET_COLOR);
    }

    // 
}