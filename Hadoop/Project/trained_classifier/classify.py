import nltk
import pickle
import sys
import re
from nltk.classify.naivebayes import NaiveBayesClassifier

word_features = []

def get_words_in_tweets(tweets):
    all_words = []
    for (words, sentiment) in tweets:
      all_words.extend(words)
    return all_words


def get_word_features(wordlist):
    wordlist = nltk.FreqDist(wordlist)
    word_features = wordlist.keys()
    return word_features


def read_tweets(fname, t_type):
    tweets = []
    f = open(fname, 'r')
    line = f.readline()
    while line != '':
        tweets.append([line, t_type])
        line = f.readline()
    f.close()
    return tweets


def extract_features(document):
    document_words = set(document)
    features = {}
    for word in word_features:
      features['contains(%s)' % word] = (word in document_words)
    return features

def classify_tweet(tweet):
  return classifier.classify(extract_features(nltk.word_tokenize(tweet)))


f = open('trained_classifier/simple_tweet_classifier.pickle', 'rb')
classifier = pickle.load(f)
f.close()

p = re.compile("contains\((.*)\)")
for item in classifier._feature_probdist.items():
  word_features.append(p.search(item[0][1]).group(1))

tweet_to_classify = sys.argv[1]
print tweet_to_classify

result = classify_tweet(tweet_to_classify)
print result

if result == 'negative':
  sys.exit(0) 
elif result == 'positive':
  sys.exit(1)


  sys.exit(0) 
