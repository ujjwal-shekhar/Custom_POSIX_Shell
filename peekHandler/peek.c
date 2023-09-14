#include "../headers.h"

int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int peek(char ** command_args, char starting_directory[], char ** previous_directory) {
    int listFlag = 0, allFlag = 0, pathFound = 0;
    char * path = NULL;

    for (int i=1; i<4; i++) {
        if (command_args[i] == NULL) break;

        // Check if argument is only white space
        int isWhitepace = 1;
        for (int j=0; j<strlen(command_args[i]); j++) {
            if (!isspace(command_args[i][j])) {
                isWhitepace = 0;
                break;
            }
        }

        if (isWhitepace) break;


        struct FlagInfo fi; fi.isFlag = 0; fi.flags = NULL;
        // TODO : Handle error return
        flagHandler(command_args[i], starting_directory, "-", &fi);

        struct PathInfo pi = pathHandler(command_args[i], starting_directory, previous_directory);

        // Check if this is a flag
        if (fi.isFlag) {
            // The path must not come before the flag
            if (pathFound) {
                fprintf(stderr, RED_COLOR);
                fprintf(stderr, "SYNTAX ERROR : Correct Usage : peek <flags> <path/name>\n");
                fprintf(stderr, RESET_COLOR);
                return 1;
            }

            // Go over the string of flags
            for (int j=0; j<strlen(fi.flags); j++) {
                // Detect invalid flags
                if (fi.flags[j] != 'l' && fi.flags[j] != 'a') {
                    fprintf(stderr, RED_COLOR);
                    fprintf(stderr, "SYNTAX ERROR : Invalid flag(s)\n");
                    fprintf(stderr, RESET_COLOR);
                return 1;
                }

                listFlag |= (fi.flags[j] == 'l');
                allFlag |= (fi.flags[j] == 'a');
            }
        } else if (pi.isPath) {
            path = pi.path;
            pathFound = 1;
        } else {
            // printf("Flags : %d %d, Pathfound : %d at %d\n", listFlag, allFlag, pathFound, i);
            fprintf(stderr, RED_COLOR);
            fprintf(stderr, "SYNTAX ERROR : Invalid argument\n");
            fprintf(stderr, RESET_COLOR);
            return 1;
        }
    }

    if (!pathFound) {
        path = (char *) malloc(sizeof(char) * 4096);
        strcpy(path, starting_directory);
        pathFound = 1;
    }

    if (pathFound) {
        // Go over all the files in pi.path
        // Autocompleted by ChatGPT-3.5-Turbo
        // Prompt used : `how to implement ls in C`
        // Prompt2 : `How can I display these in lexicographical order ?`
        struct dirent **entry;
        int n = scandir(path, &entry, NULL, alphasort);

        if (n < 0) {
            fprintf(stderr, RED_COLOR);
            fprintf(stderr, "ERROR : Directory not found\n");
            fprintf(stderr, RESET_COLOR);
            return 1;
        }
        if (listFlag) printf("total %d\n", n); // TODO : Fix this number
        
        for (int k = 0; k < n; k++) {
            if ((!allFlag) && (entry[k]->d_name[0] == '.')) continue;

            struct stat fileStat;
            // Concatenate the calling directory and the filename
            char * temp = (char *) malloc(sizeof(char) * (strlen(path) + strlen(entry[k]->d_name) + 2));
            strcpy(temp, path);
            strcat(temp, "/");
            strcat(temp, entry[k]->d_name);
            stat(temp, &fileStat);
            if (listFlag) {
                // Bibliography : https://stackoverflow.com/questions/17578647/what-does-terminal-command-ls-l-show

                // Print the permissions
                // Auto completed by Github Copilot
                printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
                printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
                printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
                printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
                printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
                printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
                printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
                printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
                printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
                printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");

                // Add a space after perms
                printf(" ");

                // Get the number of hardlinks
                // Auto completed by Github Copilot
                struct dirent *entry2;
                DIR *dp2 = opendir(path);
                int hardlinks = 0;

                while ((entry2 = readdir(dp2)) != NULL) {
                    if (strcmp(entry[k]->d_name, entry2->d_name) == 0) {
                        hardlinks++;
                    }
                }

                // Get the username and groupname
                struct passwd *pw = getpwuid(fileStat.st_uid);
                struct group *gr = getgrgid(fileStat.st_gid);

                // Get the size of the file
                // Auto completed by Github Copilot
                int size = fileStat.st_size;

                // Get the modification time
                // Auto completed by Github Copilot
                char * time = ctime(&fileStat.st_mtime);
                time[strcspn(time, "\n")] = '\0';

                // Check if the file is a directory 
                // or a file or an executable
                // Auto completed by Github Copilot
                if (S_ISDIR(fileStat.st_mode)) {
                    // Print filename in blue and bold
                    printf("%4d %s %s %7d %s \033[34;1m%s\033[0m\n", 
                            hardlinks, pw->pw_name, gr->gr_name, size, time, entry[k]->d_name);
                } else if (fileStat.st_mode & S_IXUSR) {
                    // Print filename in green
                    printf("%4d %s %s %7d %s \033[32;1m%s\033[0m\n",
                            hardlinks, pw->pw_name, gr->gr_name, size, time, entry[k]->d_name);
                } else {
                    printf("%4d %s %s %7d %s %s\n", 
                            hardlinks, pw->pw_name, gr->gr_name, size, time, entry[k]->d_name);
                }
            } else {
                if (S_ISDIR(fileStat.st_mode)) {
                    // Print filename in blue and bold
                    printf("\033[34;1m%s\033[0m ", entry[k]->d_name);
                } else if (fileStat.st_mode & S_IXUSR) {
                    // Print filename in green
                    printf("\033[32;1m%s\033[0m ", entry[k]->d_name);
                } else {
                    printf("%s ", entry[k]->d_name);
                }
            }    

            free(temp); 
        }
        printf("\n");
    } else {
        fprintf(stderr, RED_COLOR);
        fprintf(stderr, "ERROR : Directory not found\n");
        fprintf(stderr, RESET_COLOR);
        return 1;
    }

    return 0;
}