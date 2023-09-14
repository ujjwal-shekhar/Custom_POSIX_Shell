#ifndef CUSTOM_STRUCT_H
#define CUSTOM_STRUCT_H

#include <sys/types.h>

struct CommandArgs {
    char * command_details;
    char ** command_args;
    int num_args;
    int input_fd;
    int output_fd;
    int outputAppend; // 1 if  ">>", 0 if ">"
} commandArgs;

struct Command {
    char *command_details;
    int printProcId;
};

struct CommandList {
    struct Command *commands;
    int num_commands;
};

struct ProcessDetails {
    pid_t pid;
    int completed; // bool
    int normallyExited; // -1 : not exited, 0 : abnormally, 1 : normally
    char *commandName;
    char *status; // Running / Stopped / Finished
};

struct FlagInfo {
    int isFlag;
    char * flags; // Basically removes the "-" and returns the string
} flagInfo;

struct PathInfo {
    int isPath;
    char * path;
} pathInfo;

struct PipedCommandDetails {
    char * command_details;
    struct CommandArgs *ca;
    int num_piped_commands;
} pipedCommandDetails;

#endif
