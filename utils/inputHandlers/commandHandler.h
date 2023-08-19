#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

struct Command {
    char * command_details;
    int printProcId;
} command;

struct Command executeCommands(char *input, char *starting_directory);

#endif
