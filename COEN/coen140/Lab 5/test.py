import numpy as np
import pandas as pd
import math


# load and store datasets from website
train = pd.read_csv('spambase/spam-train', header=None)
test = pd.read_csv('spambase/spam-test', header=None)
print("Training dataset:")
train.head()

# store expected outcomes (last column) into y_train; everything else for prediction goes into X_train
X_train = train.drop(57,axis=1)
y_train = train[57]

# check to make sure we have the correct amount of communities and features
print("X_train shape:", X_train.shape)
print("y_train shape:", y_train.shape)

# now do the same thing but for the testing data
X_test = test.drop(57,axis=1)
y_test = test[57]

print("X_test shape:", X_test.shape)
print("y_test shape:", y_test.shape)

# this function takes in a matrix and standardizes it to obtain a mean of 0 and unit variance
def standardize(matrix):
    standard = (matrix - matrix.mean(axis=0)) / matrix.std(axis=0)
    return standard

    # necessary to compute sigmoid for logistic function
def sigmoid(r):
    return 1 / (1 + math.exp(-r))

# condense our numpy arrays into one when we use this function
v_sigmoid = np.vectorize(sigmoid)

alpha = 5*(10e-5)
threshold = 10e-5

class logistic_gradient_descent:
    
    # this function is used to determine our max w through gradient descent for logistic regression
    def fit(self, X_train, y_train):
        X = X_train.copy()
        
        # append a column of ones to the end of the training attributes
        X['bias'] = np.ones(X.shape[0])
        
        # generate a Gaussian (normal) distribution for our initial value, based off how many features we have
        w0 = np.random.normal(0,1,size=X.shape[1])
        
        epsilon = 1
        # find the max value (w0) with our given cost function
        while epsilon > threshold:
            w1 = w0 - alpha*(np.dot((v_sigmoid(np.dot(X,w0)) - y_train),X))
            epsilon = np.linalg.norm(w1 - w0) # euclidian distance
            w0 = w1
            
        # assign the max value to our class instance
        self.w0 = w0
        return self
    
    # calculate our predictions by calculating the sigmoid of our max w and chosen matrix
    def predict(self,matrix):
        X = matrix.copy()
        
        # append a column of ones to the end of the testing attributes
        X['bias'] = np.ones(X.shape[0])
        
        # sigmoid to squash outputs to [0,1]
        return v_sigmoid(np.dot(self.w0,X.T))

#take raw predictions and round to 0 or 1
def round_predictions(prediction_list):
    rounded_predictions = []
    for item in prediction_list:
        # predictions are rounded since 0 = non-spam and 1 = spam
        if item < 0.5:
            rounded_predictions.append(0)
        else:
            rounded_predictions.append(1)
    return rounded_predictions

    # Calculate the accuracy by comparing predictions vs actual values
def calculate_accuracy(actual, prediction):
    n_correct = 0
    total = len(y_test)
    for i in range(total):
        if prediction[i] == actual[i]:
            n_correct += 1
    accuracy = (n_correct/total) * 100
    error = 100 - accuracy
    return str(error)

logistic_regression = logistic_gradient_descent()
logistic_regression.fit(standardize(X_train),y_train)

train_predictions = round_predictions(logistic_regression.predict(standardize(X_train)))
print('Training error with logistic regression and gradient descent: ' + calculate_accuracy(y_train, train_predictions) + "%")

test_predictions = round_predictions(logistic_regression.predict(standardize(X_test)))
print('Testing error with logistic regression and gradient descent: ' + calculate_accuracy(y_test, test_predictions) + "%")