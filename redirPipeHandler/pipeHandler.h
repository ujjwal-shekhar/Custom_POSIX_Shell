#ifndef __PIPE_H
#define __PIPE_H

struct PipedCommandDetails {
    char * command_details;
    struct CommandArgs *ca;
    int num_piped_commands;
} pipedCommandDetails;

int splitByPipe(char *command_details, char **command_details_split);
int pipe_split(char *command_details, char ** command_args, int isBackground, char ** errorString, char starting_directory[], char ** previous_directory, struct PipedCommandDetails * pcd);

#endif