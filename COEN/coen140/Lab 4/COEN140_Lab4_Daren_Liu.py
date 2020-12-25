import numpy as np
import pandas as pd
import csv
import math
import random
from random import gauss

nFeatures = 95
nTrain = 1595 #samples
nTest = 399 #samples

def RMSE(pred, actual):
    total = 0.0
    n = len(actual)
    for i in range(n): #samples
        total += ((pred[i] - actual[i][0])**2)
    return math.sqrt(total/n)

def RMSE2(pred1, pred2):
    total = 0.0
    n = len(pred1)
    for i in range(n):
        total += (pred1[i] - pred2[i])**2
    return math.sqrt(total/n)

def problem1(samples):
    samples1 = np.array(samples)
    n = len(samples)
    Y = []
    X = []
    for i in range(n):
        Y.append([samples1[i][0]])
        X.append(np.transpose(samples1[i]))

    Y = np.array(Y)
    X = np.array(X)
    xT = np.transpose(X)
    theta = np.linalg.inv(xT @ X) @ (xT @ Y)
    thetaT = np.transpose(theta)

    results = []
    for i in range(n):
        results.append(thetaT @ X[i])
    results = np.array(results)
    return results

def problem2(samples):
    n = len(samples)
    alpha = 0.00001
    weights = []
    np.random.shuffle(samples)

    for i in range(1, 96):
        weights.append(np.random.normal(0, 1))
    weights = np.array(weights)

    samples1 = samples[0][1:] #features that don't include the response variable
    pred = np.dot(weights, samples1)
    initial = weights + alpha*(samples[0][0] - pred)*samples1

    for i in range(1, n):
        samples1 = samples[i][1:]
        pred = np.dot(initial, samples1)
        initial = initial + alpha*(samples[i][0] - pred)*samples1


    while True:
        np.random.shuffle(samples)
        samples1 = samples[0][1:]
        pred = np.dot(initial, samples1)
        thetas = initial + alpha*(samples[0][0] - pred)*samples1

        for i in range(1, n):
            samples1 = samples[i][1:]
            pred = np.dot(thetas, samples1)
            thetas = thetas + alpha*(samples[i][0] - pred)*samples1

        if np.all((thetas - initial) < 10**-5):
            break
        initial = thetas

    results = []
    for i in range(n):
        results.append(np.dot(thetas, samples[i][1:]))

    results1 = np.array(results)
    return results1


trainData = []
testData = []
with open ('crime-test.txt', 'r') as file:
    reader = csv.reader(file, delimiter = '\t')
    testData = list(reader)[1:nTest]
with open ('crime-train.txt', 'r') as file:
    reader = csv.reader(file, delimiter = '\t')
    trainData = list(reader)[1:nTrain]

testDataf = np.array(testData)
trainDataf = np.array(trainData)
testDataf = testDataf.astype(np.float64)
trainDataf = trainDataf.astype(np.float64)

print("Test RMSE value for normal equation: ")
testData1 = problem1(testDataf)
print(RMSE(testData1, testDataf))
print("Train RMSE value for normal equation: ")
trainData1 = problem1(trainDataf)
print(RMSE(trainData1, trainDataf))

print("Test RMSE value for stochastic gradient descent: ")
testData2 = problem2(testDataf)
print(RMSE(testData2, testDataf))
print("Train RMSE value for stochastic gradient descent: ")
trainData2 = problem2(trainDataf)
print(RMSE(trainData2, trainDataf))

print("RMSE between model vectors of problem 1 and problem 2:")
print("Test data: ")
print(RMSE2(testData1, testData2))
print("Train Data: ")
print(RMSE2(trainData1, trainData2))