#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

// This is also completely ChatGPT generated

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

int main(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "-n") != 0) {
        fprintf(stderr, "Usage: %s -n <time_arg>\n", argv[0]);
        return 1;
    }

    int time_arg = atoi(argv[2]);

    if (time_arg <= 0) {
        fprintf(stderr, "Invalid time_arg\n");
        return 1;
    }

    char c;
    int pid = -1;
    int child_running = 0; // Flag to track if the child process is running

    while (1) {
        setbuf(stdout, NULL);
        enableRawMode();

        if (!child_running) {
            // Start a new child process
            int new_pid = fork();
            if (new_pid == -1) {
                die("fork");
            } else if (new_pid == 0) {
                // Child process
                pid = getpid();
                while (1) {
                    printf("%d\n", pid);
                    sleep(time_arg);
                }
            } else {
                // Parent process
                child_running = 1; // Set flag to indicate child is running
            }
        }

        if (read(STDIN_FILENO, &c, 1) == 1) {
            if (c == 'x') {
                // Toggle the flag to stop the child process
                child_running = 0;
                disableRawMode();
                exit(0);
            }
        }
    }

    return 0;
}
