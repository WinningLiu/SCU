from scipy.sparse import csr_matrix
import numpy as np
import scipy as sp
from collections import defaultdict

def build_matrix(docs):
    nrows = len(docs)
    idx = {}
    tid = 0
    nnz = 0
    ncols=0
    for d in docs:
        d1=d.split()
        nnz += int(len(d1)/2)
        for i in range(0,len(d1),2):
            feature_n = int(d1[i])-1
            if 1+feature_n > ncols:
                ncols = 1+feature_n        
    ind = np.zeros(nnz, dtype=np.int)
    val = np.zeros(nnz, dtype=np.double)
    ptr = np.zeros(nrows+1, dtype=np.int)
    i = 0  # document ID / row counter
    n = 0  # non-zero counter
    # transfer values
    for d in docs:
        d1=d.split()
        for j in range(0,len(d1),2):
            ind[n] = int(d1[j])-1
            val[n] = float(d1[j+1]) 
            n +=1
        ptr[i+1] =n
        i+=1
            
    mat = csr_matrix((val, ind, ptr), shape=(nrows, ncols), dtype=np.double)
    mat.sort_indices()
    
    return mat

with open("data/train.dat", "r") as fh:
    docs = fh.readlines()  
#mat1 = sparse matrix now
mat1 = build_matrix(docs)
print(mat1.get_shape())


def csr_idf(matrix, copy=False, **kargs):
    r""" Scale a CSR matrix by idf. 
    Returns scaling factors as dict. If copy is True, 
    returns scaled matrix and scaling factors.
    """
    if copy is True:
        matrix = matrix.copy()
    nrows = matrix.shape[0]
    nnz = matrix.nnz
    ind, val, ptr = matrix.indices, matrix.data, matrix.indptr
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
        
    return df if copy is False else matrix

def csr_l2normalize(matrix, copy=False, **kargs):
    r""" Normalize the rows of a CSR matrix by their L-2 norm. 
    If copy is True, returns a copy of the normalized matrix.
    """
    if copy is True:
        matrix = matrix.copy()
    nrows = matrix.shape[0]
    nnz = matrix.nnz
    ind, val, ptr = matrix.indices, matrix.data, matrix.indptr
    # normalize
    for i in range(nrows):
        rsum = 0.0    
        for j in range(ptr[i], ptr[i+1]):
            rsum += val[j]**2
        if rsum == 0.0:
            continue  # do not normalize empty rows
        rsum = float(1.0/np.sqrt(rsum))
        for j in range(ptr[i], ptr[i+1]):
            val[j] *= rsum
            
    if copy is True:
        return matrix

    
mat3 = csr_idf(mat1, copy=True)
mat33 = csr_l2normalize(mat3, copy=True)



from sklearn.decomposition import TruncatedSVD

def dimr_svd(n, mat):
    mat2=mat.copy()
    svd = TruncatedSVD(n_components=n)
    return svd.fit_transform(mat2)


from sklearn.manifold import TSNE

def dimr_tsne(n,mat):
    mat2=mat.copy()
    return TSNE(n_components=n).fit_transform(mat2)


from sklearn import random_projection
def dimr_rp(mat):
    mat2=mat.copy()
    transformer = random_projection.SparseRandomProjection()
    X_new = transformer.fit_transform(mat2)
    return X_new

mat4=dimr_rp(mat1)
print(mat4.get_shape())
print(type(mat4))


from sklearn.cluster import KMeans 
from sklearn.cluster import MiniBatchKMeans
def kmeans(n, mat):
    clf=MiniBatchKMeans(n_clusters=n)
    clf.fit(mat)
    labels=clf.labels_
    centers=clf.cluster_centers_
    #print(labels.shape)
    #print(centers.shape)
    return labels, centers


from sklearn.metrics.pairwise import cosine_similarity
import collections
import matplotlib.pyplot as plt
import queue

def neighbor_points(data, pointId, radius):
    #if cosine_similarity(c1[i],c1[j])>eps:
     #           pts.append(j)
    points = []
    for i in range(len(data)):
        if sp.spatial.distance.cosine(data[pointId] ,data[i]) <= radius:
        #if np.linalg.norm(data[i] - data[pointId]) <= radius:
            points.append(i)
    return points

def dbscan(data, Eps, MinPt):
    pointlabel  = [0] * len(data)
    pointcount = []
    corepoint=[]
    noncore=[]
    
    #Find all neigbor for all point
    for i in range(len(data)):
        pointcount.append(neighbor_points(data,i,Eps))
    
    #Find all core point, edgepoint and noise
    for i in range(len(pointcount)):
        if (len(pointcount[i])>=MinPt):
            pointlabel[i]=-1
            corepoint.append(i)
        else:
            noncore.append(i)

    for i in noncore:
        for j in pointcount[i]:
            if j in corepoint:
                pointlabel[i]=-2

                break
            
    #start assigning point to luster
    cl = 1
    #Using a Queue to put all neigbor core point in queue and find neigboir's neigbor
    for i in range(len(pointlabel)):
        q = queue.Queue()
        if (pointlabel[i] == -1):
            pointlabel[i] = cl
            for x in pointcount[i]:
                if(pointlabel[x]==-1):
                    q.put(x)
                    pointlabel[x]=cl
                elif(pointlabel[x]==-2):
                    pointlabel[x]=cl
            #Stop when all point in Queue has been checked   
            while not q.empty():
                neighbors = pointcount[q.get()]
                for y in neighbors:
                    if (pointlabel[y]==-1):
                        pointlabel[y]=cl
                        q.put(y)
                    if (pointlabel[y]==-2):
                        pointlabel[y]=cl            
            cl=cl+1 #move to next cluster
    
    
           
    return pointlabel,cl



from sklearn import metrics
mat4=mat33
l1, c1 = kmeans(250,mat4)

a,b=dbscan(c1,0.5,2)
print(b)
print(a)
print(metrics.silhouette_score(c1, a))


#l1 len 8580 has intiial labels
#a len 200 has centroid labels to shrinked 8 clusters
final_ans=[]
for i in range(len(l1)):
    temp=l1[i]
    final_ans.append(a[temp])
print(len(final_ans))
    


count=0
with open("predictions.dat", "w") as fn:
    for ans in final_ans:
        if ans==0:
            count+=1
        fn.write('%i\n' % ans)
print(count)