import sys
import os

global np
global pd
global plt
global fitted

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from imblearn.over_sampling import RandomOverSampler,SMOTE,ADASYN
from sklearn.model_selection import train_test_split,cross_val_score
from sklearn.preprocessing import StandardScaler,normalize,MinMaxScaler
from sklearn.linear_model import LogisticRegression, LinearRegression
from sklearn.ensemble import RandomForestClassifier, RandomForestRegressor, GradientBoostingClassifier
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, mean_squared_error, r2_score,confusion_matrix
from sklearn.naive_bayes import GaussianNB
from sklearn.tree import DecisionTreeClassifier, DecisionTreeRegressor
from sklearn.neighbors import KNeighborsClassifier
from sklearn.svm import SVC
from sklearn.neural_network import MLPClassifier
import copy

from sklearn.feature_selection import SelectKBest, chi2, mutual_info_classif,SequentialFeatureSelector
#from mlxtend.feature_selection import SequentialFeatureSelector
from mlxtend.plotting import plot_sequential_feature_selection
from sklearn.neural_network import MLPClassifier
from sklearn.model_selection import GridSearchCV

data = pd.read_csv('Fish.csv')
classes = data['Species'].unique()
X = data.drop("Species", axis=1)
y = data["Species"]


X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=2)
#SVC
params = {
    "kernel" : ['linear', 'poly', 'rbf', 'sigmoid'],
    "degree": [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 40],
    'gamma': ['scale', 'auto'],
    'shrinking': [True, False],
    'verbose': [False],
    'random_state': [2]
}
#trova i migliori parametri dei modelli
def gridSearchSVM(train_x, train_y, param_grid):
    # Creazione di un classificatore di tipo SVM
    svm_clf =SVC()
    # Numero di fold per la Cross-validation
    n_folds = 5
    # Creazione di un oggetto di tipo GridSearchCV
    grid_search_cv = GridSearchCV(svm_clf, param_grid, cv=n_folds)
    # Esecuzione della ricerca degli iperparametri
    grid_search_cv.fit(train_x, train_y)
    # Stampa risultati
    print('Combinazioni di parametri:\n', grid_search_cv.cv_results_['params'])
    print('Accuratezza media per combinazione:\n', grid_search_cv.cv_results_['mean_test_score'])
    print('Combinazione migliore:\n', grid_search_cv.best_params_)
    print('Accuratezza media della combinazione migliore: %.3f' % grid_search_cv.best_score_)
    print()
    print()


scaler = StandardScaler()
scld_train_x1 = scaler.fit_transform(X_train)
print('SVM lineare')
gridSearchSVM(scld_train_x1, y_train, params)

#----------------------------------------------------------------------------------------------------------------------------------
#KNeighborsClassifier
from sklearn.neural_network import MLPClassifier

params = {
    'n_neighbors': [1, 3, 5, 7, 9,20,50,100],
    'weights': ['uniform', 'distance'],
    'algorithm': ['auto', 'ball_tree', 'kd_tree', 'brute'],
    'leaf_size': [10, 20, 30, 40, 50],
    'p': [1, 2],
    'metric': ['euclidean', 'manhattan', 'chebyshev', 'minkowski'],
}


from sklearn.model_selection import GridSearchCV

def gridSearchSVM(train_x, train_y, param_grid):
    # Creazione di un classificatore di tipo SVM
    svm_clf = KNeighborsClassifier()
    # Numero di fold per la Cross-validation
    n_folds = 5
    # Creazione di un oggetto di tipo GridSearchCV
    grid_search_cv = GridSearchCV(svm_clf, param_grid, cv=n_folds)
    # Esecuzione della ricerca degli iperparametri
    grid_search_cv.fit(train_x, train_y)
    # Stampa risultati
    print('Combinazioni di parametri:\n', grid_search_cv.cv_results_['params'])
    print('Accuratezza media per combinazione:\n', grid_search_cv.cv_results_['mean_test_score'])
    print('Combinazione migliore:\n', grid_search_cv.best_params_)
    print('Accuratezza media della combinazione migliore: %.3f' % grid_search_cv.best_score_)
    print()
    print()


classes = data['Species'].unique()
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2,stratify=y, random_state=2)

scaler = StandardScaler()
scld_train_x1 = scaler.fit_transform(X_train)
print('SVM lineare')
gridSearchSVM(scld_train_x1, y_train, params)

#----------------------------------------------------------------------------------------------------------------------------------
#RandomForestClassifier
from sklearn.neural_network import MLPClassifier

params = {
    "n_estimators": [10, 50, 100, 200],
    "criterion": ["gini", "entropy"],
    "max_depth": [None, 5, 10, 20],
    "min_samples_split": [2, 5, 10],
    "min_samples_leaf": [1, 2, 4],
    "max_features": ["auto", "sqrt", "log2"],
    "bootstrap": [True, False],
    "random_state": [42]
}




from sklearn.model_selection import GridSearchCV

def gridSearchSVM(train_x, train_y, param_grid):
    # Creazione di un classificatore di tipo SVM
    svm_clf = RandomForestClassifier()
    # Numero di fold per la Cross-validation
    n_folds = 5
    # Creazione di un oggetto di tipo GridSearchCV
    grid_search_cv = GridSearchCV(svm_clf, param_grid, cv=n_folds)
    # Esecuzione della ricerca degli iperparametri
    grid_search_cv.fit(train_x, train_y)
    # Stampa risultati
    print('Combinazioni di parametri:\n', grid_search_cv.cv_results_['params'])
    print('Accuratezza media per combinazione:\n', grid_search_cv.cv_results_['mean_test_score'])
    print('Combinazione migliore:\n', grid_search_cv.best_params_)
    print('Accuratezza media della combinazione migliore: %.3f' % grid_search_cv.best_score_)
    print()
    print()


