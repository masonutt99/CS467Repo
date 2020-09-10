//Code obtained and modified from https://www.geeksforgeeks.org/udp-server-client-implementation-c/
/**
 * 
 * @authors Mason Utt and Timothy Jackson
 * 
 * Doc: C2C Duran and Olszewski explained to only adjust udp_client and to reverse engineer server.
 * EI with Lt Col Merrit helped get the file to be read to be sent to server
 * 
 * 
**/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

#define PORT     4240
#define MAXLINE 1024

// struct timeval timeout; //structure for timeout
// timeout.tv_sec = 5;  //5 second timeout
// timeout.tv_usec = 0; //0 milliseconds
// if (setsockopt(sockfd, SQL_SOCKET, SQ_RCVTIMEO, (char *) &timeout, sizeof(timeout)) < 0){
//     perror("setsockopt failed");
//     exit(EXIT_FAILURE);
// }

// Driver code
int main() {
    int input;  //the user input`
    int sockfd; //Socket descriptor, like a file-handle
    char buffer[MAXLINE]; //buffer to store message from server
    char *hello = "Hello from client"; //message to send to server
    char *list  = "LIST_REQUEST"; //message sent to server to list songs.
    
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

    do
    {
        int frames = 1;
        int SumOfBytes = 0;
        do
        {
        printf("Enter one of the following commands:\n");
        printf("\"1\" = List Songs\n");
        printf("\"2\" = Stream a Song\n");
        printf("\"3\" = exit\n");       // standard  question
        scanf("%d", &input);        //gets input from user
        } while (input != 1 && input != 2 && input != 3);
        fflush(stdin);              //clears input line so new input is needed each time

        if (input == 3)     // exits while loop if user inputs 3
        {
            printf("Exit");
            break;
        }
        if (input == 2)     // collects song name then streams if user input is 2
        {
            char songName[MAXLINE] = "";
            char stream[MAXLINE] = "START_STREAM\n"; //first half of stream message
            printf("Please enter a song name: ");

            int c;
                do{
                c = getchar();
                }while(c != EOF && c != '\n');  // buffer for input from scan f (from stack over flow)

            fgets(songName, MAXLINE, stdin);
            fflush(stdin);
            songName[strlen(songName)-1] = '\0';
            strcat(stream, songName) ;

            printf("Sending start stream\n");
            sendto(sockfd, (const char *)stream, strlen(stream), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
            printf("Waiting for response\n");
            FILE* fp;
            
            if (strstr(buffer, "COMMAND_ERROR") == NULL){
            while (strstr(buffer, "STREAM_DONE") == NULL)
            {
                if(( n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, (struct sockaddr *) &servaddr, &len))<0)
                    {
                    perror("ERROR");
                    printf("Errno: %d. ",errno);
                    exit(EXIT_FAILURE);
                    }
                buffer[n] = '\0'; //terminate message
                if (strstr(buffer, "STREAM_DATA"))
                {
                    
                    SumOfBytes = SumOfBytes + (n-12);
                    printf("Frame # %d received with %d bytes\n", frames, n-12);
                    frames++;
                } 
            }
            printf("Stream done.  Total Frames: %d Total Size: %d bytes\n", frames - 1, SumOfBytes);
            }
            printf("Done!\n");
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
        fflush(stdin); // clears the stdin
    } while (input != 3); // do while loop goes back to top
    
    printf("closing socket");
    close(sockfd);
    return 0;
}