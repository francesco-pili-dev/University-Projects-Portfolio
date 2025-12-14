from sklearn.tree import DecisionTreeClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import accuracy_score
from sklearn.ensemble import RandomForestClassifier
from sklearn.svm import SVC
from sklearn.base import BaseEstimator, ClassifierMixin
from sklearn.utils import shuffle
import numpy as np
import pandas as pd

#custom mutliplo hard e soft voting
#ensemble
class EnsembleClassifier(BaseEstimator, ClassifierMixin):
	def __init__(self, classifiers, voting='hard', weights=None,accuracy_score=None):
		self.classifiers = classifiers
		self.voting = voting
		self.weights = weights
		self.accuracy_score = accuracy_score

	def fit(self, X, y):
		for clf in self.classifiers:
			clf.fit(X, y)
		self.classes_= self.classifiers[0].classes_
		
	def predict(self, X):
		predictions = []
		for clf in self.classifiers:
			predictions.append(clf.predict(X))
		self.classes_= self.classifiers[0].classes_
		
		if self.voting == 'hard':
			return self._hard_voting(predictions)
		elif self.voting == 'soft':
			return self._soft_voting(X)
		else:
			raise ValueError("Opzione non valida. 'hard' or 'soft'.")

	def _hard_voting(self, predictions):#classe più votata
		ensemble_predictions = []
		for samples in zip(*predictions):
			ensemble_predictions.append(max(set(samples), key=samples.count))
		return ensemble_predictions

	def _soft_voting(self, X_test):#probabilità più alte
		# Inizializza una matrice per memorizzare le probabilità predette dai modelli
		prob_matrix = np.zeros((len(X_test), len(self.classifiers[0].classes_)))

		# Ottieni le probabilità predette da ciascun modello
		for model in self.classifiers:
			prob = model.predict_proba(X_test)
			prob_matrix += prob

		# Calcola la media delle probabilità per ciascuna classe
		avg_prob = prob_matrix / len(self.classifiers)

		# Ottieni le classi predette utilizzando la probabilità media più alta
		y_pred = [self.classifiers[0].classes_[np.argmax(prob)] for prob in avg_prob]

		return y_pred

	def predict_proba(self,X_test):
		return np.mean([classifier.predict_proba(X_test) for classifier in self.classifiers],axis=0)
		
	def set_class_name(self, new_name):
		    # Metodo per impostare il nuovo nome della classe
		    self.__name__ = new_name
	def get_class_name(self):
		    # Metodo per ottenere nome della classe, faccio così altrimenti compare ensemble nei grafici
		    return self.__name__
		
	
# Creazione dei classificatori di base
clf1 = DecisionTreeClassifier(criterion="gini", max_depth=None, min_samples_split=3, min_samples_leaf=1, max_features=None, random_state=42)
clf2 = RandomForestClassifier(criterion='gini',max_depth=53)#0.8125 come l'altro sopra
clf3 = SVC(kernel = 'linear',C =3.0,probability=True)#0.96875


# Creazione dell'ensemble classifier
ensemble_hard = EnsembleClassifier(classifiers=[clf1, clf2, clf3], voting='hard')
ensemble_hard.set_class_name("ensemble_hard")


ensemble_soft = EnsembleClassifier(classifiers=[clf1, clf2, clf3], voting='soft')
ensemble_soft.set_class_name("ensemble_soft")

"""
# Addestramento dell'ensemble classifier
ensemble_hard.fit(X_train, y_train)

# Previsioni sull'insieme di test
y_pred = ensemble_hard.predict(X_test)

# Valutazione delle previsioni
accuracy = accuracy_score(y_test, y_pred)
print("Accuracy:", accuracy)"""
