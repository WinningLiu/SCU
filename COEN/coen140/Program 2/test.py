import pandas as pd

traindata = pd.read_csv("pr2/train.dat",sep='\t',header=None)

from sklearn.feature_extraction.text import CountVectorizer 
count_vect = CountVectorizer()
train_counts = count_vect.fit_transform(traindata[1])

from sklearn.feature_extraction.text import TfidfTransformer 
tfidf_transformer = TfidfTransformer()
train_tfidf = tfidf_transformer.fit_transform(train_counts)

from sklearn.naive_bayes import MultinomialNB
clf = MultinomialNB().fit(train_tfidf, traindata[0])

from sklearn.pipeline import Pipeline
from sklearn.svm import LinearSVC

text_classifier = Pipeline([('vect', CountVectorizer()),
        ('tfidf', TfidfTransformer()),
        ('svc', LinearSVC())])

text_classifier = text_classifier.fit(traindata[1], traindata[0])

testdata = open("pr2/test.dat")
predicted = text_classifier.predict(testdata)

result = pd.DataFrame(predicted)

result.to_csv('predictions.dat',index=False,header=None)