#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H

// int addEventToHistory(struct CommandList cl, int dontAddToHistory);
int addEventToHistory(char * input);

int showHistory(char **errorString);

int clearHistory(char **errorString);

int getIndexInHistory(char **errorString, int fetchIndex, char ** command_details);

int replacePastEventCommands(char *input);

#endif