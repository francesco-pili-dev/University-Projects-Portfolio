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

from colors import *
from custom_multiple import EnsembleClassifier, ensemble_hard,ensemble_soft
from custom_simple import * 

from analisi_dati import *
from compare_models import *

global classifiers
def get_classifiers():
	return [
	DecisionTreeClassifier(criterion="gini", max_depth=None, min_samples_split=3, min_samples_leaf=1, max_features=None, random_state=42),
	RandomForestClassifier(criterion='gini',max_depth=53),#0.8125 come l'altro sopra
 	SVC(kernel = 'linear',C =3.0,probability=True),#0.96875
 	
 	ensemble_hard,
 	ensemble_soft,
 	NaiveBayesClassifier(),
 	#MLPClassifier(max_iter=10000,random_state=42),#acc 0,96 buono ma superato
 	MLPClassifier(activation= 'identity',early_stopping= False,hidden_layer_sizes= (50,),max_iter= 10000,momentum= 0.9,nesterovs_momentum= True,random_state= 2,tol= 1e-06)#Accuratezza media della combinazione migliore: 0.890
 	
]
class main():
	def run():
		while True:
			print(fg + "Seleziona un'operazione:")
			print(yy + "1. Analisi del dataset")
			print(yy + "2. Confronto tra tecniche di classificazione (senza scalatura dati)")
			print(yy + "3. Confronto con e senza scaling dei dati")
			print(yy + "4. [LENTO] Confronto con e senza oversampling")
			print(yy + "5. [LENTO] Confronto con e senza feature selection")
			print(fr + "6. Esci")
			
			choice = input("Inserisci il numero corrispondente all'operazione da eseguire: ")

			if choice == "1":
				print("\n-------------------------------------------------")
				print("Analisi statistica del dataset:") # Analisi statistica del dataset
				print("# Analisi statistica degli attributi numerici")
				print(data.describe())
				print("-------------------------------------------------")
				while(True):
					print()
					print(yy+'''\t\t1 Conta il numero di istanze per ciascuna specie 
		2 Zeri e valori mancanti degli attributi
		3 Matrice di correlazione
		4 Grafico a barre varianza degli attributi
		5 Distribuzione degli attributi 
		6 BoxPlot attributi ''')
					print(fr+'\t\t7 ritorna indietro')
					choice = input("Inserisci il numero corrispondente all'operazione da eseguire: ")
					
					if choice == "1": conta_istanze(data)
					elif choice == "2": zero_attrib(data)
					elif choice == "3": corr_matrix(data)
					elif choice == "4": var_attrib(data)
					elif choice == "5": distrib_attrib(data)
					elif choice == "6": box_plot_attrib(data)
					else: break
		
			elif choice == "2":
				# Divisione del dataset in features e target
				#svolto più volte perché sovrascrivo nella compare_methods
				X = data.drop("Species", axis=1)
				y = data["Species"]
			
				# Divisione del dataset in training set e test set
				#strattificato così da avere almeno un elemento per classe(non accade sempre altrimenti)
				X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=2)

				
				main.compare_methods("Confronto prestazioni SENZA standardizzazione dei dati:",get_classifiers(),X_train, X_test, y_train, y_test,X,y)
				print("-------------------------------------------------")
			elif choice == "3":
				title = ["SENZA STRATTIFICAZIONE","CON STRATTIFICAZIONE","standard scaler" ,"MinMax scaler" ,"normalizzazione" ]
				
	
				#svolto più volte perché sovrascrivo 
				X = data.drop("Species", axis=1)
				y = data["Species"]
				
				for classifier in get_classifiers():
					fitted =[]
					model_names = []
					list_x_test = []
					list_y_test = []
					#senza stattificazione
					m1 = copy.deepcopy(classifier)
					train_x, test_x, train_y, test_y = train_test_split(X, y, random_state=2, test_size=0.2)
					m1.fit(train_x, train_y)
					pred_y = m1.predict(test_x)
					print(bb+'Accuratezza del %s sul dataset originale SENZA STRATTIFICAZIONE, %s' % (get_model_name(m1), accuracy_score(test_y, pred_y)))
					list_x_test.append(test_x)
					
					#con strattificazione
					m11 = copy.deepcopy(classifier) #clono modello, così non sovrascrive l'originale
					#split con stratificazione
					train_x, test_x, train_y, test_y = train_test_split(X, y, random_state=0, test_size=0.2,stratify=y)
					#train_x, test_x, train_y, test_y = train_test_split(X, y, random_state=2,stratify=y, test_size=0.2)#abbiamo notato che con random state=2 mlp raggiunge 1.0 di accuracy e con 0 resta su 0.875
					
					m11.fit(train_x, train_y)
					pred_y = m11.predict(test_x)
					print(bb+'Accuratezza del %s sul dataset originale CON STRATTIFICAZIONE, %s' % (get_model_name(m11), accuracy_score(test_y, pred_y)))
					list_x_test.append(test_x)
					
					#Standard Scaler
					scaler = StandardScaler()
					scaler.fit(train_x)

					m2 = copy.deepcopy(classifier)
					m2.fit(scaler.transform(train_x), train_y)
					y_prova = scaler.transform(test_x)
					pred_y = m2.predict(y_prova)
					print(bb+'Accuratezza del %s dopo standard scaler %s' % (get_model_name(m2), accuracy_score(test_y, pred_y)))
					list_x_test.append(y_prova)
					
					#MinMax Scaler
					scaler = MinMaxScaler()
					scaler.fit(train_x)

					m3 = copy.deepcopy(classifier)
					m3.fit(scaler.transform(train_x), train_y)
					y_prova = scaler.transform(test_x)
					pred_y = m3.predict(y_prova)
					print(bb+'Accuratezza del %s dopo MinMax scaler %s' % (get_model_name(m3), accuracy_score(test_y, pred_y)))
					list_x_test.append(y_prova)

					#normalizzazione
					m4 = copy.deepcopy(classifier)
					m4.fit(normalize(train_x, norm='l1'), train_y)
					y_prova = normalize(test_x, norm='l1')
					pred_y = m4.predict(y_prova)
					
					print(bb+'Accuratezza del %s dopo normalizzazione %s' % (get_model_name(m4), accuracy_score(test_y, pred_y)))
					print("--------------------------------------------------")
					list_x_test.append(y_prova)
					
					fitted.append(m1)
					fitted.append(m11)
					fitted.append(m2)
					fitted.append(m3)
					fitted.append(m4)
					
					for t in title:
						model_names.append(str(get_model_name(m4))+" "+t)
					
					_, _,_, my = train_test_split(X, y, random_state=2, test_size=0.2)
					
					list_y_test = [my] + [test_y] * (len(fitted)-1) 	
					"""#Nel caso si desideri l'output senza sottomenu
					conf_matrix_3(fitted,list_x_test,list_y_test,classes,title=title)
					curva_roc3(fitted,list_x_test,test_y,classes,title=title)
					confronto_metriche3(fitted,list_x_test,list_y_test	,model_names=model_names)"""
					while(True):
						print()
						print(yy+'''\t\t\t1 Confusion Matrix Heatmap
			2 Curva ROC
			3 Grafico di Confronto delle Metriche''')
						print(fr + '\t\t\t4 ritorna indietro')
						choice = input("Inserisci il numero corrispondente all'operazione da eseguire: ")
							
						if choice == "1": conf_matrix_3(fitted,list_x_test,list_y_test,classes,title=title)
						elif choice == "2": curva_roc3(fitted,list_x_test,test_y,classes,title=title)
						elif choice == "3": confronto_metriche3(fitted,list_x_test,list_y_test	,model_names=model_names)
						else: break
			elif choice == "4":
				#OverSampling
				X = data.drop("Species", axis=1)
				y = data["Species"]		
				X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2,stratify=y, random_state=2)
				
				ros = RandomOverSampler(random_state=0)
				X_ros, y_ros= ros.fit_resample(X_train, y_train)
				fitted = []
				model_names = []
				title = ["Strattificato","Smote"] * len(get_classifiers())
				for model in get_classifiers():
					#modello originale senza modifiche
					ros_svc = copy.deepcopy(model)
					print(bc + f"Classificatore: {get_model_name(model)}")
					ros_svc.fit(X_train, y_train)
					fitted.append(ros_svc)
					#print("Score:",ros_svc.score(X_test, y_test))
					ros_svc_y_pred = ros_svc.predict(X_test)
					print("Accuracy:", accuracy_score(ros_svc_y_pred, y_test))
					#CrossValue
					ros_svc_scores = cross_val_score(ros_svc,X_ros, y_ros,cv=4)
					model.accuracy_score = accuracy_score(ros_svc_y_pred, y_test)
					print("Score with Cross Val:", ros_svc_scores.mean())
					
					#Oversampling: smote
					smote = SMOTE(random_state=0)
					X_smote, y_smote = smote.fit_resample(X, y)
					
					smote_model = copy.deepcopy(model)
					#Fit
					smote_model.fit(X_train, y_train)
					print("RFC Score:",smote_model.score(X_test, y_test))
					smote_model_y_pred = smote_model.predict(X_test)
					print("RFC Accuracy:", accuracy_score(smote_model_y_pred, y_test))
					#CrossValue
					smote_model_scores = cross_val_score(smote_model,X_smote, y_smote,cv=4)
					print("RFC score with Smote + Cross Val:", smote_model_scores.mean())
					fitted.append(smote_model)
					
					model_names.append(str(get_model_name(ros_svc))+" "+title[0])
					model_names.append(str(get_model_name(smote_model))+" "+title[1])
					
					
					print("-----------------------------------------")
				#Bilanciamento RandomOverSampler
				print("Dataset bilanciato")
				print(np.unique(y_ros, return_counts = True))
				
				print("Dataset bilanciato Smote")#da mettere nel drive
				print(np.unique(y_smote, return_counts = True))
				
				
				X = data.drop("Species", axis=1)
				y = data["Species"]		
				X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2,stratify=y, random_state=2)
								
				"""#Nel caso si desideri l'output senza sottomenu
				conf_matrix(fitted,X_test,y_test,classes,title=title)
				curva_roc(fitted,X_test,y_test,classes,title=title)
				confronto_metriche(fitted,X_test,y_test,model_names=model_names)"""
				while(True):
						print()
						print(yy+'''\t\t\t1 Confusion Matrix Heatmap
			2 Curva ROC
			3 Grafico di Confronto delle Metriche''')
						print(fr + '\t\t\t4 ritorna indietro')
						title=None
						choice = input("Inserisci il numero corrispondente all'operazione da eseguire: ")
						#print(X_train)
						if choice == "1": conf_matrix(fitted,X_test,y_test,classes,title=title)
						elif choice == "2": curva_roc(fitted,X_test,y_test,classes,title=title)
						elif choice == "3": confronto_metriche(fitted,X_test,y_test,model_names=model_names)
						else: break
			elif choice == "5":		
				classifiers = get_classifiers()
				list_x_test = []
				fitted = []
				title = []
				X = data.drop("Species", axis=1)
				y = data["Species"]
			
				# Divisione del dataset in training set e test set
				#strattificato così da avere almeno un elemento per classe(altrimenti non accade sempre )
				X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2,stratify=y, random_state=2)
				
				#calcolo miglior split
				tutto = main.feature_selection_with_classifiers(classifiers, data)
				for classifier, best_n_feature, max_accuracy, best_selected_features in tutto :
					fitted.append(classifier)
					list_x_test.append(X_test[best_selected_features])
					#title.append(f"{get_model_name(classifier)} \nselezionate migliori: {best_selected_features}")
					title.append(bb+f"{get_model_name(classifier)} \n{best_selected_features}")
					print(bb+f"/nClassificatore: {get_model_name(classifier)}")
					print(bb+f"Miglior n_features_to_select: {best_n_feature}, Migliore accuratezza: {max_accuracy}")
					print(bb+f"Feature selezionate migliori: {best_selected_features}")
					print("--------------------------------------------------------")
				
				"""#Nel caso si desideri l'output senza sottomenu
				conf_matrix_3(fitted,list_x_test,y_test,classes,title=title)
				curva_roc3(fitted,list_x_test,y_test,classes,title=title)
				confronto_metriche3(fitted,list_x_test,y_test,model_names=title)"""
				
				while(True):
						print()
						print(yy+'''\t\t\t1 Confusion Matrix Heatmap
			2 Curva ROC
			3 Grafico di Confronto delle Metriche	 
			4 ritorna indietro''')
						choice = input("Inserisci il numero corrispondente all'operazione da eseguire: ")
							
						if choice == "1": conf_matrix_3(fitted,list_x_test,y_test,classes,title=title)
						elif choice == "2": curva_roc3(fitted,list_x_test,y_test,classes,title=title)
						elif choice == "3": confronto_metriche3(fitted,list_x_test,y_test,model_names=title)
						else: break
				
			elif choice == "6":
				break
			else:
				print("Scelta non valida. Riprova.")
			print("-------------------------------------------------")
	
	
	def compare_methods(title,classifiers,X_train, X_test, y_train, y_test,X,y):
		print("-------------------------------------------------\n",title)
		fitted = list()
		classes = data['Species'].unique()
		for classifier in classifiers:
			classifier.fit(X_train, y_train)
			fitted.append(classifier)
			y_pred = classifier.predict(X_test)
			accuracy = accuracy_score(y_test, y_pred)
			precision = precision_score(y_test, y_pred, average='macro',zero_division=1)
			f1 = f1_score(y_test, y_pred, average='macro',zero_division=1)
			
			print(bc + f"Classificatore: {get_model_name(classifier)}")
			print(f"Accuracy: {accuracy}")
			print(f"Precision: {precision}")
			print(f"F1-score: {f1}\n")
		
		while(True):
			print()
			print(yy+'''\t\t1 Confusion Matrix Heatmap
		2 Curva ROC
		3 Grafico di Confronto delle Metriche''')
			print(fr + '\t\t4 ritorna indietro')
			choice = input("Inserisci il numero corrispondente all'operazione da eseguire: ")
					
			if choice == "1": conf_matrix(fitted,X_test,y_test,classes)
			elif choice == "2": curva_roc(fitted,X_test,y_test,classes)
			elif choice == "3": confronto_metriche(fitted,X_test,y_test)
			else: break
	
	def feature_selection_with_classifiers(classifiers, data):
		results = []

		X = data.drop("Species", axis=1)
		y = data["Species"]

		attribute_names = X.columns.tolist()

		X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=2)

		for classifier in classifiers:
		    max_accuracy = 0
		    best_n_feature = 0
		    best_selected_features = []

		    for n_feature in range(1, len(attribute_names)):
		        sfs = SequentialFeatureSelector(classifier, n_features_to_select=n_feature, direction='forward', scoring='accuracy')
		        X_reduced = sfs.fit_transform(X, y)

		        sfs = sfs.fit(X_train, y_train)

		        selected_indices = sfs.get_support(indices=True)
		        selected_feature_names = [attribute_names[i] for i in selected_indices]

		        X_train_selected = X_train[list(selected_feature_names)]
		        X_test_selected = X_test[list(selected_feature_names)]

		        classifier.fit(X_train_selected, y_train)
		        y_pred = classifier.predict(X_test_selected)

		        accuracy = accuracy_score(y_test, y_pred)

		        if accuracy > max_accuracy:
		            max_accuracy = accuracy
		            best_n_feature = n_feature
		            best_selected_features = selected_feature_names
		            best_classifier = copy.deepcopy(classifier)
				#log andamento
		        print(fg+f"Classificatore: {get_model_name(classifier)}")
		        print(f"Numero di feature selezionate: {n_feature}")
		        print(f"Feature selezionate: {selected_feature_names}")
		        print(f"Accuratezza: {accuracy}")
		        print("--------------------------------------------------------")

		    print(bc + f"Migliori valori per il classificatore: {get_model_name(classifier)}")
		    print(f"Miglior n_features_to_select: {best_n_feature}, Migliore accuratezza: {max_accuracy}")
		    print(f"Feature selezionate migliori: {best_selected_features}")
		    print("--------------------------------------------------------")

		    results.append((best_classifier, best_n_feature, max_accuracy, best_selected_features))

		return results
			

		

				
# Caricamento del dataset
data = pd.read_csv('Fish.csv')
classes = data['Species'].unique()

# Esecuzione del programma
if __name__ == "__main__":
	main.run()

#GaussianNB(),#naive Bayes#scartato un sacco di tuning e 0.59 acc
#KNeighborsClassifier(n_neighbors=5, metric='euclidean'),
#RandomForestClassifier(criterion='gini',max_depth=53),#0.78 vs 0.82


