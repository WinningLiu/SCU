import numpy as np
import pandas as pd
import scipy.sparse as sp
from numpy.linalg import norm
from collections import Counter, defaultdict
from scipy.sparse import csr_matrix
import statistics
from statistics import mode

#Last two libraries and functions were not used
from sklearn.neighbors import KNeighborsClassifier
from sklearn.pipeline import Pipeline

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


#Data preprocessing
df = pd.read_csv('train.dat', sep = '\n', header = None)
tdf = pd.read_csv('test.dat', sep = '\n', header = None)

#Seperate the names from classes for train and test
vals = df.iloc[:,:].values
testVals = tdf.iloc[:,:].values
names = [n[0][2:] for n in vals]
testNames = [n[0][2:] for n in testVals]

#Make sure the cls and testCls has the same amount of features
cls = [n[0][0] for n in vals]
testCls = [int(i) for i in cls] #convert cls into ints

#Splits each sample into words as indices
names = [n.split() for n in names]
testNames = [n.split() for n in testNames]

#Filter out any words less than 4 letters in names and testNames
names = filterLen(names, 4)
testNames = filterLen(testNames, 4)

#Eliminates spaces and special characters and makes all letters lowercase in the training set
temp_docs = []
for doc in names:
    temp_doc = []
    for word in doc:
        temp = ''.join(c for c in word if c.isalnum())
        temp_doc.append(temp.lower())
    temp_docs.append(temp_doc)

names = temp_docs

#Eliminates spaces and special characters and makes all letters lowercase in the test set
temp_docs = []
for doc in testNames:
    temp_doc = []
    for word in doc:
        temp = ''.join(c for c in word if c.isalnum())
        temp_doc.append(temp.lower())
    temp_docs.append(temp_doc)

testNames = temp_docs

#Build a sparse matrix for both train and test datasets
train_matrix, idx = build_matrix(names)
csr_info(train_matrix)
test_matrix = build_matrix2(testNames, idx)
csr_info(test_matrix)

#Scale feature importance by using inverse document frequency
#Was not used in final implementation
train_matrix1 = csr_idf(train_matrix, copy = True)
test_matrix1 = csr_idf(test_matrix, copy = True)

#Normalize the train and test matricies by l2 norm
#Was used in final implmentation
train_matrix2 = csr_l2normalize(train_matrix, copy = True)
test_matrix2 = csr_l2normalize(test_matrix, copy = True)

class KNN_Classifer():

    def __init__(self, k, x_train, y_train):
        self.k = k
        self.x_train = x_train
        self.y_train = y_train

    def predict(self, x_test):
        self.x_test = x_test
        y_predict = []
        count = 0

        for i in self.x_test:
            tc = [] #k highest cosine similaritty scores, (index, score)
            classes = []
            sims = self.cosine_similarity(i, self.x_train)
            for s in sims[:self.k]:#Finds the k highest cosine similarity scores
                tc.append(s)
            for j in range(len(tc)):
                classes.append(self.y_train[tc[j][0]]) #appends the y_train value at index in each tc
            y_predict.append(mode(classes))

        return y_predict

    def cosine_similarity(self, x, train):
        dots = x.dot(train.T) #computes dot product
        sims = list(zip(dots.indices, dots.data))
        sims.sort(key=lambda x: x[1], reverse=True) #orders cosine similarity from highest to lowest
        return sims


#k = 20 got me my highest F-1 score
knn = KNN_Classifer(20, train_matrix2, testCls) 
predicted = knn.predict(test_matrix2)

#SKlearn k nearest neighbors library and functions that were never used
"""knn = KNeighborsClassifier(n_neighbors=7)
clf = knn.fit(train_matrix2, testCls)
predicted = clf.predict(test_matrix2)"""

#Write the predicted classes to predictions.dat
pred_file = open('predictions.dat', 'w+')
predicted_df = pd.DataFrame(predicted)
predicted_df.to_csv('predictions.dat', index = False, header = None)