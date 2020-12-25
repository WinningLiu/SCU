import numpy as np
import pandas as pd
import scipy.sparse as sp
from numpy.linalg import norm
from collections import Counter, defaultdict
from scipy.sparse import csr_matrix

from sklearn.svm import LinearSVC
from sklearn.feature_selection import SelectFromModel

#K nearest neighbors
from sklearn.neighbors import KNeighborsClassifier
from sklearn.pipeline import Pipeline

#naive bayes
from sklearn.naive_bayes import GaussianNB

def build_matrix(docs):
    """ Build sparse matrix from a list of documents, 
    each of which is a list of word/terms in the document.  
    Returns both matrix and idx
    """
    nrows = len(docs)
    idx = {}
    tid = 0
    nnz = 0
    for d in docs:
        nnz += len(set(d))
        for w in d:
            if w not in idx:
                idx[w] = tid
                tid += 1
    ncols = len(idx)
        
    # set up memory
    ind = np.zeros(nnz, dtype=np.int)
    val = np.zeros(nnz, dtype=np.double)
    ptr = np.zeros(nrows+1, dtype=np.int)
    i = 0  # document ID / row counter
    n = 0  # non-zero counter
    # transfer values
    for d in docs:
        cnt = Counter(d)
        keys = list(k for k,_ in cnt.most_common())
        l = len(keys)
        for j,k in enumerate(keys):
            ind[j+n] = idx[k]
            val[j+n] = cnt[k]
        ptr[i+1] = ptr[i] + l
        n += l
        i += 1
            
    mat = csr_matrix((val, ind, ptr), shape=(nrows, ncols), dtype=np.double)
    mat.sort_indices()
    
    return mat, idx

def build_matrix2(docs,idx):
    """ Build sparse matrix from a list of documents, 
    each of which is a list of word/terms in the document.  
    Returns only matrix
    """
    nrows = len(docs)
#     idx = {}
    tid = 0
    nnz = 0
    for d in docs:
        nnz += len(set(d))
#         for w in d:
#             if w not in idx:
#                 idx[w] = tid
#                 tid += 1
    ncols = len(idx)
        
    # set up memory
    ind = np.zeros(nnz, dtype=np.int)
    val = np.zeros(nnz, dtype=np.double)
    ptr = np.zeros(nrows+1, dtype=np.int)
    i = 0  # document ID / row counter
    n = 0  # non-zero counter
    # transfer values
    for d in docs:
        cnt = Counter(d)
        keys = list(k for k,_ in cnt.most_common())
        l = len(keys)
        for j,k in enumerate(keys):
            temp = idx.get(k,-1)
            if temp != -1:
                ind[j+n] = temp
                val[j+n] = cnt[k]
        ptr[i+1] = ptr[i] + l
        n += l
        i += 1
            
    mat = csr_matrix((val, ind, ptr), shape=(nrows, ncols), dtype=np.double)
    mat.sort_indices()
    
    return mat

def csr_info(mat, name="", non_empy=False):
    """ Print out info about this CSR matrix. If non_empy, 
    report number of non-empty rows and cols as well
    """
    if non_empy:
        print("%s [nrows %d (%d non-empty), ncols %d (%d non-empty), nnz %d]" % (
                name, mat.shape[0], 
                sum(1 if mat.indptr[i+1] > mat.indptr[i] else 0 
                for i in range(mat.shape[0])), 
                mat.shape[1], len(np.unique(mat.indices)), 
                len(mat.data)))
    else:
        print( "%s [nrows %d, ncols %d, nnz %d]" % (name, 
                mat.shape[0], mat.shape[1], len(mat.data)) )

def csr_l2normalize(mat, copy=False, **kargs):
    """ Normalize the rows of a CSR matrix by their L-2 norm. 
    If copy is True, returns a copy of the normalized matrix.
    """
    if copy is True:
        mat = mat.copy()
    nrows = mat.shape[0]
    nnz = mat.nnz
    ind, val, ptr = mat.indices, mat.data, mat.indptr
    # normalize
    for i in range(nrows):
        rsum = 0.0    
        for j in range(ptr[i], ptr[i+1]):
            rsum += val[j]**2
        if rsum == 0.0:
            continue  # do not normalize empty rows
        rsum = 1.0/np.sqrt(rsum)
        for j in range(ptr[i], ptr[i+1]):
            val[j] *= rsum
            
    if copy is True:
        return mat

def filterLen(docs, minlen):
    """ filter out terms that are too short. 
    docs is a list of lists, each inner list is a document represented as a list of words
    minlen is the minimum length of the word to keep
    """
    return [ [t for t in d if len(t) >= minlen ] for d in docs ]

def csr_idf(mat, copy=False, **kargs):
    """ Scale a CSR matrix by idf. 
    Returns scaling factors as dict. If copy is True, 
    returns scaled matrix and scaling factors.
    """
    if copy is True:
        mat = mat.copy()
    nrows = mat.shape[0]
    nnz = mat.nnz
    ind, val, ptr = mat.indices, mat.data, mat.indptr
    # document frequency
    df = defaultdict(int)
    for i in ind:
        df[i] += 1
    # inverse document frequency
    for k,v in df.items():
        df[k] = np.log(nrows / float(v))  ## df turns to idf - reusing memory
    # scale by idf
    for i in range(0, nnz):
        val[i] *= df[ind[i]]
        
    return df if copy is False else mat


df = pd.read_csv("pr2/train.dat", sep = '\t', header = None)
tdf = pd.read_csv("pr2/test.dat", sep = '\t', header = None)

vals = df.iloc[:,:].values
testVals = tdf.iloc[:,:].values

names = []
testNames = []

cls = []
for i in range(len(vals)):
    cls.append(vals[i][0])
    names.append(vals[i][1])

for i in range(len(testVals)):
    testNames.append(testVals[i][0])
    
testCls = [int(i) for i in cls]

testCls = np.asarray(testCls)
names = np.asarray(names)
testNames = np.asarray(testNames)

train_matrix, idx = build_matrix(names)
csr_info(train_matrix)
test_matrix = build_matrix2(testNames, idx)
csr_info(test_matrix)

train_matrix2 = csr_l2normalize(train_matrix, copy = True)
test_matrix2 = csr_l2normalize(test_matrix, copy = True)

#0.8381
"""knn = KNeighborsClassifier(n_neighbors=6)
clf = knn.fit(train_matrix2, testCls)
predicted = clf.predict(test_matrix2)"""

"""gnb = GaussianNB()
predicted = gnb.fit(train_matrix2, testCls).predict(test_matrix2)"""

pred_file = open('predictions.dat', 'w+')
predicted_df = pd.DataFrame(predicted)
predicted_df.to_csv('predictions.dat', index = False, header = None)