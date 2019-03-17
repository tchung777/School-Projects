# Overview

In this lab, we created a client and server program where multiple client programs can log onto the server program, choose a username for themselves and send messages to the server. Essentially it acts as a chatroom.

# Sources

In this section, use all the resources you used, people not included

- Arman (because he is awesome)

# Questions

Type your answers to the following questions on a new line after each question.

1. What does it mean that recvfrom() is a blocking function?

It means that recvfrom() will go to sleep if the process that issued it cannot be completed immediately and will wake up when a packet arrives that allows it to finish its task.

2. How would you implement a program that allows both the client and server to send and receive messages? So first the client would send, the server would receive. Then the server would send and the client would receive and so on. Don't need to actually implement, pseudocode is fine.

Lab 5 in a nutshell(As I write this, Lab 5 has already occured.) We will maintain the while-loop we had in client and after it finishes sending a message, we will use recvfrom() to hold a loop until a message comes from the server. On the server side, it is the reverse. The server will always listen for a packet to come in before it sends a message of its own. 

3. How does getaddrinfo() or gethostbyname() resolve the correct address? Hint: What Linux file are hosts stored in?

Linux has a /etc/hosts directory which contains a list of hosts. gethostbyname() will set the h_name field in hosting to localhost (because we have specified that we are using IPv4 address). 

4. What is the loopback address?

The loopback address is a special IP address that allows people to test whether the method of communcation is working via sending packets down the network software stack and back up again as if it had been received from another device.

5. How can you tell if the server has closed its connection?

By calling recvfrom(). If it returns 0, then the connection has closed.
