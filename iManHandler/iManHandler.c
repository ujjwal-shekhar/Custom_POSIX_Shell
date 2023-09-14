#include "iManHandler.h"

#define MAX_BUFFER_SIZE 1024

// Completely taken from ChatGPT

// // Function to clean and extract man page content
// void cleanAndExtractManPage(const char* html) {
//     const char* start = strstr(html, "<PRE>");
//     const char* end = strstr(html, "</PRE>");

//     if (start != NULL && end != NULL) {
//         start += 5; // Skip "<PRE>"
//         int len = end - start;

//         // Process and print the extracted content
//         while (len > 0) {
//             if (*start == '<') {
//                 // Skip HTML tags
//                 while (*start != '>' && len > 0) {
//                     start++;
//                     len--;
//                 }
//                 if (*start == '>') {
//                     start++;
//                     len--;
//                 }
//             } else {
//                 putchar(*start);
//                 start++;
//                 len--;
//             }
//         }
//     }
// }

// Function to fetch and display a man page from http://man.he.net/
int iman(char ** command_args) {
    char * commandName = command_args[1];

    struct hostent* server;
    struct sockaddr_in server_addr;
    int sockfd;

    // Perform DNS resolution for man.he.net
    server = gethostbyname("man.he.net");
    if (server == NULL) {
        fprintf(stderr, "Error: DNS resolution failed\n");
        return 1;
    }

    // Initialize server_addr structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Open a TCP socket to the IP address
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        return 1;
    }

    // Send a GET request to the website's server
    char request[MAX_BUFFER_SIZE];
    snprintf(request, sizeof(request), "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n", commandName);

    // printf("Request: %s\n", request);


    if (send(sockfd, request, strlen(request), 0) < 0) {
        perror("send");
        close(sockfd);
        return 1;
    }

    // Read and display the body of the website (man page)
    char response[MAX_BUFFER_SIZE];
    int bytesRead;

    // cleanAndExtractManPage(response);

    while ((bytesRead = recv(sockfd, response, sizeof(response) - 1, 0)) > 0) {
        response[bytesRead] = '\0';
        printf("%s", response);
    }

    // Close the socket
    close(sockfd);

    return 0;
}