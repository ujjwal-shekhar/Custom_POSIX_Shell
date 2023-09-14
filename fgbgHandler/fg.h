#ifndef __FG_H
#define __FG_H

#include "../headers.h"
#include "../utils/customStructs.h"

int send_process_to_foreground(char ** command_args, pid_t shell_pid, struct ProcessList *pl);

#endif