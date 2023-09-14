#ifndef FOREGROUND_MANAGER_H
#define FOREGROUND_MANAGER_H

#include "../../headers.h"
#include "../../utils/inputHandlers/commandArgsHandler.h"
#include "../../redirPipeHandler/pipeHandler.h"

int execute_foreground_process(pid_t shell_pid, struct PipedCommandDetails pcd, char* commandName, int num_args, char* command_details);

#endif // BACKGROUND_MANAGER_H
