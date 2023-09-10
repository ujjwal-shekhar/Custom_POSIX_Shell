// #include "../headers.h"

// /*
// This specification requires you to print a list of all the processes currently running that were spawned by your shell in lexicographic order. This list should contain the following information about all processes:

// Command Name
// pid
// state: running or stopped
// */


// // Function to check if a string is numeric
// int is_numeric(const char *str) {
//     while (*str) {
//         if (!isdigit(*str))
//             return 0;
//         str++;
//     }
//     return 1;
// }

// int activities(char **command_args) {
//     // Open the /proc directory
//     DIR *proc_dir = opendir("/proc");
//     if (proc_dir == NULL) {
//         perror("Failed to open /proc directory");
//         return 1;
//     }

//     // Define buffer sizes for paths
//     size_t cmdline_path_size = sizeof("/proc/") + sizeof("/cmdline") + 64; // Adjust the size accordingly
//     size_t status_path_size = sizeof("/proc/") + sizeof("/status") + 64;   // Adjust the size accordingly

//     // Iterate through each entry in the /proc directory
//     struct dirent *entry;
//     while ((entry = readdir(proc_dir)) != NULL) {
//         // Check if the entry is a directory and its name consists of digits (process ID)
//         if (entry->d_type == DT_DIR && is_numeric(entry->d_name)) {
//             // Construct the path to the cmdline file for this process
//             char cmdline_path[cmdline_path_size];
//             snprintf(cmdline_path, cmdline_path_size, "/proc/%s/cmdline", entry->d_name);

//             // Open the cmdline file
//             FILE *cmdline_file = fopen(cmdline_path, "r");
//             if (cmdline_file == NULL) {
//                 perror("Failed to open cmdline file");
//                 continue;
//             }

//             // Read the command name from the cmdline file
//             char command_name[256];  // Adjust buffer size accordingly
//             if (fscanf(cmdline_file, "%255s", command_name) == 1) {
//                 // Construct the path to the status file for this process
//                 char status_path[status_path_size];
//                 snprintf(status_path, status_path_size, "/proc/%s/status", entry->d_name);

//                 // Open the status file
//                 FILE *status_file = fopen(status_path, "r");
//                 if (status_file == NULL) {
//                     perror("Failed to open status file");
//                 } else {
//                     // Read and print the process state
//                     char line[256];  // Adjust buffer size accordingly
//                     while (fgets(line, sizeof(line), status_file)) {
//                         // Check fi the process was spawned by this process

//                         if (strncmp(line, "State:", 6) == 0) {
//                             printf("Command Name: %s\n", command_name);
//                             printf("pid: %s\n", entry->d_name);
//                             printf("state: %s", line + 7);
//                             break;
//                         }
//                     }
//                     fclose(status_file);
//                 }
//             }

//             fclose(cmdline_file);
//         }
//     }

//     closedir(proc_dir);
//     return 0;
// }

#include "../headers.h"

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

int activities(char ** command_args) {
    int numbg = get_num_bg_processes();
    struct ProcessDetails * bgprocs = get_background_processes();

    for (int i=0; i<numbg; i++) {
        printf("%d : ", bgprocs[i].pid);
        printf(" %s - ", bgprocs[i].commandName);
        printf("%s\n", ((bgprocs[i].completed) ? ("Stopped") : ("Running")));
    }

    return 0;
}
