#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <ctype.h>

#include "prompt.h"

#include "warpHandler/warp.h"

#include "peekHandler/peek.h"

#include "utils/pathHandler.h"
#include "utils/flagHandler.h"
#include "utils/errorHandler.h"
#include "utils/inputHandlers/commandHandler.h"
#include "utils/inputHandlers/commandArgsHandler.h"

#endif