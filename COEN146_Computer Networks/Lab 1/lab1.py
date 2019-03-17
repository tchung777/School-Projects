# Name: Thomas Chung
# Lab 0: Python Tutorial
# Description: This program implements two funtions multiplex() and demultiplex(). multiplex() takes in an tuple of tuple and assigns
# each tuple in the tuple a machine number. It goes through and take 5 elements from each tuple until there are no elements left to
# take and writes it to a list which is written to a file. demultiplex() does the opposite of multiplex() and reads from a file. It
# reads the file line by line and use split() multiple times to grab the machine number and the actual word itself. It then appends
# the word to the appropriate list with the corresponding machine number.

#The following function uses Time Division Multiplexing to implement multiplexing.
def multiplex(messages):
    notDone = True
    
    #outputList is a variable we use to hold our final output.
    outputList = []
    
    #The cycle variable corresponds to the number of times we advanced through a tuple.
    cycle = 0;
    
    #While there are still elements left in the tuples to examine, notDone will equal True.
    while notDone:
        
        #We will loop through all the tuples of 'messages'
        for i in range(len(messages)):
            
            #The machineNumber is determined by the index of the tuple we're examining.
            machineNum = 60400 + i
            
            #Because we're implementing Time Division Multiplexing, we ask each machine five times for a message.    
            for x in range(5):
                
                #If I exceeded the length of the tuple, then the tuple doesn't have a message for me, otherwise:
                if x+cycle > len(messages[i])-1:
                    #print("{}:{}".format(machineNum,"No messages"))
                    
                    #Append to outputList "No messages" if there isn't a message.
                    outputList.append("{}:{}\n".format(machineNum,"No messages"))
                    notDone = False
                else:
                    #print("{}:{}".format(machineNum,messages[i][x+cycle]))
                    
                    #Append to outputList if there is a message.
                    outputList.append("{}:{}\n".format(machineNum,messages[i][x+cycle]))
                    notDone = True
        
        #Increase cycle by 5 since in Time Division Multiplexing, we advanced the tuples by 5.
        cycle+=5
        
    #Finally, write output to file.
    with open('muxed_stream','w') as f:
        for text in outputList:
            f.write(text)
        
           
            
#The following function uses Frequency Division Demultiplexing to demultiplex messages from an input file. 
def demultiplex(input_file='muxed_stream'):
    
    #Open the file called 'muxed_stream'.
    with open('muxed_stream','r') as f:
        
        #Create an empty list to hold all of our 'machines'.
        outputList = [[],[],[],[],[]]
        
        #Our maxIDX+1 variable will hold the number of lists in our outputList.
        maxIDX = 4
        
        #Loop through every line.
        for line in f:
            #Use split to split whatever string is found at line by the keyword '604'.
            iSplit = line.split("604")[1].split(":")
            
            #The word at index 0 of the returned list from split() will hold the machine number/index number.
            index = int(iSplit[0])
            
            #If it's greater than our maxIDX, then it belongs in the last machine.
            if index >= maxIDX:
                index = 4;
                
            #We need to do some clean up here so we split the word found from the returned list of the previous split()
            #and remove \n. We should now have our final message.
            moreSplit = iSplit[1].split("\n")
            
            #If our final message is 'No messages' then do nothing. Otherwise, add it to the appropriate machine.
            if moreSplit[0] == 'No messages':
                continue
            else:
                outputList[index].append(moreSplit[0])
        #Turn our outputList to a tuple.
        outputList = tuple(outputList)
        
        return outputList
            

## YOU DON'T NEED TO EDIT ANYTHING BELOW HERE

def test_case_1():
    """Testing Multiplexing with tons of messages."""

    # we have to keep the type immutable and then create a copy or else pass by argument will
    # mutate the variable
    messages = (
        ('1', '2', '3', '4', '5', '6', '7', '8', '9'),
        ('1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17'),
        ('1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20'),
        ('1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16'),
        ('1', '2', '3', '4'),
        ('1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20')
    )

    # this will get consumed if we pass as list of lists so make a copy
    messages_copy = [list(msgs) for msgs in messages]

    multiplex(messages_copy)
    messages_received = demultiplex('muxed_stream')

    assert len(messages_received[0]) == 9
    assert len(messages_received[1]) == 17
    assert len(messages_received[2]) == 20
    assert len(messages_received[3]) == 16
    assert len(messages_received[4]) == 24

def test_case_2():
    """Testing multiplexing with tons of lost messages."""

    # we have to keep the type immutable and then create a copy or else pass by argument will
    # mutate the variable
    messages = (
        ("This ain't", "no intro,", " this the entree"),
        (),
        (),
        (),
        ("Hit that", "intro with Kanye ", "and sound like Andre"),
        ("Tryna", "turn", "my", "baby", "mama", "to", "my", "fiancee"),
        ("She like"," music, she from", "Houston",  "like Auntie Yonce"),
        ("Man my daughter couldn't", "have a better mother"),
        ("If she ever", "find another, he", "better", "love", "her")
    )

    # this will get consumed if we pass as list of lists so make a copy
    messages_copy = [list(msgs) for msgs in messages]

    multiplex(messages_copy)
    messages_received = demultiplex('muxed_stream')

    assert len(messages_received[0]) == 3
    assert len(messages_received[1]) == 0
    assert len(messages_received[2]) == 0
    assert len(messages_received[3]) == 0
    assert len(messages_received[4]) == 22


if __name__ == '__main__':
    #test_case_1()
    test_case_2()
