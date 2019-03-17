import socket
import sys
import json

#Pre-condition: Nothing
#Post-condition: Creates a client program that continues to send message to a server program on the same port. They will continue to exchange messages until the client types in FIN in which that will be the final message and it'll kill the program.
def client(host, port):        
    # Resolve the IP Address given the hostname and the port number
    try:
        ai = socket.getaddrinfo(host,port)
    except socket.gaierror: 
        # insert error handling code because we couldn't resolve host name
        print("We had an error.")

    # Create a socket object with the proper socket specifications.
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # My variables: msgType, the checksum, message, and th sequence number
    string=""
    loop = True
    sequenceNum = 1
    msgType = ""
    
    #While we haven't ended yet.
    while msgType != "FIN":
        # client sends a Python dictionary encoded as a JSON String
        msg = input()
        if msg == 'FIN':
            msgType = 'FIN'
        #For every message, checkSum will be different
        checkSum = sum(bytearray(msg,'utf-8'))
        packet = {"type":msgType, "checksum":checkSum, "message":msg, "sequence":sequenceNum}
        string = json.dumps(packet)
        
        oldSequenceNum = sequenceNum
        sock.sendto(string.encode('ascii','ignore'),(ai[0][4]))
        
        #Client kills itself if client inputs FIN
        if msgType == 'FIN':
            break

        #End Client Sending Code
        
        #Client waits for a response
        while sequenceNum == oldSequenceNum:
            data, address = sock.recvfrom(1024) #sock.recvfrom waits before proceeding
        

            data = data.decode('ascii','ignore')

            rPacket = json.loads(data)

            sequenceNum = rPacket["sequence"]
            
            #If we get the same sequence number back, we must re-send it and await for a new response.
            if sequenceNum == oldSequenceNum:
                sock.sendto(string.encode('ascii','ignore'),(ai[0][4]))
        

    # Close the socket
    sock.close()

if __name__ == '__main__':
    if len (sys.argv) > 1:
        try:
            host = sys.argv[1]
            port = int(sys.argv[2])
            client (host, port)
        except ValueError:
            #print ('Usage: python3 client.py host port file_name')
            raise
    else:
            print ('Usage: python3 client.py host port')
