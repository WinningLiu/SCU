import pandas as pd

df = pd.read_csv("pr2/train.dat", sep = '\t', header = None)
tdf = pd.read_csv("pr2/test.dat", sep = '\t', header = None)
pred = pd.read_csv("predictions.dat", sep = '\t', header = None)

print(tdf.shape)
print(pred.shape)