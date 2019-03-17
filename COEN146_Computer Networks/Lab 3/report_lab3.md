# Overview

In this lab, we created a client and server program. The client program sends a file to the server program which opens and reads the file. It then copies the file. 

# Sources

In this section, use all the resources you used, people not included

- Arman (because he is awesome)
- https://docs.python.org/3.5/library/socket.html
- https://docs.python.org/2/howto/sockets.html
- http://searchmicroservices.techtarget.com/definition/UDP-User-Datagram-Protocol
- https://www.tutorialspoint.com/python/python_networking.htm

# Questions

Type your answers to the following questions on a new line after each question.

1. Describe UDP in your own words.

UDP is the inferior protocol for data transmission. It allows the user to input a port number and uses checksum to verify data. In UDP packets are sent in chunks.

2. What does the bind function do? Do we need it on the client?

The bind() binds the socket we created to the IP address we inputted. As Arman said, "it connects a socket object to an interface on your system." No we do not need it for the client .

3. In your own words, describe what a socket is.

Sockets are endpoints on a communication channel. They're used in transporting data across a network.


4. What are the Address, Family, Host, and Port variables used for in creating a socket

The address variable serves as the identifier of the network. The family variable dictates which protocol the socket will used  (e.g: AF_INET and etc..). The Host variable is a string that represents the hostname. And finally the port number is an another identifier used to tell the socket what process to follow to send data to the server.

# Extra Credit Completion

Put an X in the following boxes if you completed the extra credits. Please describe your general process for doing this. What sorts of changes did you have to make in running your program?

[] Transfer a text file to your neighbor
[] Transfer an image file to your TA

# Questions about the class

1. Do you feel like you have enough Python knowledge to complete this lab?

I feel like there could have been a more methodical way to complete this lab. Perhaps maybe if COEN 146 could be more analytical about the structures of sockets and what not.

2. Do you actually understand what we did in this lab?

I suppose to some degree although I'm still a bit iffy about why I have to pass in an a 2d array to sock.sendot()

3. Do the templates and stuff help? How could they be improved (No I will not just give you all the correct code)

Yes, the templates definitily helped. As to how they could be improved...maybe also have resources to look up information whereever you commented in the template?
