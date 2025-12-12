# Progetto Machine Learning

Francesco Pili, Francesco Marotto, Stefano Lilliu - Università di Cagliari 2022/2023

## Fish Market Prediction System 🐟

Un sistema completo di Machine Learning per l'analisi, la classificazione e la regressione basato sul dataset *Fish Market*.
Il progetto confronta le performance di algoritmi standard (Scikit-learn) con algoritmi custom implementati da zero, valutando l'impatto di diverse tecniche di pre-processing.

## 🎯 Obiettivi del Progetto
L'obiettivo è individuare la combinazione ottimale (Modello + Pre-processing) per predire le caratteristiche delle specie ittiche, attraverso un'analisi comparativa rigorosa.

## 🛠 Tecnologie Utilizzate
- **Python 3.x**
- **Data Manipulation:** Pandas, NumPy
- **ML Libraries:** Scikit-learn (SVM, Random Forest, KNN, etc.)
- **Visualization:** Matplotlib, Seaborn

## ⚙️ Funzionalità Chiave

### 1. CLI Interattiva (User Menu)
Il software dispone di un'interfaccia a riga di comando che permette all'utente di:
- Selezionare il tipo di task (Classificazione o Regressione).
- Scegliere il modello predittivo (già ottimizzato).
- Eseguire analisi esplorative sul dataset.

### 2. Modelli Implementati (5 Tecniche)
Ho confrontato 5 approcci distinti:
- **3 Modelli Scikit-learn:** (Es. Decision Tree, SVM, KNN) con tuning degli iperparametri.
- **1 Custom Simple Estimator:** Un algoritmo di stima semplificato sviluppato ad hoc.
- **1 Custom Multiple Estimator:** Un algoritmo di ensemble custom per migliorare la robustezza delle predizioni.

### 3. Pipeline di Sperimentazione
Il progetto analizza come variano le performance applicando diverse trasformazioni ai dati:
- **Standardizzazione:** Confronto performance con/senza scaling dei dati.
- **Feature Engineering:** Confronto con/senza Selezione o Aggregazione delle features.
- **Data Balancing:** Gestione delle classi sbilanciate tramite tecniche di Undersampling e Oversampling.

## 📊 Risultati e Metodologia
*(Qui puoi inserire una breve frase sui risultati, ad esempio:)*
L'analisi ha mostrato che la combinazione di [Nome Modello] con [Tecnica, es. Standardizzazione] ha prodotto i risultati migliori, con un'accuratezza del XX%.
Tutti i passaggi di ottimizzazione degli iperparametri sono documentati nel codice di training separato.

## 👥 Il Team
Questo progetto è stato realizzato in collaborazione accademica da:

* **Francesco Pili** - [GitHub](https://github.com/FranCBrain)
* **Francesco Marotto** -
* **Stefano Lilliu** - 


## 🚀 Come eseguire il codice

