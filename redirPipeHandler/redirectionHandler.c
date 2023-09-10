#include "../headers.h"

// Use of Github Copilot
/*
I have mostly written the first function
and then Copilot was able to auto complete the
other two
*/

int replaceInputRedirect(struct CommandArgs ca, char ** errorString, char starting_directory[], char ** previous_directory) {
    // Find the position of the input redirect
    int inputRedirectPos = -1;
    for (int i = 0; i < ca.num_args; i++) {
        if (strcmp(ca.command_args[i], "<") == 0) {
            inputRedirectPos = i;
            break;
        }
    }

    // Check if the token after that exists
    if (inputRedirectPos == -1 || inputRedirectPos == ca.num_args - 1) {
        return 1;
    }

    // Check if the the token after that is a path or not
    struct PathInfo pi = pathHandler(ca.command_args[inputRedirectPos + 1], errorString, starting_directory, previous_directory);

    if (!pi.isPath) {
        return 1;
    }

    // Open the file
    int fd = open(pi.path, O_RDONLY);

    // Check if the file descriptor is valid
    if (fd == -1) {
        errorHandler("Opening file failed", errorString);
        return 1;
    }

    // Replace the input fd of the command with the file descriptor
    ca.input_fd = fd;

    return 0;
}

int replaceOutputRedirect(struct CommandArgs ca, char ** errorString, char starting_directory[], char ** previous_directory) {
    // Find the position of the output redirect
    int outputRedirectPos = -1;
    for (int i = 0; i < ca.num_args; i++) {
        if (strcmp(ca.command_args[i], ">") == 0) {
            outputRedirectPos = i;
            break;
        }
    }

    // Check if the token after that exists
    if (outputRedirectPos == -1 || outputRedirectPos == ca.num_args - 1) {
        return 1;
    }

    // Check if the the token after that is a path or not
    struct PathInfo pi = pathHandler(ca.command_args[outputRedirectPos + 1], errorString, starting_directory, previous_directory);

    if (!pi.isPath) {
        return 1;
    }

    // Open the file
    int fd = open(pi.path, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    // Check if the file descriptor is valid
    if (fd == -1) {
        errorHandler("Opening file failed", errorString);
        return 1;
    }

    // Replace the output fd of the command with the file descriptor
    ca.output_fd = fd;

    return 0;
}

int replaceOutputRedirectAppend(struct CommandArgs ca, char ** errorString, char starting_directory[], char ** previous_directory) {
    // Find the position of the output redirect
    int outputRedirectPos = -1;
    for (int i = 0; i < ca.num_args; i++) {
        if (strcmp(ca.command_args[i], ">>") == 0) {
            outputRedirectPos = i;
            break;
        }
    }

    // Check if the token after that exists
    if (outputRedirectPos == -1 || outputRedirectPos == ca.num_args - 1) {
        return 1;
    }

    // Check if the the token after that is a path or not
    struct PathInfo pi = pathHandler(ca.command_args[outputRedirectPos + 1], errorString, starting_directory, previous_directory);

    if (!pi.isPath) {
        return 1;
    }

    // Open the file
    int fd = open(pi.path, O_WRONLY | O_CREAT | O_APPEND, 0644);

    // Check if the file descriptor is valid
    if (fd == -1) {
        errorHandler("Opening file failed", errorString);
        return 1;
    }

    // Replace the output fd of the command with the file descriptor
    ca.output_fd = fd;

    return 0;
}