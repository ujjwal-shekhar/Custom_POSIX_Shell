#include "../headers.h"

// Usage of ChatGPT-3.5Turbo
    /*
    Prompt used :
    Complete this part : "if (fFlag) { // Only search for files with exact name
        // ... (implement searching for files)
    } "
    */

// Usage of GH Copilot
/*
Copilot wrote trash code for most of this 
that I had to correct

the code is 40% copilot and 60% me
*/

int numMatchingFiles = 0, numMatchingDirs = 0;

int traverseDirectry(char * searchTerm, char ** errorString, char * path, int dFlag, int fFlag, char ** directoryPath, char ** filePath) {
    // Open the directory
    DIR *dir = opendir(path);
    if (!dir) {
        errorHandler("\033[31mUnable to open directory\033[0m", errorString);
        return 1;        
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (dFlag) { // Check if this entry is a directory
                if (strcmp(entry->d_name, searchTerm) == 0) {
                    numMatchingDirs++;

                    // Construct the relative path to the file or directory
                    // with respect to the starting directory
                    char relative_path[4096];
                    snprintf(relative_path, sizeof(relative_path), "%s/%s", path, entry->d_name);
                    printf("\033[34;1m%s\033[0m\n", relative_path + strlen(path) + 1);

                    // Save this relative path to directoryPath
                    // printf("The path is %s\n", *directoryPath);  

                    char * tempPath = (char *) malloc(sizeof(char) * 4096);
                    if (tempPath == NULL) {
                        errorHandler("\033[31mMemory allocation failed\033[0m", errorString);
                        return 1;
                    }

                    strcpy(tempPath, path);
                    strcat(tempPath, relative_path + strlen(path) + 1);
                    strcpy(*directoryPath, tempPath);                  

                    // printf("The path is %s\n", *directoryPath);  
                }
            }

            // printf("Entered if\n");
            // Check if the directory is not . or ..
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                // printf("%s\n", entry->d_name);
                // Recursively call traverseDirectory
                // make relative path from here to child
                char * childPath = (char *) malloc(strlen(path) + strlen(entry->d_name) + 2);
                strcpy(childPath, path);
                strcat(childPath, "/");
                strcat(childPath, entry->d_name);
                traverseDirectry(searchTerm, errorString, childPath, dFlag, fFlag, directoryPath, filePath);
            }
        } else if (entry->d_type == DT_REG) {
            // printf("%s\n", entry->d_name);
            // Check if the file name matches the search term
            if (fFlag) { // Only search for files with exact name
                char * dName_copy = (char *) malloc(sizeof(char) * (strlen(entry->d_name) + 1));
                strcpy(dName_copy, entry->d_name);

                char * nameWithoutExtension = strtok(dName_copy, ".");
                if (strcmp(nameWithoutExtension, searchTerm) == 0) {
                    numMatchingFiles++;

                    // Construct the relative path to the file or directory
                    char relative_path[4096];
                    snprintf(relative_path, sizeof(relative_path), "%s/%s", path, entry->d_name);
                    printf("\033[32;1m%s\033[0m\n", relative_path + strlen(path) + 1);

                    // Save this relative path to filePath
                    strcat(path, relative_path + strlen(path));
                    strcpy(*filePath, path);
                }
            } 
        }
    }

    closedir(dir);
}

int seek(char ** command_args, char ** errorString, char starting_directory[], char ** previous_directory) {
    int dFlag = 0, fFlag = 0, eFlag = 0;
    char * path = NULL;
    char * searchTerm = NULL;

    // Last position of flag, search and path
    int posLast[] = {-1, -1, -1};

    for (int i=1; i<6; i++) {
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

        struct FlagInfo fi = flagHandler(command_args[i], errorString, starting_directory, "-");
        struct PathInfo pi = pathHandler(command_args[i], errorString, starting_directory, previous_directory);

        // This is either a flag
        if (fi.isFlag) {
            posLast[0] = i;

            // Go over the string of flags
            for (int j=0; j<strlen(fi.flags); j++) {
                // Detect invalid flags
                if (fi.flags[j] != 'd' && fi.flags[j] != 'e' && fi.flags[j] != 'f') {
                    errorHandler("\033[31mInvalid flag\033[0m", errorString);
                    return 1;
                }

                dFlag = (fi.flags[0] == 'd');
                eFlag = (fi.flags[0] == 'e');
                fFlag = (fi.flags[0] == 'f');
            }
        } else {
            if (posLast[1] != -1) { // If a search term was found already
                posLast[2] = i;
                if (!pi.isPath) {
                    errorHandler("\033[31mInvalid path\033[0m", errorString);
                    return 1;
                } else {
                    path = pi.path;
                }
            } else {
                posLast[1] = i;
                searchTerm = command_args[i];
            }
        }
    }

    if (!(
        (posLast[0] < posLast[1]) &&
        (posLast[0] < posLast[2]) &&
        (posLast[1] != -1) &&
        (posLast[2] != -1)
    )) {
        errorHandler("\033[31mSyntax Error\nCorrect Usage : seek <flags> <search_term> <path/name>\033[0m", errorString);
        return 1;
    } else if (dFlag && fFlag) {
        errorHandler("\033[31mInvalid flags!\033[0m", errorString);
        return 1;
    }

    // Swap d and f flags
    if ((!dFlag) && (!fFlag)) {
        dFlag = 1;
        fFlag = 1;
    }

    char * directoryPath = (char *) malloc(sizeof(char) * 4096);
    char * filePath = (char *) malloc(sizeof(char) * 4096);

    if (directoryPath == NULL || filePath == NULL) {
        errorHandler("\033[31mMemory allocation failed\033[0m", errorString);
        return 1;
    }

    // Call traverseDirectory
    traverseDirectry(searchTerm, errorString, path, dFlag, fFlag, &directoryPath, &filePath);

    // Handling the "-e" flag
    if (numMatchingFiles + numMatchingDirs == 1) {
        if (dFlag && numMatchingDirs) {

            if (chdir(searchTerm) != 0) {
                errorHandler("\033[31mMissing permissions for task!\033[0m", errorString);
                return 1;
            }
        } if (fFlag && numMatchingFiles) {
            // printf("\033[32;1m%s\033[0m\n", searchTerm);
            // Print the contents of the file
            FILE * file = fopen(filePath, "r");
            if (file == NULL) {
                errorHandler("\033[31mUnable to open file\033[0m", errorString);
                return 1;
            }

            char buffer[4096];
            while (fgets(buffer, sizeof(buffer), file) != NULL) {
                printf("%s", buffer);
            }

            fclose(file);
        }
    } 

    return 0;
}

