import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from sklearn import linear_model

# Get data
df = pd.read_csv(
    filepath_or_buffer='data/trucks.csv', 
    header=None)
data = df.iloc[:,:].values
X = data[:,0].reshape(-1, 1)
Y = data[:,1].reshape(-1, 1)

#linear_model.LinearRegression
# Train the model using the training set only, then extract the slope and intercept
# Hint: look up the manual for linear_model.LinearRegression
regr = linear_model.LinearRegression()
regr.fit(X, Y)

slope = regr.coef_
intercept = regr.intercept_

print("y = %f + %fx" %(intercept, slope))
print("Mean squared error: %f"
      % np.mean((regr.predict(X) - Y) ** 2))
# Explained variance score: 1 is perfect prediction
print('Variance score: %f' % regr.score(X, Y))

# Create a plot to show the XY points (in black) as well as the prediction line (in red)
plt.scatter(X, Y, color = "black")
plt.plot(X, regr.predict(X), color = "red", linewidth=3)

plt.show()