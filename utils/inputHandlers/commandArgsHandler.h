#ifndef __COMMAND_ARGS_HANDLER_H
#define __COMMAND_ARGS_HANDLER_H

struct CommandArgs {
    char * command_details;
    char ** command_args;
    int num_args;
    int input_fd;
    int output_fd;
    int outputAppend; // 1 if  ">>", 0 if ">"
} commandArgs;

struct CommandArgs parseCommandArgs(char *command_details, char **command_args);

#endif
