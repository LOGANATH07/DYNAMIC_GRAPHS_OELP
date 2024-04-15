import random
file = open("input.txt","w")
for i in range(10000):
    a = random.randint(0,4038)
    b = random.randint(0,4038)
    c = str(a)+" "+str(b)+"\n"
    file.write(c)