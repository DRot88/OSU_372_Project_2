# Program: ftserver.c
# Author: Daniel Rotenberg
# Class: OSU 372 - Project # 2

# References: https://docs.python.org/release/2.6.5/library/socketserver.html

# 3. ftclient starts on Host B, and validates any pertinent command-line parameters.
# (<SERVER_HOST>, <SERVER_PORT>, <COMMAND>, <FILENAME>, <DATA_PORT>)

from socket import *
import sys

def argValidation():
  if len(sys.argv) < 5:  # Make sure correct number of arguments
    print ("Usage: python ftclient.py [hostname] [server port] [command] [filename] [data port]\n [filename] is optional\n")
    exit(1)

def portValidation(portNum):
  if (int(portNum) > 65535 or (portNum) < 1024):
    print("ERROR: Port Number is invalid. Please use a port in the range 1024-65535.\n")
    exit(1)

def commandValidation(com):
  if (com.lower() != "-g" and com.lower() != "-l"):
    print("ERROR: Command is invalid. Valid Commands are '-l' for directory list, and '-g' [filename] to get a file.\n")
    exit(1)

if __name__ == "__main__":
  print ("Arg Count: %d"  % len(sys.argv))
  argValidation()

  try:
    HOST = sys.argv[1] # get the host name

    PORT = int(sys.argv[2]) # get the port number specificed in the command line
    portValidation(PORT) # verify port number validity

    COMMAND = sys.argv[3] # store command
    commandValidation(COMMAND) # verify command validity

    if (len(sys.argv) == 6): # set filename/data_port based on arg count
      FILENAME = sys.argv[4]
      DATA_PORT = sys.argv[5]
      # print ("Filename: %s" % FILENAME)
      # print ("Data_Port: %s" % DATA_PORT)
    elif (len(sys.argv) == 5):
      DATA_PORT = sys.argv[4]
      # print ("Data_Port: %s" % DATA_PORT)

    cSocket = socket(AF_INET,SOCK_STREAM) # Create a socket (SOCK_STREAM means a TCP socket)
    cSocket.connect((HOST, PORT)) # connect to port
    # print "Connection Successful"
    # data = "Hello Server!"
    # cSocket.send(data) #test statement
    # print "Sent: %s" % data
  except KeyboardInterrupt:
    print ("\nKeyboard Interrupt: Exiting Chat")
    sys.exit()
