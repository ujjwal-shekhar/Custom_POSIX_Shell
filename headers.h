#ifndef HEADERS_H_
#define HEADERS_H_

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
#include <termios.h>
#include <errno.h>

// #include "utils/customStructs.h"

// #include "utils/pathHandler.h"
// #include "utils/flagHandler.h"
// #include "utils/errorHandler.h"
// #include "utils/inputHandlers/commandExecutor.h"
// #include "utils/inputHandlers/commandHandler.h"
// #include "utils/inputHandlers/commandArgsHandler.h"

// #include "warpHandler/warp.h"
// #include "peekHandler/peek.h"
// #include "seekHandler/seek.h"

// #include "procloreHandler/proclore.h"
// #include "pingHandler/ping.h"
// #include "activitiesHandler/activities.h"

// #include "pasteventsHandler/pastevents.h"

// #include "systemCommands/backgroundManager/backgroundProcHandler.h"
// #include "systemCommands/foregroundManager/foregroundProcHandler.h"

// #include "redirPipeHandler/pipeHandler.h"
// #include "redirPipeHandler/redirectionHandler.h"

// #include "iManHandler/iManHandler.h"


// struct Command {
//     char * command_details;
//     int printProcId;
// } command;

// struct CommandList {
//     struct Command * commands;
//     int num_commands;
// } commandList;

// struct ProcessDetails {
//     pid_t pid;
//     int completed; // bool
//     int normallyExited ; // -1 : not exited, 0 : abnormally, 1 : normally
//     char *commandName;
// };

#define MAX_BACKGROUND_PROCESSES 10

#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define BLUE_COLOR "\033[34m"
#define RESET_COLOR "\033[0m"

#define MEMORY_ALLOC_ERROR "Failed to allocate memory\n"

// #define PAST_EVENTS_PATH "./pasteventsHandler/pastevents.log"
#define PAST_EVENTS_PATH "/home/ujjwal-shake-her/M23_Code_Thingies/OSN/A1/pasteventsHandler/pastevents.log"
// struct ProcessDetails backgroundProcesses[MAX_BACKGROUND_PROCESSES + 1];


#endif