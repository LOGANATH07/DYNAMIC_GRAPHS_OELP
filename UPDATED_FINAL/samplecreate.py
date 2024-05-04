import random
import sys
file = open("input.txt","w")
for i in range(1000000):
    a = random.randint(0,int(sys.argv[1]))
    b = random.randint(0,int(sys.argv[1]))
    c = str(a)+" "+str(b)+"\n"
    file.write(c)