#include "../headers.h"

/*
struct PipedCommandDetails {
    char * command_details;
    struct CommandArgs * ca;
    int num_piped_commands;
} PipedCommandDetails;
*/

int splitByPipe(char *command_details, char **command_details_split) {
    int num_commands = 0;
    for (char *token = strtok(command_details, "|"); 
         token != NULL; token = strtok(NULL, "|")) 
    {
        int start = 0;
        int end = strlen(token) - 1;
        while (isspace(token[start])) start++;
        while (isspace(token[end])) end--;

        // If the token is only whitespace, then ignore it
        if (start > end) continue;

        // Add the token to the command_args array
        token[end + 1] = '\0';

        command_details_split[num_commands] = token + start;
        num_commands++;
    }

    return num_commands;
}

int pipe_split(char *command_details, char **command_args, int isBackground, char **errorString, char starting_directory[], char **previous_directory, struct PipedCommandDetails *pcd) {
    // Split the command details by "|"
    char *command_details_split[4096];
    int num_commands = splitByPipe(command_details, command_details_split);

    // Allocate memory for PipedCommandDetails
    pcd->num_piped_commands = num_commands;
    pcd->ca = (struct CommandArgs *)malloc(num_commands * sizeof(struct CommandArgs));

    // Iterate through each command
    for (int i = 0; i < num_commands; i++) {
        struct CommandArgs *ca = &(pcd->ca[i]);
        char *currCommandDetails = command_details_split[i];
        
        // Allocate memory for command_args
        ca->command_args = (char **)malloc(4096 * sizeof(char *));
        
        // Parse the command arguments
        parseCommandArgs(currCommandDetails, ca->command_args, ca);

        // Set input and output descriptors
        if (isBackground) {
            int null_fd = open("/dev/null", O_RDWR);
            ca->input_fd = null_fd;
            ca->output_fd = null_fd;
        } else {
            ca->input_fd = STDIN_FILENO;
            ca->output_fd = STDOUT_FILENO;
        }
        
        // Check for redirection
        // if (strstr(ca->command_details, "<") != NULL) {
        //     replaceInputRedirect(ca, errorString, starting_directory, previous_directory);
        // } 
        // if (strstr(ca->command_details, ">") != NULL) {
        //     replaceOutputRedirect(ca, errorString, starting_directory, previous_directory);
        // }
        // if (strstr(ca->command_details, ">>") != NULL) {
        //     ca->outputAppend = 1;
        //     replaceOutputRedirectAppend(ca, errorString, starting_directory, previous_directory);
        // }
    }

    return 0;
}


// int pipe_split(char *command_details, char ** command_args, int isBackground, char ** errorString, char starting_directory[], char ** previous_directory, struct PipedCommandDetails * pcd) {

//     // Maintain a copy of the command_details
//     char * command_details_copy = (char *) malloc(strlen(command_details) + 1);
//     strcpy(command_details_copy, command_details);

//     // Split the command details by "pipe"
//     char *command_details_split[4096];
//     int num_commands = splitByPipe(command_details, command_details_split);

//     // Print the command_details_split
//     printf("Command details split:\n");
//     for (int i = 0; i < num_commands; i++) {
//         printf("%d : %s\n", i, command_details_split[i]);
//     }

//     // struct PipedCommandDetails pcd;
//     pcd->num_piped_commands = num_commands;

//     pcd->command_details = command_details_copy;
//     struct CommandArgs * ca_list = (struct CommandArgs *)malloc(128 * sizeof(struct CommandArgs));

//     // Create command_args for each command
//     for (int i = 0; i < num_commands; i++) {
//         char * currCommandDetails = command_details_split[i];
//         // char * command_args[4096];

//         // Get some space for command_args
//         char ** command_args = (char **)malloc(4096 * sizeof(char *));

//         printf("\n\nBefore parsing : %s\n", currCommandDetails);

//         // Autocompleted by Github copilot
//         // struct CommandArgs ca = (struct CommandArgs)malloc(sizeof(struct CommandArgs));
//         struct CommandArgs ca;
        
//         parseCommandArgs(currCommandDetails, command_args, &ca);

//         // Print the struct command args
//         printf("After parsing : %s\n", ca.command_details);
//         printf("Command arguments:\n");
//         for (int i = 0; i < ca.num_args; i++) {
//             printf("%d : %s\n", i, command_args[i]);
//         }

//         // Copy ca to ca_list[i]
//         ca_list[i] = ca;

//         // ca_list[i] = ca;

//         // ca_list[i].outputAppend = 0;
//     }

