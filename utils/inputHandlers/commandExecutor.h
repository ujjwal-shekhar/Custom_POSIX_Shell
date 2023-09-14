#ifndef __COMMAND_EXECUTOR_H
#define __COMMAND_EXECUTOR_H

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

#include "../customStructs.h"

#define MAX_BACKGROUND_PROCESSES 10

#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define BLUE_COLOR "\033[34m"
#define RESET_COLOR "\033[0m"

#define MEMORY_ALLOC_ERROR "Failed to allocate memory\n"

// #define PAST_EVENTS_PATH "./pasteventsHandler/pastevents.log"
#define PAST_EVENTS_PATH "/home/ujjwal-shake-her/M23_Code_Thingies/OSN/A1/pasteventsHandler/pastevents.log"

#include "../../warpHandler/warp.h"

#include "../../peekHandler/peek.h"
#include "../../seekHandler/seek.h"

#include "../../procloreHandler/proclore.h"
#include "../../pingHandler/ping.h"
#include "../../activitiesHandler/activities.h"

#include "../../pasteventsHandler/pastevents.h"

#include "../../systemCommands/backgroundManager/backgroundProcHandler.h"
#include "../../systemCommands/foregroundManager/foregroundProcHandler.h"


#include "../../iManHandler/iManHandler.h"
#include "../../neonateHandler/neonate.h"

#include "../../fgbgHandler/fg.h"
#include "../../fgbgHandler/bg.h"

int executeCommand(char* commandName, int num_args, char *command_args[4096], char starting_directory[], char ** previous_directory, char ** prevCommDetails);
int checkUserCommand(char * commandName);

#endif
