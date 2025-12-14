import math
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.metrics import roc_curve, roc_auc_score
from sklearn.preprocessing import label_binarize
from sklearn.metrics import roc_curve, auc
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, confusion_matrix


def get_model_name(model):
	if model.__class__.__name__ == "EnsembleClassifier":
		return model.get_class_name()
	return model.__class__.__name__

def curva_roc(models,X_test,y_test,classes,title=None):
	#calcolo num colonne e righe per subplot
	rows = math.ceil(len(models)/4)+1 if len(models) % 2 == 0 else math.ceil(len(models)/4)+1
	cols= math.ceil(len(models)/4)-1 if len(models) % 2 == 0 else math.ceil(len(models)/4)+1
	
	plt.figure(figsize=(12, 12))
	for i,model in enumerate(models):
		ax = plt.subplot(rows,cols,i+1)
		plt.title(f'Curva ROC {get_model_name(model)}')
		
		if title is not None:
			plt.title(f'Curva ROC {get_model_name(model)} {title[i]}')
		# Conversione y_test in etichette binarie per ciascuna classe (one-hot encoding)
		y_test_bin = label_binarize(y_test, classes=classes)

		# Calcola le probabilità predette per tutte le classi dal modello
		y_prob = model.predict_proba(X_test)

		# Calcola la curva ROC per tutte le classi
		for i in range(len(classes)):
			fpr, tpr, _ = roc_curve(y_test_bin[:, i], y_prob[:, i])#estraggo TPR e FPR
			roc_auc = auc(fpr, tpr)#area sottesa alla curva ROC (AUC -> Area Under the Curve) 
			plt.plot(fpr, tpr, label=f'{classes[i]} (AUC = {roc_auc:.2f})')

		plt.plot([0, 1], [0, 1], 'k--', label='Random')
		plt.xlabel('FPR')						#False Positive Rate 
		plt.ylabel('True Positive Rate (TPR)')  #TRUE Positive Rate 
		
		plt.legend(loc='lower right') #sposto leggenda in basso a destra, così non distrurba lettura
		plt.grid(True)		#linee guida griglia
		plt.tight_layout()  #layout stretto perché mostro più cose
	plt.show()

def conf_matrix(models,X_test,y_test,classes,title=None):
	plt.figure(figsize=(12, 12))
	rows = math.ceil(len(models)/4)+1 if len(models) % 2 == 0 else math.ceil(len(models)/4)+1
	cols= math.ceil(len(models)/4)-1 if len(models) % 2 == 0 else math.ceil(len(models)/4)+1
	
	for i,model in enumerate(models):
		ax = plt.subplot(rows,cols,i+1)
		plt.title(f'Confusion Matrix {get_model_name(model)}')
		
		if title is not None:
			plt.title(f'Conf Matrix {get_model_name(model)} {title[i]}')
			
		
		y_pred = model.predict(X_test)
		accuracy = accuracy_score(y_test, y_pred)
		precision = precision_score(y_test, y_pred, average='weighted',zero_division=1)
		f1 = f1_score(y_test, y_pred, average='weighted',zero_division=1)
		
		# Confusion Matrix Heatmap
		conf_matrix = confusion_matrix(y_test, y_pred)
		#Heatmap
		sns.heatmap(conf_matrix, annot=True, fmt='d', cmap='Blues', xticklabels=classes, yticklabels=classes)
		plt.xlabel('Predicted Label',fontsize=12)
		plt.xticks(rotation=45)
		plt.ylabel('True Label',fontsize=12)
		plt.yticks(rotation=0)

		plt.tight_layout()
	plt.show()

def confronto_metriche(models,X_test,y_test,model_names=None):
	# Valuta le metriche di valutazione per ogni modello
	metrics = {}
	for i,model in enumerate(models):
		name = get_model_name(model)
		if model_names is not None:
			name = model_names[i]
		y_pred = model.predict(X_test)
		
		accuracy = accuracy_score(y_test, y_pred)
		precision = precision_score(y_test, y_pred, average='weighted',zero_division=1)
		f1 = f1_score(y_test, y_pred, average='weighted',zero_division=1)
		
		metrics[name] = {
		    'Accuracy': accuracy,
		    'Precision': precision,
		    'F1-score': f1
		}

	# Grafico di Confronto delle Metriche
	if model_names is None:
		metrics_names = list(metrics[get_model_name(models[0])].keys())
	else:
		metrics_names = list(metrics[model_names[0]].keys())
		
	metrics_values = {name: list(m.values()) for name, m in metrics.items()}

	plt.figure(figsize=(10, 6))
	for name, values in metrics_values.items():
		plt.plot(metrics_names, values, marker='o', label=name)

	plt.ylim(0, 1.1)#metto un po più alto così si vedono meglio i valori a 1
	plt.ylabel('Metric Value')
	plt.title('Comparazione metriche')
	plt.legend()
	plt.show()

