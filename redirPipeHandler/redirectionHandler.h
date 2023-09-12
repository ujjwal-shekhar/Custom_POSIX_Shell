#ifndef __REDIR_H
#define __REDIR_H

int replaceInputRedirect(struct CommandArgs* ca, char ** errorString, char starting_directory[], char ** previous_directory);
int replaceOutputRedirect(struct CommandArgs* ca, char ** errorString, char starting_directory[], char ** previous_directory);
int replaceOutputRedirectAppend(struct CommandArgs* ca, char ** errorString, char starting_directory[], char ** previous_directory);

#endif