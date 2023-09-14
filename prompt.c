// #define _POSIX_SOURCE
// #include "headers.h"

#include "prompt.h"

void prompt(char starting_directory[], char prevCommandName[], double timeTaken) {
    // Retrieve the host name
    char hostname[256];

    /*
    USE OF ChatGPT
    The following if-statement was suggested by ChatGPT-3.5-Turbo
    the prompt given was : "get computer hostname C"
    */

    if (gethostname(hostname, sizeof(hostname)) != 0) {
        perror("\033[31mgethostname failed\033[0m");
        exit(EXIT_FAILURE);
    }

    // Get the user name
    struct passwd *pass = getpwuid(getuid());
    char *username = pass->pw_name; /*AUTO COMPLETED BY COPILOT*/

    if (username == NULL) {
        perror("\033[31mCouldn't fetch user name failed\033[0m");
        exit(EXIT_FAILURE);
    }

    // Check if the currect working directory is inside or outside 
    // the starting directory
    char current_directory[4096];
    getcwd(current_directory, 4096);

    // If the current directory is inside the starting directory
    // replace it with tilde sign followed by the relative path
    // else use the absolute path
    if (strstr(current_directory, starting_directory) != NULL) {
        char *relative_path = strstr(current_directory, starting_directory);
        printf("<\033[96m%s\033[0m@%s:~%s", username, hostname, relative_path + strlen(starting_directory));
    } else {
        printf("<\033[96m%s\033[0m@%s:%s", username, hostname, current_directory);
    }

    // If the time taken is greater than 2 (rounded down to integer)
    // then print the prevCommandName : time taken
    if (timeTaken > 2) {
        printf(" \033[92m%s : %.0lfs\033[0m", prevCommandName, timeTaken);
    }

    printf("> ");

    // Bibliography
    // https://www.bing.com/search?q=get+username+c&cvid=9705e13fb2614d8db0993f94136f9089&aqs=edge.0.69i59l3j69i57j0l5j69i11004.1604j0j9&FORM=ANAB01&PC=HCTS
    // https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
    // https://stackoverflow.com/questions/4785126/getlogin-c-function-returns-null-and-error-no-such-file-or-directory
}
