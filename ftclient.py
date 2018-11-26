# Program: ftserver.c
# Author: Daniel Rotenberg
# Class: OSU 372 - Project # 2
# Description: This program is the client side of a file transfer system written in python 

# References: https://docs.python.org/release/2.6.5/library/socketserver.html

# 3. ftclient starts on Host B, and validates any pertinent command-line parameters.
# (<SERVER_HOST>, <SERVER_PORT>, <COMMAND>, <FILENAME>, <DATA_PORT>)

from socket import *
import sys
import time

def argValidation():
  if len(sys.argv) < 5:  # Make sure correct number of arguments
    print ("Usage: python ftclient.py [hostname] [server port] [command] [filename] [data port]\n [filename] is optional\n")
    exit(1)

def portValidation(portNum):
  if (int(portNum) > 65535 or (portNum) < 1024):
    print("ERROR: Port Number is invalid. Please use a port in the range 1024-65535.")
    exit(1)

def commandValidation(com):
  if (com.lower() != "-g" and com.lower() != "-l"):
    print("ERROR: Command is invalid. Valid Commands are '-l' for directory list, and '-g' [filename] to get a file.\n")
    exit(1)

if __name__ == "__main__":
  # print ("Arg Count: %d"  % len(sys.argv))
  argValidation()

  try:
    HOST = sys.argv[1] # get the host name

    PORT = int(sys.argv[2]) # get the port number specificed in the command line
    portValidation(PORT) # verify port number validity

    COMMAND = sys.argv[3] # store command
    commandValidation(COMMAND) # verify command validity

    if (len(sys.argv) == 6): # set filename/data_port based on arg count
      FILENAME = sys.argv[4]
      try:
        DATA_PORT = int(sys.argv[5])
      except:
        print("ERROR: Data Port needs to be an integer in the range 1024-65535")
        exit(1)
      portValidation(DATA_PORT)
      # print ("Filename: %s" % FILENAME)
      # print ("Data_Port: %s" % DATA_PORT)
    elif (len(sys.argv) == 5):
      try:
        DATA_PORT = int(sys.argv[4])
      except:
        print("ERROR: Data Port needs to be an integer in the range 1024-65535")
        exit(1)
      # print ("Data_Port: %s" % DATA_PORT)
      portValidation(DATA_PORT)

    cSocket = socket(AF_INET,SOCK_STREAM) # Create a socket (SOCK_STREAM means a TCP socket)
    cSocket.connect((HOST, PORT)) # connect to port
    # print "Connection Successful"
    # data = "Hello Server!"

    print("Command: %s" % COMMAND.lower())
    if (COMMAND.lower() == "-l"):
      cSocket.send("l")
      cSocket.send(str(DATA_PORT))
      time.sleep(1)
      cSocket.send(str(HOST))

      itemInFileList = cSocket.recv(100)
        # while we havent reached the end
      while itemInFileList != "completed":
          # continue printing
        print itemInFileList
        # print "\n"
        itemInFileList = cSocket.recv(100)



      # print (serverResponse)

    if (COMMAND.lower() == "-g"):
      cSocket.send("g")
      cSocket.send(str(DATA_PORT))
      time.sleep(1)
      cSocket.send(str(HOST))
      time.sleep(1)
      cSocket.send(str(FILENAME))
      # serverResponse = cSocket.recv(1024)
      # print (serverResponse)      
    # print "Sent: %s" % data
  except KeyboardInterrupt:
    print ("\nKeyboard Interrupt: Exiting Chat")
    sys.exit()
