import csv
import math
import time
import string
import numpy as np
import matplotlib.pyplot as plt

#Part 1
def oddnumbers(n):
    res = []
    for x in range(0, n + 1):
        if (x%2 == 1):
            res.append(x)
    return res

def readFile(name):
    filename = name
    ans = []
    with open(filename, 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        for row in csvreader:
            ans.append(row)
    ans.sort()
    return ans

def readString(string):
    most_freq = {}
    for i in string:
        if i in most_freq:
            most_freq[i] += 1
        else:
            most_freq[i] = 1
    res = max(most_freq, key = most_freq.get)
    return res

#Calculates area of a circle
def useModule(radius):
    return math.pi * math.pow(radius, 2)

#Part 2
def formatrix():
    a = np.random.random((100, 100)) #sets a random 100 by 100 matrix
    b = np.random.random((100, 100))
    c = np.zeros((100, 100))
    start = time.time()
    for i in range(0, 100):
        for j in range(0, 100):
            c[i][j] = a[i][j] + b[i][j]
    end = time.time()
    return(end - start)

def addmatrix():
    a = np.random.random((100, 100)) #sets a random 100 by 100 matrix
    b = np.random.random((100, 100))
    start = time.time()
    a = a + b
    end = time.time()
    return(end - start)
    
def plotgraphs():
    fortimes = []
    addtimes = []
    rangetimes = []
    for i in range(0, 1000):
        fortimes.append(formatrix())
        addtimes.append(addmatrix())
    
    print("Using double for loops: ")
    print("Standard deviation: ", np.std(fortimes), " Average: ", np.mean(fortimes), "\n")
    print("Using add:")
    print("Standard deviation: ", np.std(addtimes), " Average: ", np.mean(addtimes), "\n")
    plt.hist(fortimes)
    plt.show()
    plt.hist(addtimes)
    plt.show()


print(oddnumbers(6), '\n')
print(readFile('students.csv'), '\n')
print(readString("test"), '\n')
print(useModule(3), '\n')
plotgraphs()