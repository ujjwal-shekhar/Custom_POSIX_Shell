#include "ping.h"

/*
ping command is used to send signals to processes. 
Take the pid of a process and send a signal to it 
which corresponds to the signal number (which is 
provided as an argument). Print error “No such process found”, 
if process with given pid does not exist. You should take signal 
number’s modulo with 32 before checking which signal it belongs 
to (assuming x86/ARM machine). Check man page for signal for 
an exhaustive list of all signals present.
*/

// Autocompleted by Github Copilot
int ping(char ** command_args) {
    pid_t pid = atoi(command_args[1]);
    int signalNumber = atoi(command_args[2]);
    // int signalNumber = action;
    int signalNumberModulo = signalNumber % 32;

    if (kill(pid, signalNumberModulo) == -1) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, "ERROR : No such process found with PID [%d]\n", pid);
        fprintf(stderr, RESET_COLOR);
        return 1;
    } else {
        printf("%sSignal %d sent to process with pid %d%s\n", GREEN_COLOR, signalNumber, pid, RESET_COLOR);
    }

    return 0;
}