#ifndef __FLAG_H
#define __FLAG_H

struct FlagInfo {
    int isFlag;
    char * flags; // Basically removes the "-" and returns the string
} flagInfo;

struct FlagInfo flagHandler(char * command_args, char starting_directory[], const char * prefix);

#endif