//     // pcd->ca = ca_list;
//     // pcd->ca = (struct CommandArgs *)malloc(128 * sizeof(struct CommandArgs));
//     for (int i=0; i<num_commands; i++) {
//         pcd->ca[i] = ca_list[i];
//     }

//     // Now we need to update the input fd and output fd
//     // of each component of the piped command details
//     // according to the pipe
//     for (int i=0; i<pcd->num_piped_commands; i++) {
//         // // If the command is not the first command
//         // if (i != 0) {
//         //     // Set the input fd of the command to the output fd of the previous command
//         //     pcd->ca[i].input_fd = pcd->ca[i-1].output_fd;
//         // }

//         // // If the command is not the last command
//         // if (i != pcd->num_piped_commands - 1) {
//         //     // Create a pipe
//         //     int pipefd[2];
//         //     if (pipe(pipefd) == -1) {
//         //         errorHandler("Pipe creation failed", errorString);
//         //         return pcd;
//         //     }

//         //     // Set the output fd of the command to the write end of the pipe
//         //     pcd->ca[i].output_fd = pipefd[1];

//         //     // Set the input fd of the next command to the read end of the pipe
//         //     pcd->ca[i+1].input_fd = pipefd[0];
//         // }

//         int null_fd = open("/dev/null", O_RDWR);

//         // If the process is background, make the input and outpu
//         // fd as dev/null
//         if (isBackground) {
//             pcd->ca[i].input_fd = null_fd;
//             pcd->ca[i].output_fd = null_fd;
//         } else {
//             pcd->ca[i].input_fd = STDIN_FILENO;
//             pcd->ca[i].output_fd = STDOUT_FILENO;
//         }
//     }

//     // If the process itself is a background process
//     // the input fd of the first command will be /dev/null
//     // and the output fd of the last command will be /dev/null
//     // if (isBackground) {
//     //     // Open /dev/null for reading and writing
//     //     int null_fd = open("/dev/null", O_RDWR);

//     //     // Check if the file descriptor is valid
//     //     if (null_fd == -1) {
//     //         errorHandler("Opening /dev/null failed", errorString);
//     //         return pcd;
//     //     }

//     //     // Set the input fd of the first command to null_fd
//     //     pcd->ca[0].input_fd = null_fd;

//     //     // Set the output fd of the last command to null_fd
//     //     pcd->ca[pcd->num_piped_commands - 1].output_fd = null_fd;
//     // } else {
//     //     // Set the input of first command as stdio
//     //     pcd->ca[0].input_fd = STDIN_FILENO;

//     //     // Set the output of last command as stdio
//     //     pcd->ca[pcd->num_piped_commands - 1].output_fd = STDOUT_FILENO;
//     // }

//     // Make the last entry NULL-terminated
//     // ca_list[num_commands] = NULL;

//     // Lets do redirection later
//     // TODO: Handle redirection
//     for (int i=0; i<pcd->num_piped_commands; i++) {
//         // Check if "<" is present in pcd->ca[i]
//         if (strstr(pcd->ca[i].command_details, "<") != NULL) {
//             // If yes, then we need to redirect the input
//             // to the file mentioned after "<"
//             replaceInputRedirect(pcd->ca[i], errorString, starting_directory, previous_directory);
//         } 
//         // Check if ">" is present in pcd->ca[i]
//         if (strstr(pcd->ca[i].command_details, ">") != NULL) {
//             // If yes, then we need to redirect the output
//             // to the file mentioned after ">"
//             replaceOutputRedirect(pcd->ca[i], errorString, starting_directory, previous_directory);
//         }

//         // Check if ">>" is present in pcd->ca[i]
//         if (strstr(pcd->ca[i].command_details, ">>") != NULL) {
//             pcd->ca[i].outputAppend = 1;
//             // If yes, then we need to redirect the output
//             // to the file mentioned after ">>"
//             replaceOutputRedirectAppend(pcd->ca[i], errorString, starting_directory, previous_directory);
//         }
//     }

//     // Print each entry of the struct
//     printf("\n\nInside pipe splitter\n");
//     for (int i=0; i<pcd->num_piped_commands; i++) {
//         printf("Command details: %s\n", pcd->ca[i].command_details);
//         printf("Command args:\n");
//         for (int j=0; j<pcd->ca[i].num_args; j++) {
//             printf("%d : %s\n", j, pcd->ca[i].command_args[j]);
//         }
//         printf("Input fd: %d\n", pcd->ca[i].input_fd);
//         printf("Output fd: %d\n", pcd->ca[i].output_fd);
//     }
//     printf("end of pipe splitter\n\n");

//     return 0;
// }