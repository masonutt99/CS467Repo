//Code obtained and modified from https://www.geeksforgeeks.org/udp-server-client-implementation-c/
/**
 * 
 * @authors Mason Utt and Timothy Jackson
 * 
 * 
 * 
**/
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
    char *list  = "LIST_REQUEST"; //message sent to server to list songs.
    char stream[MAXLINE] = "START_STREAM\n"; //first half of stream message
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
            printf("Exit");
            break;
        }
        if (input == 2)     // collects song name then streams if user input is 2
        {
            char songName[MAXLINE];
            printf("Please enter a song name: ");

            int c;
                do{
                c = getchar();
                }while(c != EOF && c != '\n');  // buffer for input from scan f (from stack over flow)

            fgets(songName, MAXLINE, stdin);
            printf("String is: %s", songName);
            fflush(stdin);
            songName[strlen(songName)-1] = '\0';
            strcat(stream, songName) ;
            printf("file name: %s", stream);
            sendto(sockfd, (const char *)stream, strlen(stream), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));

        }
        if (input == 1)
        {
            sendto(sockfd, (const char *)list, strlen(list), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
            printf("Requesting a list of songs\nSongs Available:\n");
        }
        
        if(( n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, (struct sockaddr *) &servaddr, &len))<0)
        {
        perror("ERROR");
        printf("Errno: %d. ",errno);
        exit(EXIT_FAILURE);
        }
        buffer[n] = '\0'; //terminate message
        printf("Server : %s\n", buffer);
        fflush(stdin); // clears the stdin
    } // do while loop goes back to top

    
    //Sending message to server
    // sendto(sockfd, (const char *)hello, strlen(hello), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    // printf("Hello message sent.\n");

    // Receive message from client
    // if(( n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, (struct sockaddr *) &servaddr, &len))<0)
    // {
    //     perror("ERROR");
    //     printf("Errno: %d. ",errno);
    //     exit(EXIT_FAILURE);
    // }
    // buffer[n] = '\0'; //terminate message
    // printf("Server : %s\n", buffer);

    close(sockfd);
    return 0;
}