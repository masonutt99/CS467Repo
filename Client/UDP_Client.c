//Code obtained and modified from https://www.geeksforgeeks.org/udp-server-client-implementation-c/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

#define PORT     4240
#define MAXLINE 1024

// Driver code
int main() {
    int input;  //the user input`
    int sockfd; //Socket descriptor, like a file-handle
    char buffer[MAXLINE]; //buffer to store message from server
    char *hello = "Hello from client"; //message to send to server
    struct sockaddr_in     servaddr;  //we don't bind to a socket to send UDP traffic, so we only need to configure server address

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Filling server information
    servaddr.sin_family = AF_INET; //IPv4
    servaddr.sin_port = htons(PORT); // port, converted to network byte order (prevents little/big endian confusion between hosts)
    servaddr.sin_addr.s_addr = INADDR_ANY; //localhost

    int n, len = sizeof(servaddr);

    while (1)
    {
        printf("Enter one of the following commands:\n");
        printf("\"1\" = List Songs\n");
        printf("\"2\" = Stream a Song\n");
        printf("\"3\" = exit\n");       // standard  question
        scanf("%d", &input);        //gets input from user
        fflush(stdin);              //clears input line so new input is needed each time

        if (input == 3)     // exits while loop if user inputs 3
        {
            break;
        }
        if (input == 2)     // collects song name then streams if user input is 2
        {
            
            
        }
        
        
        
    }
    
    //Sending message to server
    sendto(sockfd, (const char *)hello, strlen(hello), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    printf("Hello message sent.\n");

    // Receive message from client
    if(( n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, (struct sockaddr *) &servaddr, &len))<0)
    {
        perror("ERROR");
        printf("Errno: %d. ",errno);
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0'; //terminate message
    printf("Server : %s\n", buffer);

    close(sockfd);
    return 0;
}