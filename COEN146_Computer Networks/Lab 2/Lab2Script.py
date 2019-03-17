import subprocess
import sys

def shell():
    with open('output.txt','w') as f:
        for stepSize in range(11):
           # print('Step Size: {}\n'.format(stepSize*100),file=f)
            for x in range(8):
                subprocess.run('time cat ~/146_lab2.txt|head -c {}|ssh -q  cchung@linux.scudc.scu.edu "(cat -> ~/COEN146/test3)"'.format(stepSize*10**8),shell=True,stderr=f)



def parseFile():
    with open('output.txt','r') as f:
        times =[]
        i = 0
        time = 0
        for line in f:
            if i % 32 is 0:
                times.append(time/8)
                time = 0
            else:
                if i % 4 == 2:
                    iSplit = line.split('0m')[1].strip()[:-1]
                    time += float(iSplit)
                if i % 4 == 3:
                    iSplit = line.split('0m')[1].strip()[:-1]
                    time += float(iSplit)
            i+=1
    print(times)

shell()
parseFile()
