#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H

int addEventToHistory(char * command_details, char * commandName, char **errorString, int dontAddToHistory);

int showHistory(char **errorString);

int clearHistory(char **errorString);

int getIndexInHistory(char **errorString, int fetchIndex);

#endif