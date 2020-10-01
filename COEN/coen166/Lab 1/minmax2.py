def minmax(test,array):
    res = array[0]
    for arg in array[1:]:
        if test(arg, res):
            res = arg
    return res

def lessthan(x,y): 
    return x<y
def grtrthan(x,y): 
    return x>y

if __name__ == '__main__':
    print(minmax(lessthan, [4,2,1,5,6,3])) # self-test code
    print(minmax(grtrthan, [4,2,1,5,6,3]))