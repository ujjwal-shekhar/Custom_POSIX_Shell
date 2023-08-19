#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

struct Command {
    char * command_details;
    int printProcId;
} command;

struct CommandList {
    struct Command * commands;
    int num_commands;
} commandList;

struct CommandList executeCommands(char *input);

#endif
