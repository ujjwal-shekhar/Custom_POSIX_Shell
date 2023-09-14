#ifndef __FLAG_H
#define __FLAG_H

struct FlagInfo {
    int isFlag;
    char * flags; // Basically removes the "-" and returns the string
} flagInfo;

int flagHandler(char *command_arg, char starting_directory[], const char *prefix, struct FlagInfo *fi);

#endif