#ifndef __COMMAND_EXECUTOR_H
#define __COMMAND_EXECUTOR_H

int executeCommand(char* commandName, int num_args, char *command_args[4096], char ** errorString, char starting_directory[], char ** previous_directory);

#endif
