import socket
import sys

#This function sets up the client program and readies to foward a file to the server
def client(host, port, file_name):        
    # Resolve the IP Address given the hostname and the port number
    try:
        ai = socket.getaddrinfo(host,port)
    except socket.gaierror: 
        # insert error handling code because we couldn't resolve host name
        print("We had an error.")

    # Create a socket object with the proper socket specifications.
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Send over a new file name
    # The first thing sent to the server is the name of the new file where everything is going to be stored.
    strTest = 'some.txt'.encode('ascii', 'ignore')
    sock.sendto(strTest, (ai[0][4]))

    # Open the file to transfer and start sending data using the created socket object
    # At the end, you should send a control message, just an empty string to notify the end of file
    with open(file_name, 'r') as f:
        for line in f:
            #Have to encode the string we're sending over 
            msg  = line.encode('ascii', 'ignore')
            sock.sendto(msg, (ai[0][4]))
    #Send an empty string at the end of it to notify the end of file.
    sock.sendto(" ".encode('ascii','ignore'),(ai[0][4]))
    # Close the socket
    sock.close()

if __name__ == '__main__':
    if len (sys.argv) > 3:
        try:
            host = sys.argv[1]
            port = int(sys.argv[2])
            file_name = sys.argv[3]
            client (host, port, file_name)
        except ValueError:
            #print ('Usage: python3 client.py host port file_name')
            raise
    else:
            print ('Usage: python3 client.py host port')
