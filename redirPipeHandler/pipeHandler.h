#ifndef __PIPE_H
#define __PIPE_H

#include "../headers.h"
#include "redirectionHandler.h"

// #include "../utils/pathHandler.h"
// #include "../utils/flagHandler.h"

int splitByPipe(char *command_details, char **command_details_split);
int pipe_split(char *command_details, char ** command_args, int isBackground, char starting_directory[], char ** previous_directory, struct PipedCommandDetails * pcd);

#endif