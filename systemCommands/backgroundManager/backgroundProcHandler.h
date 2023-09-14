#ifndef BACKGROUND_MANAGER_H
#define BACKGROUND_MANAGER_H

#include "../../headers.h"

#include "../../utils/customStructs.h"

#include "../../utils/pathHandler.h"
#include "../../utils/flagHandler.h"


// #include "../../utils/inputHandlers/commandArgsHandler.h"
// #include "../../utils/customStructs.h"

// #define MAX_BACKGROUND_PROCESSES 10


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


void update_background_status();
void add_background_process(pid_t pid, const char *command);
void check_background_processes();
int get_num_bg_processes();
struct ProcessDetails * get_background_processes();
int execute_background_process(pid_t shell_pid, struct CommandArgs ca, char* commandName, int num_args, char* command_details);
// int numBackgroundProcesses = 0;

#endif // BACKGROUND_MANAGER_H
