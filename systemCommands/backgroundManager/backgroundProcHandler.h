#ifndef BACKGROUND_MANAGER_H
#define BACKGROUND_MANAGER_H

#define MAX_BACKGROUND_PROCESSES 10


// Usage of ChatGPT-3.5 Turbo
// Prompt used : 
/*
How to make background processes using this fork exec model

I was thinking of maintaining a list of structs for this, where each element would  be a struct processDetails {
    pid_t PID;
    int completed; // bool
    char * commandName;
}
*/

struct ProcessDetails {
    pid_t pid;
    int completed; // bool
    char *commandName;
};

void add_background_process(pid_t pid, const char *command);
void check_background_processes();

#endif // BACKGROUND_MANAGER_H