classes = data['Species'].unique()
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2,stratify=y, random_state=2)

scaler = StandardScaler()
scld_train_x1 = scaler.fit_transform(X_train)
print('SVM lineare')
gridSearchSVM(scld_train_x1, y_train, params)

#----------------------------------------------------------------------------------------------------------------------------------
#Decision Tree
from sklearn.neural_network import MLPClassifier

params = {
    "criterion": ["gini", "entropy","log_loss"],
    "splitter": ["best", "random"],
    "max_depth": [None, 10, 20, 30, 40, 50,60,70,80,90,100,120,150,180,210,250],
    "min_samples_split": [2, 3,4,5,6, 10],
    "min_samples_leaf": [1, 2, 4],
    "max_features": [None, "sqrt", "log2"],
    "random_state": [2]
}




from sklearn.model_selection import GridSearchCV

def gridSearchSVM(train_x, train_y, param_grid):
    # Creazione di un classificatore di tipo SVM
    svm_clf = DecisionTreeClassifier()
    # Numero di fold per la Cross-validation
    n_folds = 5
    # Creazione di un oggetto di tipo GridSearchCV
    grid_search_cv = GridSearchCV(svm_clf, param_grid, cv=n_folds)
    # Esecuzione della ricerca degli iperparametri
    grid_search_cv.fit(train_x, train_y)
    # Stampa risultati
    print('Combinazioni di parametri:\n', grid_search_cv.cv_results_['params'])
    print('Accuratezza media per combinazione:\n', grid_search_cv.cv_results_['mean_test_score'])
    print('Combinazione migliore:\n', grid_search_cv.best_params_)
    print('Accuratezza media della combinazione migliore: %.3f' % grid_search_cv.best_score_)
    print()
    print()


classes = data['Species'].unique()
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2,stratify=y, random_state=2)

gridSearchSVM(X_train, y_train, params)
#------------------------------------------------------------------------------------------------------------------------------
#MLP
from sklearn.neural_network import MLPClassifier

params = {
    "hidden_layer_sizes": [(50,), (100,), (50, 50), (100, 50)],
    "activation": ['identity', 'logistic', 'tanh', 'relu'],
    "solver": ['lbfgs', 'sgd', 'adam'],
    "alpha": [0.0001, 0.001, 0.01],
    "batch_size": ['auto', 32, 64],
    "learning_rate": ['constant', 'invscaling', 'adaptive'],
    "momentum": [0.9, 0.95, 0.99],
    "nesterovs_momentum": [True, False],
    "early_stopping": [True],
}

data = pd.read_csv('Fish.csv')

X = data.drop("Species", axis=1)
y = data["Species"]

# Divisione del dataset in training set e test set
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=2)



from sklearn.model_selection import GridSearchCV

def gridSearchSVM(train_x, train_y, param_grid):
    # Creazione di un classificatore di tipo SVM
    svm_clf = MLPClassifier()
    # Numero di fold per la Cross-validation
    n_folds = 5
    # Creazione di un oggetto di tipo GridSearchCV
    grid_search_cv = GridSearchCV(svm_clf, param_grid, cv=n_folds)
    # Esecuzione della ricerca degli iperparametri
    grid_search_cv.fit(train_x, train_y)
    # Stampa risultati
    print('Combinazioni di parametri:\n', grid_search_cv.cv_results_['params'])
    print('Accuratezza media per combinazione:\n', grid_search_cv.cv_results_['mean_test_score'])
    print('Combinazione migliore:\n', grid_search_cv.best_params_)
    print('Accuratezza media della combinazione migliore: %.3f' % grid_search_cv.best_score_)
    print()
    print()


classes = data['Species'].unique()
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2,stratify=y, random_state=2)

gridSearchSVM(X_train, y_train, params)



















#tentativo
#
from sklearn.neural_network import MLPClassifier

params ={
    "priors": [None],
    "var_smoothing": [1e-09, 1e-08, 1e-07, 1e-06, 1e-05]
}




from sklearn.model_selection import GridSearchCV

def gridSearchSVM(train_x, train_y, param_grid):
    # Creazione di un classificatore di tipo SVM
    svm_clf = GaussianNB()
    # Numero di fold per la Cross-validation
    n_folds = 5
    # Creazione di un oggetto di tipo GridSearchCV
    grid_search_cv = GridSearchCV(svm_clf, param_grid, cv=n_folds)
    # Esecuzione della ricerca degli iperparametri
    grid_search_cv.fit(train_x, train_y)
    # Stampa risultati
    print('Combinazioni di parametri:\n', grid_search_cv.cv_results_['params'])
    print('Accuratezza media per combinazione:\n', grid_search_cv.cv_results_['mean_test_score'])
    print('Combinazione migliore:\n', grid_search_cv.best_params_)
    print('Accuratezza media della combinazione migliore: %.3f' % grid_search_cv.best_score_)
    print()
    print()


classes = data['Species'].unique()
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2,stratify=y, random_state=2)
#dataset moons
scaler = StandardScaler()
scld_train_x1 = scaler.fit_transform(X_train)
print('SVM lineare su dataset moons')
gridSearchSVM(scld_train_x1, y_train, params)


