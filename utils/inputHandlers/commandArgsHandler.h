#ifndef COMMAND_ARGS_HANDLER_H
#define COMMAND_ARGS_HANDLER_H

struct CommandArgs {
    char * command_details;
    char ** command_args;
    int num_args;
} commandArgs;

struct CommandArgs parseCommandArgs(char *command_details, char **command_args);

#endif
