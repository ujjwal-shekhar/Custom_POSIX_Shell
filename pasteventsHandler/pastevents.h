#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H

#include "../headers.h"

int addEventToHistory(char * input);

int showHistory();

int clearHistory();

int getIndexInHistory(int fetchIndex, char ** command_details);

int replacePastEventCommands(char *input);

#endif