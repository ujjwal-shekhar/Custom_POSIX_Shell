#ifndef __FLAG_H
#define __FLAG_H

#include <stdio.h>
#include <unistd.h> //
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> //
#include <dirent.h>   //
#include <sys/stat.h> //
#include <sys/wait.h> //
#include <pwd.h> //
#include <grp.h> //
#include <time.h>
#include <ctype.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "customStructs.h"

#define MAX_BACKGROUND_PROCESSES 10

#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define BLUE_COLOR "\033[34m"
#define RESET_COLOR "\033[0m"

#define MEMORY_ALLOC_ERROR "Failed to allocate memory\n"

// #define PAST_EVENTS_PATH "./pasteventsHandler/pastevents.log"
#define PAST_EVENTS_PATH "/home/ujjwal-shake-her/M23_Code_Thingies/OSN/A1/pasteventsHandler/pastevents.log"

int flagHandler(char *command_arg, char starting_directory[], const char *prefix, struct FlagInfo *fi);

#endif