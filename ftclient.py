# Program: ftserver.c
# Author: Daniel Rotenberg
# Class: OSU 372 - Project # 2

# References: https://docs.python.org/release/2.6.5/library/socketserver.html

# 3. ftclient starts on Host B, and validates any pertinent command-line parameters.
# (<SERVER_HOST>, <SERVER_PORT>, <COMMAND>, <FILENAME>, <DATA_PORT>)

from socket import *
import sys

if __name__ == "__main__":
  if len(sys.argv) < 6:  # Make sure correct number of arguments
    print ("Usage: python ftclient.py [hostname] [server port] [command] [filename] [data port]")
    exit(1)
  
  try:
    HOST = sys.argv[1] # get the host name
    PORT = int(sys.argv[2]) # get the port number specificed in the command line
    # print("TCP_PORT: %s" % TCP_PORT) # test if correct port number
    cSocket = socket(AF_INET,SOCK_STREAM) # Create a socket (SOCK_STREAM means a TCP socket)
    cSocket.connect((HOST, PORT)) # connect to port
    # print "Connection Successful"
    # data = "Hello Server!"
    # cSocket.send(data) #test statement
    # print "Sent: %s" % data
  except KeyboardInterrupt:
    print ("\nKeyboard Interrupt: Exiting Chat")
    sys.exit()
