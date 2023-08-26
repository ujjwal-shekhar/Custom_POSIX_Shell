#ifndef HEADERS_H_
#define HEADERS_H_

struct CommandList {
    struct Command * commands;
    int num_commands;
} commandList;

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

#include "pasteventsHandler/pastevents.h"

#include "systemCommands/backgroundManager/backgroundProcHandler.h"

#include "utils/pathHandler.h"
#include "utils/flagHandler.h"
#include "utils/errorHandler.h"
#include "utils/inputHandlers/commandHandler.h"
#include "utils/inputHandlers/commandArgsHandler.h"
#include "utils/inputHandlers/commandExecutor.h"

#endif