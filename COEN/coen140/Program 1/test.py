import pandas as pd
import csv
import statistics
from statistics import mode

test = pd.read_csv('predictions.dat', sep = '\n', header = None)

print(test[:20])