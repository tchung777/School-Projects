import argparse
import random
import math


def generate_file(file_name):
    """
    Here's my docstring for my generate file. If this doesn't get filled in I get - 5.
    
    Positional Arguments: 
        file_name is the name of the file, 
        'w' is for writing, 
        encoding is 'utf-8' because we're using utf-8 encoding.
   
    Side Effects: None!
    """
    random.seed()
    with open(file_name, 'w',encoding='utf-8') as f:
        for x in range(20000):
            #Write a random character to the file.
            f.write("{}".format(chr(random.randint(0,256))))
    print ('Generate file: ', file_name)




def calculate_entropy(file_name):
    """
    Here's my docstring for my calculate entropy. If this doesn't get filled in I get - 5.
    
    Positional Arguments:
        file_name is the name of the file
        'r' is for reading
        encoding is utf-8 because we're using utf-8 encoding
   
    Side Effects: 
    """

    array = [0] * 257
    numOfChar = 0
    with open(file_name,'r',encoding='utf-8') as f:
        #Continue to read one character from the file.
        while True:
            c = f.read(1)
            if not c:
                print("EOF")
                break
            #Increment the index that corresponds to its ASCII value.
            array[ord(c)]+=1
            #Total number of characters increase.
            numOfChar+=1
    entropy = 0
    for x in range(257):
        #Goes through entire array of ASCII 
        if array[x]:
            #Probability is determined by the number of occurences of that character over the total number of characters.
            prob = array[x] / numOfChar
            #Entropy is the sum of these probability multiplied by the logarithm of that probability.
            entropy += (prob * math.log2(prob))
    #According to Shanon's Entropy Formula, the Entropy is the negative of the sum[prob*log(prob)]
    entropy = -entropy
    print ('Calculate Entropy: ', entropy)







## DO NOT NEED TO EDIT ANYTHING UNDER HERE
# setup command line options
parser = argparse.ArgumentParser(description='Generate a random file and calculate its Shannon Entropy')
parser.add_argument('-e', '--execute', dest='fxn', help='Function to be executed: calcaulte, generate, main')
parser.add_argument('-f', '--file', default='lab6.txt', dest='file_name', help='File to either calculate entropy or generate into')

args = parser.parse_args()

if args.fxn == 'generate':
    generate_file(args.file_name)
elif args.fxn == 'calculate':
    calculate_entropy(args.file_name)
elif args.fxn == 'main':
    generate_file(args.file_name)
    calculate_entropy(args.file_name)
else:
    parser.print_help()

