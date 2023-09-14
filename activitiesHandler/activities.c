#include "activities.h"

/*
This specification requires you to print a list of all the processes currently running that were spawned by your shell in lexicographic order. This list should contain the following information about all processes:

Command Name
pid
state: running or stopped

   pid_t pid;
    int completed; // bool
    int normallyExited ; // -1 : not exited, 0 : abnormally, 1 : normally
    char *commandName;
*/

int activities(char ** command_args, struct ProcessList *pl) {
    int numbg = pl->numProcesses;
    struct ProcessDetails * bgprocs = pl->backgroundProcesses;

    for (int i=0; i<numbg; i++) {
        printf("%d : ", bgprocs[i].pid);
        printf(" %s - ", bgprocs[i].commandName);
        printf("%s\n", bgprocs[i].status);
    }

    return 0;
}
