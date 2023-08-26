#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define STATUS_BUF_SIZE 128

void print_process_status(char *status) {
    char process_status;
    
    if (status[0] == 'R' || status[1] == '+') {
        process_status = 'R'; // Running
    } else if (status[0] == 'S' || status[1] == '+') {
        process_status = 'S'; // Sleeping
    } else if (status[0] == 'Z') {
        process_status = 'Z'; // Zombie
    } else {
        process_status = '?'; // Unknown
    }

    printf("Process Status: %c\n", process_status);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PID>\n", argv[0]);
        return 1;
    }

    int pid = atoi(argv[1]);
    if (pid <= 0) {
        fprintf(stderr, "Invalid PID\n");
        return 1;
    }

    // Read and print process status
    char status_path[64];
    snprintf(status_path, sizeof(status_path), "/proc/%d/status", pid);

    char status[STATUS_BUF_SIZE];
    FILE *status_file = fopen(status_path, "r");
    if (status_file) {
        if (fgets(status, STATUS_BUF_SIZE, status_file) != NULL) {
            print_process_status(status);
        }
        fclose(status_file);
    }

    // Read and print process group
    char pgid_path[64];
    snprintf(pgid_path, sizeof(pgid_path), "/proc/%d/stat", pid);

    int pgid;
    FILE *pgid_file = fopen(pgid_path, "r");
    if (pgid_file) {
        fscanf(pgid_file, "%*d %*s %*c %d", &pgid);
        fclose(pgid_file);
        printf("Process Group: %d\n", pgid);
    }

    // Add code to retrieve virtual memory and executable path if needed

    return 0;
}
