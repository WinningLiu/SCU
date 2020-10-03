import numpy as np
import pandas as pd
from sklearn import linear_model

df = pd.read_csv(
    filepath_or_buffer='http://archive.ics.uci.edu/ml/machine-learning-databases/iris/iris.data', 
    header=None)
data = df.iloc[:,:].values
data = data[data[:,4] == "Iris-setosa"][:,:4]

def model(X, Y, regressor="X", response="Y"):
    X = X.reshape(-1, 1)
    Y = Y.reshape(-1, 1)
    regr = linear_model.LinearRegression()
    regr.fit(X, Y)

    slope = regr.coef_
    intercept = regr.intercept_

    print("%s = %f + %f x %s" %(response, intercept, slope, regressor))
    Yp = np.squeeze(regr.predict(X))
    sse = np.sum((Yp - Y) ** 2)
    print("Sum of squared errors: %f" % sse)
    # Explained variance score: 1 is perfect prediction
    print('Variance score: %f\n' % regr.score(X, Y))
    return slope, intercept, sse, response, regressor

def main():
    names = ['slength', 'swidth', 'plength', 'pwidth']
    min = 10000000
    for x in names:
        for y in names:
            if x != y:
                tup = model(data[names.index(x)], data[names.index(y)], regressor=x, response=y)
                if tup[2] < min:
                    slope = tup[0]
                    intercept = tup[2]
                    min = tup[2]
                    response = tup[3]
                    regressor = tup[4]

    print("Best model: %s = %f + %f x %s" %(response, intercept, slope, regressor))
    print("Minimum SSE: ", min)
    

if __name__ == '__main__':
    main()