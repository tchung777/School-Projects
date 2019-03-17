# Overview

In this lab, we attempt to implement partial TCP by creating a client and server to continiously send and wait for messages to each other. We simulate the stop and protocol by using checksum and sequenceNum variables to tell the client to re-send messages to the server and for the server to check for message correction. We packed our messages in JSON data files to hold various fields for our messages. 

# Sources

In this section, use all the resources you used, people not included

- Arman (because hes awesome)
- stackoverflow for bytearray()

# Questions

Type your answers to the following questions on a new line after each question.

If the current sequence number is 5, what should your sequence number be if the server received the incorrect checksum? What should it be if your server received the correct checksum

If the server receieve the incorrect checksum, we will received the same sequence number again in order to transmit our previous message. If it receive the correct checksum, we will receive 6 as our sequence number.

How could we ensure the packets were delivered to the application in the correct order while allowing the client to not be blocked by a recvfrom call? (No code, just ideas). What sort of protcol could we use?

Well, the sliding window protocol somewhat utilizes that scheme by allowing the client to continiously send stuff within a quanta of time and then seeing if the received message's sequence number lies within our sliding window. If it does, great, keep the message, if not, discard and do all that ACK stuff.

What are some other ways besides checksums to check the correctness of packets?

We could use either bit parity or bit stuffin to add redundant data to the message. When the server receives the data, it can then remove the bit stuff and if it turns out that something is amissed, then we know we have the wrong message and we need to re-send the mssage. For the technical details om how bit stuffing works, refer to the textbook. 


# Extra Credit Completion

Put an X in the following boxes if you completed the extra credits. Please describe your general process for doing this. What sorts of changes did you have to make in running your program?

[] Implement Go Back N Protocol for TCP and asynchronous

# Questions For TA

If you have any questions, just start typing them here. I gave you one to start off with. These arent necessary

1. Is this stuff actually ever used/ is it useful?

2. What kind of activities should I do to improve my knowledge of this field of networks? For Java, I made games. Can I do something Python-related and similar to that to extend my knowledge?

# Comments and Feedback

If you have anything youd like to tell me about, go ahead and write it in here. If its a GIF you want me to use or some thesis on why I suck, thats cool. Just remember that I control your grade for this lab.

That's pretty savage!
