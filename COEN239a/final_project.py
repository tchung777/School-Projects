__author__ = "chchung"

"""
This script uses 5 ML classifiers to train to a dataset provided by the NBA for 20 years of regular season statistics. The five
classifiers ar einputted into a VotingClassifier to create a final classifier that would predict the semi-final contenders of the 2018 NBA 
Eastern and Western Conferences.
"""

import pandas as pd
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LogisticRegression
from sklearn.svm import SVC
from sklearn.linear_model import Perceptron
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.ensemble import VotingClassifier
from sklearn.metrics import classification_report

# Import Data
df = pd.read_csv('NBA_stats_project.csv', header=0)
df.columns = ["Rk (17-18)", "Team", "G", "MP", "FG", "FGA", "FG%", "3P", "3PA", "3P%", "2P", "2PA", "2P%", "FT", "FTA", "FT%", "ORB", "DRB",
              "TRB", "AST", "STL", "BLK", "TOV", "PF", "PTS", "Semifinals?"]
df.head()


df2 = df                                                    # Make copy of original dataFrame so we don't lose reference to all statistics
df = df.drop(columns=['Rk (17-18)', "Team", "G"])           # Drop Rank, Team and number of games columns since they're irrelevant or the same for all team.


# Create pd matrix to separate into features and data label
X = df[["MP", "FG", "FGA", "FG%", "3P", "3PA", "3P%",
        "2P", "2PA", "2P%", "FT", "FTA", "FT%", "ORB",
        "DRB", "TRB","AST", "STL", "BLK", "TOV", "PF",
        "PTS"]].copy()

Y = df["Semifinals?"]

# Split dataset into 70% training and 30% training with the first 150 as test.
X_train = X[150:535]
X_test = X[0:149]

Y_train = Y[150:535]
Y_test = Y[0:149]

# Latest NBA statistics are appended to the bottom of csv
newX = X[536:565]
newY = Y[536:565]

# Standardize the dataset so when we use kNN later we don't get punished for not using feature scaling.
sc = StandardScaler()
sc.fit(X_train)
X_train_std = sc.transform(X_train)
X_test_std = sc.transform(X_test)

"""
To get the training accuracy for this section, simply replace all instances of X_test_std past this point with X_train_std.
"""

# --- Logistic Regression Implementation and Use --- #
lr = LogisticRegression(C=100.0)
lr.fit(X_train_std, Y_train)

y_pred = lr.predict(X_test_std)

print('Logistic Regression Misclassified samples: %d' % (Y_test != y_pred).sum())
print('Logistic Regression Accuracy: %.2f' % lr.score(X_test_std, Y_test))
print(classification_report(Y_test, y_pred))

coef = lr.coef_[0].tolist()  # Print out the weighted coefficients produced by LR
print(df.columns)            # Print out the columns
result = []

coef = [abs(i) for i in coef]
coef2 = coef
#print(coef2)

# Print out the features with the biggest magnitude and report it.
for i in range(5):
    import operator

    index, value = max(enumerate(coef2), key = operator.itemgetter(1))
    print(index)
    result.append(value)
    coef.remove(value)

print(result)
print("Top 5 are: {}, {}, {}, {}, {}".format(df.columns[5], df.columns[3], df.columns[13], df.columns[15], df.columns[16]))

# --- Support Vector Machinne Implementation and Use --- #
svm = SVC(kernel='linear', probability=True)
svm.fit(X_train_std, Y_train)

y_pred = svm.predict(X_test_std)

print('SVM w/ Linear Kernel Misclassified samples: %d' % (Y_test != y_pred).sum())
print('SVM w/ Linear Kernel Accuracy: %.2f' % svm.score(X_test_std, Y_test))
print(classification_report(Y_test, y_pred))

# --- Perceptron Implementation and Use --- #

ppn = Perceptron(n_iter=40, eta0=0.1, random_state=1)
ppn.fit(X_train_std, Y_train)

y_pred = ppn.predict(X_test_std)

print('Perceptron Misclassified samples: %d' % (Y_test != y_pred).sum())
print('Perceptron Accuracy: %.2f' % ppn.score(X_test_std, Y_test))
print(classification_report(Y_test, y_pred))


# --- AdaBoost Implementation and Use --- #

bdt = AdaBoostClassifier(DecisionTreeClassifier(max_depth=1),
                         algorithm="SAMME",
                         n_estimators=200)
bdt.fit(X_train_std, Y_train)
y_pred = bdt.predict(X_test_std)

print('AdaBoost Misclassified samples: %d' % (Y_test != y_pred).sum())
print('AdaBoost Accuracy: %.2f' % bdt.score(X_test_std, Y_test))
print(classification_report(Y_test, y_pred))

# --- K Nearest Neighbour Implementation and Use --- #

neigh = KNeighborsClassifier(n_neighbors=3)
neigh.fit(X_train_std, Y_train)

y_pred = neigh.predict(X_test_std)

print('KNeighbor Misclassified samples: %d' % (Y_test != y_pred).sum())
print('KNeighbor Accuracy: %.2f' % neigh.score(X_test_std, Y_test))
print(classification_report(Y_test, y_pred))

# --- Majority Rule Classifier Implementation and Use --- #

eclf1 = VotingClassifier(estimators=[('lr', lr), ('svm', svm), ('ppn', ppn), ('bdt', bdt), ('neigh', neigh)], voting='hard')
eclf1 = eclf1.fit(X_train_std, Y_train)
y_pred2 = eclf1.predict(X_test_std)

print('Majority Rule Misclassified samples: %d' % (Y_test != y_pred2).sum())
print('Majority Accuracy: %.2f' % eclf1.score(X_test_std, Y_test))
print(classification_report(Y_test, y_pred2))

# Print out the prediction/probabilities for the current NBA season and the teams.
print(svm.predict_proba(newX))
print(df2["Team"][536:565])