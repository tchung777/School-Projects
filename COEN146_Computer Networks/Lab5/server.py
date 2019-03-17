import socket
import sys
import random
import json

#Pre-condition: Nothing
#Post-condition: This program will create a server program that will communicate with a client program on the same port. They will continue to exchange messages back and forth until the client program sends in the message FIN, in which case, both the server program will end themselves on their own sides.
def server(port):        
   
    # Create a socket object given the proper socket specifications.
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Bind the socket object to your localhost and port
    sock.bind(('localhost',6903))

    # Receive a filename and open a file
    
    while True:
        #Receive data, decode and unload it from its json object
        data, address = sock.recvfrom(1024)
        string = data.decode('ascii','ignore')
        packet = json.loads(string)
        #Preparations end

        #Partitioning the dictionary to get the appropriate attributes
        msgType = packet["type"]
        checkSum = packet["checksum"]
        sequenceNum = packet["sequence"]

        #Print the message
        print('{}:{}'.format(sequenceNum,packet["message"]))
        
        #Kill yourself if you get the FIN msgType
        if msgType == 'FIN':
            break

        #Calculate checksum on the server side as well, see if it matches.
        #If not, ask the client to send again.
        newCheckSum = 0

        #Introduce some error for the sake of showing that checkSum works.
        if random.random() < 0.1:
            newCheckSum = -1
        else:
            newCheckSum = sum(bytearray(packet["message"],'utf-8'))

        #If the checkSum works, increment it up.
        if checkSum == newCheckSum:
            sequenceNum+=1
        
        #Send a packet back
        packet = {"type":"ACK", "checksum":newCheckSum, "message":"", "sequence":sequenceNum}
        string = json.dumps(packet)
        sock.sendto(string.encode('ascii','ignore'),address)


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
