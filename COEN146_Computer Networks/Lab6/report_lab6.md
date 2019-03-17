# Overview

In this lab, we implemented Shannon's Expression for Entropy and created a program that would randomly generate a random file filled with garbage. Then we create a Shannon's Entropy Calculator to calculate entropy in the junk file.

# Sources

In this section, use all the resources you used, people not included

- Arman (because hes awesome)
- stackoverflow ....
- http://stackoverflow.com/questions/2988211/how-to-read-a-single-character-at-a-time-from-a-file-in-python
- http://stackoverflow.com/questions/2422461/how-do-i-create-a-list-with-256-elements

# Methodology for generating random file

Open up a file with utf-8 encoding and writte 256 random characters to it.


# Questions

What sort of file will have higher entropy a normal text file or an encrypted text file?

    A encrtyped text file will have higher entropy because entropy is defined as lack of information and when you encrytp a file, you are hiding information away so that only someone who understands it will be able to read it.

UTF-8 another character encoding scheme takes 8 bits per character. What is the maximum achievable entropy for a file using all characters in UTF-8? How about for UTF-16?
    
    The maximum achievable entropy is 8 for a file using all characters in UTF-8. The maximum achievable entropy is 16 for a file using all characters in UTF-16.

Did you enjoy this lab? Was it helpful overall in your understanding of networks? 
    
    Yes this lab was very enjoyable because it was easy. And yes.



# Extra Credit Completion

Put an X in the following boxes if you completed the extra credits. Please describe your general process for doing this. What sorts of changes did you have to make in running your program?

[] Implement generate_file in less than 8 lines, always achieving perfect entropy (of 8). 



# Comments and Feedback

