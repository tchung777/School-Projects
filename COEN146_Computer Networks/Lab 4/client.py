import socket
import sys

#Pre-condition: The network is up
#Post-condition: A client is set up and ready to start sending messages.
def client(host, port):        
    # Resolve the IP Address given the hostname and the port number
    try:
        ai = socket.getaddrinfo(host,port)
    except socket.gaierror: 
        # insert error handling code because we couldn't resolve host name
        print("We had an error.")

    # Create a socket object with the proper socket specifications.
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

      
    msg = ""
    
    #While the first four letters of our message (with the exception of :: or ;; keywords) isn't quit, keep the client going
    while msg[2:6].lower() != 'quit':
        
        #Send an encoded msg string
        msg = input()
        sock.sendto(msg.encode('ascii','ignore'),(ai[0][4]))
    # Close the socket
    sock.close()

if __name__ == '__main__':
    if len (sys.argv) > 2:
        try:
            host = sys.argv[1]
            port = int(sys.argv[2])
            client (host, port)
        except ValueError:
            #print ('Usage: python3 client.py host port file_name')
            raise
    else:
            print ('Usage: python3 client.py host port')
