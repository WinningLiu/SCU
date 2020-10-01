c = 5
d = 10
from module import adder #Copy out an attribute
result = adder(c, d) #No need to qualify name
print(result)

from module import a, b, multiplier #Copy out multiple attributes
result = multiplier(a, b)
print(result)