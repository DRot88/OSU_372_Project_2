// Program: ftserver.c
// Author: Daniel Rotenberg
// Class: OSU 372 - Project # 2

// References: 
// Beej's Guide to Network Programming (https://beej.us/guide/bgnet/html/multi/index.html)
// https://www.tutorialspoint.com/unix_sockets/socket_server_example.htm

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
  char buffer[BUF_SIZE]; 
// address structs
  struct sockaddr_in serverAddress, clientAddress;

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

// Accept Connections from clients
  while(1) {
    clientLen = sizeof(clientAddress); // Get the size of the address for the client that will connect
    // printf("About to establish a connection\n");
    newConnectFD = accept(serverFD, (struct sockaddr *)&clientAddress, &clientLen); // Accept
    // printf("FD Accepted\n");
    if (newConnectFD < 0) {
      fprintf(stderr, "ERROR on accept");   
    }
  }

// close serverFD before exiting
  close(serverFD); 
  return 0;
}

void portValidation(int port) {
  if ((port > 65535 || port < 1024)) {
    fprintf(stderr, "ERROR: Port Number is Invalid. Please use a port in the range 1024-65535.\n");
    exit(1);
  }
  return;
}
