#include "neonate.h"

#define MAX_BUFFER_SIZE 256

// All of this is completely ChatGPT-ed

void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

void printFifthFieldFromLoadAvg() {
    FILE *loadavg_file = fopen("/proc/loadavg", "r");
    if (loadavg_file == NULL) {
        die("Failed to open /proc/loadavg");
    }

    char buffer[MAX_BUFFER_SIZE];
    if (fgets(buffer, sizeof(buffer), loadavg_file) == NULL) {
        die("Failed to read /proc/loadavg");
    }

    // Close the file as soon as possible
    fclose(loadavg_file);

    // Tokenize the contents and print the 5th token (index 4)
    char *token = strtok(buffer, " ");
    for (int i = 0; i < 4 && token != NULL; i++) {
        token = strtok(NULL, " ");
    }

    if (token != NULL) {
        printf("%s", token);
    } else {
        die("Failed to tokenize /proc/loadavg");
    }
}

int neonate(char ** command_args) {
    if (strcmp(command_args[1], "-n") != 0) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, "Usage: %s -n <time_arg>\n", command_args[0]);
        fprintf(stderr, RESET_COLOR);
        return 1;
    }

    int time_arg = atoi(command_args[2]);

    if (time_arg <= 0) {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, "Invalid time_arg\n");
        fprintf(stderr, RESET_COLOR);
        return 1;
    }

    char c;
    int pid = -1;
    int print_flag = 1; // Flag to control printing

    while (1) {
        setbuf(stdout, NULL);
        enableRawMode();

        // Check if there's a new process
        int new_pid = fork();
        if (new_pid == -1) {
            die("fork");
        } else if (new_pid == 0) {
            // Child process
            pid = getpid();
            while (1) {
                if (print_flag) {
                    printFifthFieldFromLoadAvg();
                    sleep(time_arg);
                }
            }   
        } else {
            // Parent process
            while (1) {
                if (read(STDIN_FILENO, &c, 1) == 1) {
                    if (c == 'x') {
                        // Toggle the print flag to stop printing
                        print_flag = 0;
                        kill(new_pid, SIGTERM); // Send SIGTERM to child process
                        waitpid(new_pid, NULL, 0); // Wait for child process to exit
                        disableRawMode();
                        exit(0);
                    }
                }
            }
        }
    }

    return 0;
}
