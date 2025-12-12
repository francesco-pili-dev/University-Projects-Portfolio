# Funzione per l'analisi dei dati e della loro qualità

import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np

global numeric_attributes 
numeric_attributes = ['Weight', 'Length1', 'Length2', 'Length3', 'Height', 'Width']

def features_box_plot(x):	#grafico feature box
	col_names = x.columns.values.tolist()
	# Si crea il box plot per ogni feature del dataset
	fig, ax = plt.subplots(num='Box-Plot Dataset Feature')
	ax.boxplot(x, notch=True, patch_artist=True)
	plt.title('Box Plot degli Attributi')

	plt.xticks(list(range(1, len(col_names) + 1)), col_names)

	for label in (ax.get_xticklabels() + ax.get_yticklabels()):
		label.set_fontsize(12)
	plt.show()
   
def conta_istanze(data):
	# Conta il numero di istanze per ciascuna specie
	species_count = data['Species'].value_counts()
	print("\n#Conteggio delle specie:")
	print(species_count)
	
	# Visualizza un grafico a barre del conteggio delle specie
	plt.figure(figsize=(8, 6))
	species_count.plot(kind='bar')
	plt.xlabel('Specie')
	plt.ylabel('Conteggio')
	plt.title('Conteggio delle specie di pesci')
	plt.xticks(rotation=45) #ruota label classi di 45 gradi per migliore lettura
	plt.show()
	
def zero_attrib(data):#conta attributi con valore zero
	print("\nValori mancanti nel dataset:")#di questo non faccio grafico perché non ci sono valori mancanti
	print(data.isnull().sum())
			
	# Zeri degli attributi
	print("\nAttributi a zero nel dataset:")
	zero_counts = data[numeric_attributes].apply(lambda x: np.sum(x == 0))#somma i valori a zero	
	print(zero_counts)
	
	plt.figure(figsize=(8, 6))	
	zero_counts.plot(kind='bar')
	plt.xlabel('Attributo')
	plt.ylabel('Conteggio di zeri')
	plt.title('Zeri degli attributi')
	plt.xticks(rotation=45)
	plt.show()

def corr_matrix(data):
	# Matrice di correlazione
	numeric_attributes = ['Weight', 'Length1', 'Length2', 'Length3', 'Height', 'Width']
	correlation_matrix = data[numeric_attributes].corr()

	plt.figure(figsize=(10, 8))
	sns.heatmap(correlation_matrix, annot=True, cmap='coolwarm')
	plt.title('Matrice di correlazione')
	plt.show()

def var_attrib(data):# Calcola la varianza degli attributi
	variance = data[numeric_attributes].var()

	# Grafico a barre
	plt.figure(figsize=(8, 6))
	variance.plot(kind='bar')
	plt.xlabel('Attributi')
	plt.ylabel('Varianza')
	plt.title('Varianza degli attributi')
	plt.xticks(rotation=45)
	plt.show()

def distrib_attrib(data):#distribuzione valori assunti dagli attributi(indifferentemente dalle classi)
	plt.figure(figsize=(12, 8))
	for i, attribute in enumerate(numeric_attributes):
		plt.subplot(2, 3, i + 1)
		sns.histplot(data[attribute], kde=True)
		plt.xlabel(attribute)
		plt.title(f'Distribuzione di {attribute}')
	plt.tight_layout()
	plt.show()
	
def box_plot_attrib(data):#box plot valori attributi
	x = data[numeric_attributes]
	col_names = x.columns.values.tolist()
	# Si crea il box plot per ogni feature del dataset
	fig, ax = plt.subplots(num='Box-Plot Dataset Feature')
	ax.boxplot(x, notch=True, patch_artist=True)
	
	plt.title('Box Plot degli Attributi')
	plt.xticks(list(range(1, len(col_names) + 1)), col_names)
	plt.show()
