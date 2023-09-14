#ifndef __SEEK_H
#define __SEEK_H

#include "../headers.h"
#include "../utils/customStructs.h"

#include "../utils/pathHandler.h"
#include "../utils/flagHandler.h"

int seek(char ** command_args, char starting_directory[], char ** previous_directory);

#endif