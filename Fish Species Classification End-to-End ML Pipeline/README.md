#  FISH SPECIES CLASSIFICATION END-TO-END ML PIPELINE

## Project Overview
This project implements an End-toEnd Machine Learning pipeline to classify **7 different fish species** based on ther physical attributes (weight, length, height. wifth).
The Goal was not just to train a model, but to conduct a **comparative analysis** of 7 different algorithms, optimizing them through **Data Scaling**, **Oversampling (SMOTE)**, and **Feature Selection** to solve class imbalance and overfitting issues.

## The Dataset
The dataset used is teh [Fish Market Dataset](https://www.kaggle.com/datasets/aungpyaeap/fish-market) from Kaggle.
* **Records:** 159 observations.
* **Target:** Species (Bram, Roach, Whitefish, Parkki, Perch, Pike, Smelt).
* **Features:** Weight, Length1 (Vertical), Length2 (Diagonal), Length3 (Cross), Height, Width.

## Methodologi & Pipeline

### 1. Exploratory Data Analysis (EDA)
* Analyzed attribute distribution and correlation matrix.
* Identified class imbalance (e.g., *Perch* had 56 samples, while *Whitefish* ha only 6).
* Handled missing/erroneous values (identified a record with Weight = 0).

### 2. Preprocessing & Optimization
The following techniuques were tested to improve model performance:
* **Scaling:** Compraison between *StandaloneScaler*, *MimMaxScaler*, and *Normalization*.
* **Balancing:** Applied **SMOTE (Synthetic Minority Over-sampling Technique)** to handle the imbalance of minority classes(Whitefish, Parkki).
*  **Feature Selection:** Iterative testing to identify the optimal number of features (Optimal result found at **4 features**).

### 3. Models Implemented
We trained and compared the following supervised learning algorithms:
* **Decision Tree**
* **Random Forest**
* **Support Vector Classifier (SVC)**
* **Naive Bayes**
* **MLP Classifier (Multi-Layer Perceptron Neural Network)**
* **Ensemble Methods:** Custom implementation of **Hard Voting** and **Soft Voting** classifiers.

| Model | Accuracy (Baseline) | Accuracy (Optimized*) |
| :--- | :---: | :---: |
| **SVC** | 96.8% | **96.9%** |
| **MLP Classifier** | 96.8% | **96.9%** |
| Random Forest | 81.2% | 82.1% |
| Naive Bayes | 53.1% | 62.5% |

*\*Optimized with SMOTE and Cross Validation.*

### Key Insights
* **SMOTE Impact:** Oversampling significantly improved the recognition of minority classes (like *Whitefish*), which were previously misclassified as *Perch*.
* **Feature Selection:** Reducing features from 6 to 4 maintained high accuracy while reducing model complexity.
* **Normalization:** Interestingly, stratified sampling did not always yield better results compared to random splitting for this specific small dataset.

## Key Results
The comparative analysis showet that **SVC** and **MLP (Neural Network)** were the top performers.

## Credits:
Developed by <br>
**Francesco Pili** - "https://github.com/francesco-pili-dev <br>
**Francesco Marotto** - <br>
**Stefano Lilliu** - <br>
for the *Machine Learning* exam in **University of Cagliari**
