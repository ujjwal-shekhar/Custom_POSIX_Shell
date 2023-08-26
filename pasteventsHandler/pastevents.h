#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H

int addEventToHistory(struct CommandList cl, int dontAddToHistory);

int showHistory(char **errorString);

int clearHistory(char **errorString);

int getIndexInHistory(char **errorString, int fetchIndex, char ** command_details);

#endif