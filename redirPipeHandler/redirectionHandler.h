#ifndef __REDIR_H
#define __REDIR_H

#include "../headers.h"
#include "../utils/customStructs.h"
#include "../utils/inputHandlers/commandExecutor.h"
#include "../utils/inputHandlers/commandHandler.h"
#include "../utils/inputHandlers/commandArgsHandler.h"

int replaceInputRedirect(struct CommandArgs* ca, char starting_directory[], char ** previous_directory);
int replaceOutputRedirect(struct CommandArgs* ca, char starting_directory[], char ** previous_directory);
int replaceOutputRedirectAppend(struct CommandArgs* ca, char starting_directory[], char ** previous_directory);

#endif