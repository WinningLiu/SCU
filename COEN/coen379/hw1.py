import math, random

# Problem 1
# def isPointInShape(x, y):
#     tempx = (1-(y**(2/3)))**(3/2)
#     tempy = (1-(x**(2/3)))**(3/2)

#     if (x <= tempx and y <= tempy):
#         return True
#     return False
 
# def approximateArea(radius, numberOfPoints):
#     pointsInside = 0
#     for i in range(numberOfPoints):
#         x = random.uniform(0, radius)
#         y = random.uniform(0, radius)
#         if (isPointInShape(x, y)):
#             pointsInside = pointsInside + 1

#     return (pointsInside / numberOfPoints) * 4

# print(approximateArea(1, 1000000))

# Problem 2
def rfind(l, target):
    n = len(l)
    visited = [False] * n
    unique = 0
    k = 0

    while unique < n:
        i = random.randint(0, len(visited) - 1)
        if (visited[i] == False):
            if (l[i] == target):
                print(k)
                return unique + 1
            visited[i] = True
            unique += 1
        k += 1

    return unique

lst = []
for i in range(0, 100):
    lst.append(i)

average = 0
for i in range(0, 10000):
    average += rfind(lst, 50)

print(average/10000)