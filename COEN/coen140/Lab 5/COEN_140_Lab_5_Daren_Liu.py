import numpy as np
import pandas as pd
import math

def normalize(mat):
    result = (mat - mat.mean(axis=0)) / mat.std(axis=0)
    return result

def sigmoid(z):
    return 1 / (1 + math.exp(-z))

def accuracy(actual, pred):
    count = 0
    total = len(y_test)
    for i in range(total):
        if pred[i] == actual[i]:
            count += 1
    accuracy = (count/total) * 100
    error = 100 - accuracy
    return str(error)

def round_pred(pred):
    results = []
    for item in pred:
        if item < 0.5:
            results.append(0)
        else:
            results.append(1)
    return results

condense = np.vectorize(sigmoid)

alpha = 5*(10e-5)

class logistic_gradient_descent:
    
    def fit(self, X_initial, y):
        X = X_initial.copy()
        
        X['bias'] = np.ones(X.shape[0])
        
        weights = np.random.normal(0,1,size=X.shape[1])
        
        e = 1
        while e > 10e-5:
            weights1 = weights - alpha*(np.dot((condense(np.dot(X,weights)) - y),X))
            e = np.linalg.norm(weights1 - weights) 
            weights = weights1
            
        self.weights = weights
        return self

    def predict(self,mat):
        X = mat.copy()
        
        X['bias'] = np.ones(X.shape[0])
        
        return condense(np.dot(self.weights,X.T))


train = pd.read_csv('spambase/spam-train', header=None)
test = pd.read_csv('spambase/spam-test', header=None)

X_train = train.drop(57,axis=1)
y_train = train[57]

X_test = test.drop(57,axis=1)
y_test = test[57]

logistic_regression = logistic_gradient_descent()
logistic_regression.fit(normalize(X_train),y_train)

train_pred = round_pred(logistic_regression.predict(normalize(X_train)))
print('Training error with logistic regression and gradient descent: ' + accuracy(y_train, train_pred) + '%')

test_pred = round_pred(logistic_regression.predict(normalize(X_test)))
print('Testing error with logistic regression and gradient descent: ' + accuracy(y_test, test_pred) + '%')