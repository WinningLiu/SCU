def intersect(seq1, seq2):
    res = [] #Start empty
    for x in seq1: #Scan seq1
        if x in seq2: #Common item?
            res.append(x) #Add to end
    return res

s1 = "SPAM"
s2 = "SCAM"
result1 = intersect(s1, s2)
print(result1)

result2 = intersect([1,2,3], (1, 4)) #mixed type: list & tuple
print(result2)