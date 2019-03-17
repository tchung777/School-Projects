import socket
import sys

#Pre-condition: Network is up
#Post-condition: A server has been setup that is ready to receive messages.
def server(port):        
    users ={}

    # Create a socket object given the proper socket specifications.
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Bind the socket object to your localhost and port
    sock.bind(('linux60812',port))
    
    # Server will keep running until there are zero users
    while True:
        data, address = sock.recvfrom(1024)
    
        msg = data.decode('ascii','ignore')
        
        #Users have to input '::' first in order to register a username
        if msg[0:2] == '::':
            
            #Store this in the dictionary of users with the appropriate key as the address
            users[address[0]] = msg[2:]
        #Users have to input ';;' to type in a message
        elif msg[0:2] == ';;':
            
            #If we can find the users in the user dictionary
            if address[0] in users:
                
                #Print the message
                print("{}:{}".format(users[address[0]],msg[2:]))
                
                #If the message happens to be quit, then erase the user from the user dictionary
                if msg[2:6].lower() == 'quit':
                    print('{} quitted'.format(address[0]))
                    del users[address[0]]
                    
                    #If there are no more users, break this while-loop
                    if len(users) == 0:
                        break
            #Otherwise, print the message
            else:
                print("{}:{}".format(address[0],msg[2:]))

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
