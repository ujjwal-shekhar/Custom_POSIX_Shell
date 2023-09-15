#ifndef FOREGROUND_MANAGER_H
#define FOREGROUND_MANAGER_H

#include "../../headers.h"
#include "../../utils/inputHandlers/commandArgsHandler.h"
#include "../../redirPipeHandler/pipeHandler.h"

int execute_foreground_process(pid_t shell_pid, struct PipedCommandDetails pcd, char* commandName, int num_args, char* command_details, char starting_directory[], char ** previous_directory, char ** prevCommDetails, struct ProcessList *pl);

#endif // BACKGROUND_MANAGER_H
