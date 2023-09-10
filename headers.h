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

#include "prompt.h"

#include "warpHandler/warp.h"

#include "peekHandler/peek.h"
#include "seekHandler/seek.h"

#include "procloreHandler/proclore.h"
#include "pingHandler/ping.h"
#include "activitiesHandler/activities.h"

#include "pasteventsHandler/pastevents.h"

#include "systemCommands/backgroundManager/backgroundProcHandler.h"

#include "utils/pathHandler.h"
#include "utils/flagHandler.h"
#include "utils/errorHandler.h"
#include "utils/inputHandlers/commandHandler.h"
#include "utils/inputHandlers/commandArgsHandler.h"
#include "utils/inputHandlers/commandExecutor.h"

#include "redirPipeHandler/pipeHandler.h"
#include "redirPipeHandler/redirectionHandler.h"

struct CommandList {
    struct Command * commands;
    int num_commands;
} commandList;

struct ProcessDetails {
    pid_t pid;
    int completed; // bool
    int normallyExited ; // -1 : not exited, 0 : abnormally, 1 : normally
    char *commandName;
};

#define MAX_BACKGROUND_PROCESSES 10

// struct ProcessDetails backgroundProcesses[MAX_BACKGROUND_PROCESSES + 1];

#endif