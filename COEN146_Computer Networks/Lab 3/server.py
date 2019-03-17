import socket
import sys

#This program sets up the server which continiously waits for new files/data to be sent over from the client.
def server(port):        
   
    # Create a socket object given the proper socket specifications.
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Bind the socket object to your localhost and port
    sock.bind(('localhost',6903))

    # Receive a filename and open a file
    data, address = sock.recvfrom(1024)
    
    #The first data we receive from client will be the name of the file we're going to store everything
    with open(data.decode('ascii','ignore'),'w') as f:
        
        # Poll for all the data that should go into the file
        # When we haven't receveied the control message yet.
        while data.decode('ascii','ignore') is not " ":
            #Decode the data and write to our file
            data, address = sock.recvfrom(1024)
            f.write(data.decode('ascii','ignore'))

    # Close the socket
    sock.close()

if __name__ == '__main__':
    if len (sys.argv) > 1:
        try:
            port = int(sys.argv[1])
            print(port)
            server(port)
        except ValueError:
            raise

    else:
            print ('Usage: python3 server.py port')
