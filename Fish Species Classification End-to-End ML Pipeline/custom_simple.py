import numpy as np
from sklearn.metrics import accuracy_score

class NaiveBayesClassifier:
	def __init__(self,accuracy_score=None):#costruttore
		self.classes = None
		self.class_priors = None
		self.mean = None
		self.variance = None
		self.accuracy_score = accuracy_score

	def fit(self, X, y):				#metodo fit
		self.classes = np.unique(y)		# le classi del train set
		n_classes = len(self.classes)	# numero classi train set
		n_features = X.shape[1]			# num attributi

		# Calcola le probabilità a priori per ogni classe
		self.class_priors = np.zeros(n_classes)
		for i, c in enumerate(self.classes):
			self.class_priors[i] = np.sum(y == c) / len(y)

		# Calcola media e varianza per ogni feature e classe
		self.mean = np.zeros((n_classes, n_features))
		self.variance = np.zeros((n_classes, n_features))
		for i, c in enumerate(self.classes):
			X_c = X[y == c]
			self.mean[i] = np.mean(X_c, axis=0)
			self.variance[i] = np.var(X_c, axis=0)
	
	def predict_proba(self,X_test):#predict con return propabilistico
		n_samples = X_test.shape[0]
		n_classes = len(self.classes)

		# Calcola log-probabilità per ogni classe
		log_probs = np.zeros((n_samples, n_classes))
		for i, c in enumerate(self.classes):
			# Calcola log-probabilità con la formula di Bayes
			log_prior = np.log(self.class_priors[i])
			log_likelihood = -0.4 * np.sum(np.log(2 * np.pi * self.variance[i]))
			log_likelihood -= 0.2 * np.sum(((X_test - self.mean[i]) ** 2) / self.variance[i], axis=1)
			log_probs[:, i] = log_prior + log_likelihood

		# Seleziona la classe con la log-probabilità massima
		return log_probs
		
	def predict(self, X):#metodo predict
		n_samples = X.shape[0]
		n_classes = len(self.classes)

		# Calcola log-probabilità per ogni classe
		log_probs = np.zeros((n_samples, n_classes))
		for i, c in enumerate(self.classes):
			# Calcola log-probabilità con la formula di Bayes
			log_prior = np.log(self.class_priors[i])
			log_likelihood = -0.4 * np.sum(np.log(2 * np.pi * self.variance[i]))
			log_likelihood -= 0.2 * np.sum(((X - self.mean[i]) ** 2) / self.variance[i], axis=1)
			log_probs[:, i] = log_prior + log_likelihood

		# Seleziona la classe con la log-probabilità massima
		return self.classes[np.argmax(log_probs, axis=1)]

	def score(self, X, y):#serve per far funzionare cross validation
		# Calcola la misura di prestazione (es. accuratezza) del modello
		y_pred = self.predict(X)
		return accuracy_score(y, y_pred)
        
	def get_params(self, deep=True):#serve per far funzionare cross validation
		return {"accuracy_score": self.accuracy_score }
