#ifndef __PATH_H
#define __PATH_H

struct PathInfo {
    int isPath;
    char * path;
} pathInfo;

struct PathInfo pathHandler(char * command_arg, char starting_directory[], char ** previous_directory);

#endif