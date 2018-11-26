// Program: ftserver.c
// Author: Daniel Rotenberg
// Class: OSU 372 - Project # 2
// Description: This program is the server side of a file transfer system written in c 

// References: 
// Beej's Guide to Network Programming (https://beej.us/guide/bgnet/html/multi/index.html)
// https://www.tutorialspoint.com/unix_sockets/socket_server_example.htm
// https://stackoverflow.com/questions/41653419/c-store-a-list-of-files-in-a-directory-into-an-array

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <dirent.h>

#define BUF_SIZE 50000

// Function Declarations:

void portValidation(int port);

int main(int argc, char *argv[]) {
  
// set up file descriptors (listen on serverFD, new connection will be on newFD)
  int serverFD, newConnectFD;
// variable for port number storage
  int portNum;
// capture length of host
  socklen_t clientLen;
// char buffer
  // char buffer[BUF_SIZE]; 
// address structs
  struct sockaddr_in serverAddress, clientAddress;
// file array to store names of files
  int fileCount = 0;
  int i = 0;

  DIR *d;
  struct dirent *dir;
  d = opendir(".");

  //Determine the number of files
  while((dir = readdir(d)) != NULL) {
      if ( !strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..") )
      {

      } else {
          fileCount++;
      }
  }
  rewinddir(d);

  char *filesList[fileCount];

  //Put file names into the array
  while((dir = readdir(d)) != NULL) {
      if ( !strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..") )
      {}
      else {
        filesList[i] = (char*) malloc (strlen(dir->d_name)+1);
        strncpy (filesList[i],dir->d_name, strlen(dir->d_name) );
        i++;
      }
  }
  rewinddir(d);

// printf("Printing filelist\n");
// for(i=0; i < fileCount; i++) {
//   printf("%s\n", filesList[i]);
// }

// Check if arg count is correct
  if (argc < 2) { 
    fprintf(stderr, "Please use the correct # of arguments\n"); 
    exit(1); 
  }

// Create The Socket
  serverFD = socket(AF_INET, SOCK_STREAM, 0);

  if (serverFD < 0) {
    fprintf(stderr, "ERROR: Could Not Open Socket.\n"); 
    exit(1); 
  }

// Clear out the address struct
  memset((char *)&serverAddress, '\0', sizeof(serverAddress));

// Store the Port Number
  portNum = atoi(argv[1]);

// Test for valid port number, exit if error
  portValidation(portNum);

// Initialize Socket
  serverAddress.sin_family = AF_INET; // Create a network-capable socket
  serverAddress.sin_port = htons(portNum); // Store the port number
  serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

// Bind The Socket
  if (bind(serverFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {// Connect socket to port
    fprintf(stderr, "ERROR on binding");
  }

// Set Up Socket Listening
  if (listen(serverFD, 5) < 0) {
      fprintf(stderr, "Listener failed\n");
      exit(1);
    }

  printf("Server Open On Port # %d\n", portNum);

// Accept Connections from clients
  while(1) {
    clientLen = sizeof(clientAddress); // Get the size of the address for the client that will connect
    // printf("About to establish a connection\n");
    newConnectFD = accept(serverFD, (struct sockaddr *)&clientAddress, &clientLen); // Accept
    // printf("FD Accepted\n");
    if (newConnectFD < 0) {
      fprintf(stderr, "ERROR on accept");   
    }

    // char* connected = "Connection Established";
    char COMMAND[600];
    char FILENAME[600];
    char DATA_PORT[600];
    char HOST[600];
    int charsRead;
    char messageToSend[BUF_SIZE];

    memset(HOST, 0, sizeof(HOST)); //Make sure to clear before send
    memset(COMMAND, 0, sizeof(COMMAND)); //Make sure to clear before send
    memset(DATA_PORT, 0, sizeof(DATA_PORT)); //Make sure to clear before send
    memset(messageToSend, '\0', sizeof(messageToSend));
    // printf("Before Reading\n");
    charsRead = recv(newConnectFD, COMMAND, sizeof(COMMAND) - 1, 0); // Read data from the socket, leaving \0 at end
    // printf("Chars Read\n");
    // printf("Command: %s\n", COMMAND);
    
    if (strcmp(COMMAND, "l") == 0) {
      charsRead = recv(newConnectFD, DATA_PORT, sizeof(DATA_PORT) - 1, 0); 
      printf("List Command Requested on Port # %s\n", DATA_PORT);
      fflush(stdout); 
      // get client host name
      charsRead = recv(newConnectFD, HOST, sizeof(HOST) - 1, 0);
      printf("Sending Directory Contents To %s:%s\n", HOST, DATA_PORT);
      fflush(stdout);

      int complete = 1;
      while (complete) {
        for(i=0; i < fileCount; i++) {
          // printf("%s\n", filesList[i]);

          send(newConnectFD, filesList[i], strlen(filesList[i]), 0);
          // send(newConnectFD, newLine, strlen(newLine), 0);
          // fflush(stdout);
          sleep(1);
        }
        char * completed = "completed";
        sleep(1);
        send(newConnectFD, completed, strlen(completed), 0);  
        complete = 0;
      }
      sleep(1);
    }

    if (strcmp(COMMAND, "g") == 0) {
      memset(HOST, 0, sizeof(HOST)); //Make sure to clear before send
      memset(DATA_PORT, 0, sizeof(DATA_PORT)); //Make sure to clear before send
      memset(FILENAME, 0, sizeof(FILENAME)); //Make sure to clear before send
      printf("Get Command Received\n");
      fflush(stdout); 

      charsRead = recv(newConnectFD, DATA_PORT, sizeof(DATA_PORT) - 1, 0); 
      printf("Get Command Requested on Port # %s\n", DATA_PORT);
      fflush(stdout); 

      // get client host name
      charsRead = recv(newConnectFD, HOST, sizeof(HOST) - 1, 0);
      printf("Sending File To %s:%s\n", HOST, DATA_PORT);
      fflush(stdout);     

      charsRead = recv(newConnectFD, FILENAME, sizeof(FILENAME) - 1, 0);
      printf("Filename Request: %s\n", FILENAME);
      fflush(stdout);   

      int found = 0;
      for(i=0; i < fileCount; i++) {
        // printf("FILENAME: %s, filesList[i]: %s\n", FILENAME, filesList[i]);
        // fflush(stdout);   
        if (strcmp(FILENAME, filesList[i]) == 0) {
          printf("File Found!");
          fflush(stdout);
          found = 1;
          break;   
        }
      }

      if (found == 0) {
        printf("File not found!");
        fflush(stdout);
      }


    }    

    // printf("After If\n");
    // send(newConnectFD, connected, strlen(connected), 0);  


    memset(COMMAND, 0, sizeof(COMMAND)); // Clear out the buffer again
    memset(messageToSend, '\0', sizeof(messageToSend));
  }

// close serverFD before exiting
  // close(serverFD); 
  return 0;
}

void portValidation(int port) {
  if ((port > 65535 || port < 1024)) {
    fprintf(stderr, "ERROR: Port Number is Invalid. Please use a port in the range 1024-65535.\n");
    exit(1);
  }
  return;
}