def conf_matrix_3(models,X_test,y_test,classes,title=None):#usata quando X_test e/o y_test è una lista, usata quando devo fare predict con set diversi
	plt.figure(figsize=(12, 12))
	for i,(model,xtest) in enumerate(zip(models,X_test)):
		ax = plt.subplot(math.ceil(len(models)/3)+1,math.ceil(len(models)/3),i+1)
		plt.title(f'Confusion Matrix Heatmap - {get_model_name(model)}')
		
		if title is not None:
			plt.title(f'Conf Matrix {get_model_name(model)} {title[i]}')
			
		y_pred = model.predict(xtest)
		
		if not isinstance(y_test, list):
			accuracy = accuracy_score(y_test, y_pred)
			precision = precision_score(y_test, y_pred, average='weighted',zero_division=1)
			f1 = f1_score(y_test, y_pred, average='weighted',zero_division=1)
			# Confusion Matrix 
			conf_matrix = confusion_matrix(y_test, y_pred)
		else:
			accuracy = accuracy_score(y_test[i], y_pred)
			precision = precision_score(y_test[i], y_pred, average='weighted',zero_division=1)
			f1 = f1_score(y_test[i], y_pred, average='weighted',zero_division=1)
			conf_matrix = confusion_matrix(y_test[i], y_pred)
		#Heatmap
		sns.heatmap(conf_matrix, annot=True, fmt='d', cmap='Blues', xticklabels=classes, yticklabels=classes)
		plt.xlabel('Predicted Label',fontsize=12)
		plt.xticks(rotation=45)
		plt.ylabel('True Label',fontsize=12)
		plt.yticks(rotation=0)

		plt.tight_layout()
	plt.show()

def curva_roc3(models,X_test,y_test,classes,title=None):
	plt.figure(figsize=(12, 12))
	for i,(model,xtest) in enumerate(zip(models,X_test)):
		ax = plt.subplot(math.ceil(len(models)/3)+1,math.ceil(len(models)/3),i+1)
		plt.title(f'Curva ROC {get_model_name(model)}')
		
		if title is not None:
			plt.title(f'Curva ROC {get_model_name(model)} {title[i]}')
		# Conversione y_test in etichette binarie per ciascuna classe (one-hot encoding)
		
		if not isinstance(y_test, list):
			y_test_bin = label_binarize(y_test, classes=classes)
		else:
			y_test_bin = label_binarize(y_test[i], classes=classes)
		
		# Calcola le probabilità predette per tutte le classi dal modello
		y_prob = model.predict_proba(xtest)

		# Calcola la curva ROC per tutte le classi
		for i in range(len(classes)):
			fpr, tpr, _ = roc_curve(y_test_bin[:, i], y_prob[:, i])
			roc_auc = auc(fpr, tpr)#area sottesa alla curva ROC (AUC - Area Under the Curve) 
			plt.plot(fpr, tpr, label=f'{classes[i]} (AUC = {roc_auc:.2f})')

		plt.plot([0, 1], [0, 1], 'k--', label='Random')
		plt.xlabel('FPR')#False Positive Rate 
		plt.ylabel('True Positive Rate (TPR)')
		
			
		plt.legend(loc='lower right')
		plt.grid(True)
		plt.tight_layout()
	plt.show()



def confronto_metriche3(models,X_test,y_test,model_names=None):
	# Valuta le metriche di valutazione per ogni modello
	metrics = {}
	for i,(model,xtest) in enumerate(zip(models,X_test)):
		name = get_model_name(model)
		if model_names is not None:
			name = model_names[i]
		y_pred = model.predict(xtest)
		
		if not isinstance(y_test, list):
			accuracy = accuracy_score(y_test, y_pred)
			precision = precision_score(y_test, y_pred, average='weighted',zero_division=1)
			f1 = f1_score(y_test, y_pred, average='weighted',zero_division=1)
		else:
			accuracy = accuracy_score(y_test[i], y_pred)
			precision = precision_score(y_test[i], y_pred, average='weighted',zero_division=1)
			f1 = f1_score(y_test[i], y_pred, average='weighted',zero_division=1)
		metrics[name] = {
		    'Accuracy': accuracy,
		    'Precision': precision,
		    'F1-score': f1
		}

	# Grafico di Confronto delle Metriche
	if model_names is None:
		metrics_names = list(metrics[models[0].__class__.__name__].keys())
	else:
		metrics_names = list(metrics[model_names[0]].keys())
		
	metrics_values = {name: list(m.values()) for name, m in metrics.items()}

	plt.figure(figsize=(10, 6))
	for name, values in metrics_values.items():
		plt.plot(metrics_names, values, marker='o', label=name)

	plt.ylim(0, 1.1)
	plt.ylabel('Metric Value')
	plt.title('Comparazione metriche')
	plt.legend(loc='lower right')
	#if model_names is not None:
	#plt.legend(model_names)
	plt.show()
