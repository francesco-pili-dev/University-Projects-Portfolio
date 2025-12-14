# 🐟 Fish Species Classification: Comparative ML Analysis

![Python](https://img.shields.io/badge/Python-3.x-blue?style=for-the-badge&logo=python&logoColor=white)
![Scikit-Learn](https://img.shields.io/badge/scikit--learn-F7931E?style=for-the-badge&logo=scikit-learn&logoColor=white)
![Pandas](https://img.shields.io/badge/Pandas-150458?style=for-the-badge&logo=pandas&logoColor=white)

## 📌 Project Overview
This project implements an End-to-End Machine Learning pipeline to classify **7 different fish species** based on their physical attributes (weight, length, height, width).

The goal was not just to train a model, but to conduct a **comparative analysis** of 7 different algorithms, optimizing them through **Data Scaling**, **Oversampling (SMOTE)**, and **Feature Selection** to solve class imbalance and overfitting issues.

## 📂 The Dataset
The dataset used is the [Fish Market Dataset](https://www.kaggle.com/datasets/aungpyaeap/fish-market) from Kaggle.
* **Records:** 159 observations.
* **Target:** Species (Bream, Roach, Whitefish, Parkki, Perch, Pike, Smelt).
* **Features:** Weight, Length1 (Vertical), Length2 (Diagonal), Length3 (Cross), Height, Width.

## ⚙️ Methodology & Pipeline

### 1. Exploratory Data Analysis (EDA)
* Analyzed attribute distribution and correlation matrix.
* Identified class imbalance (e.g., *Perch* had 56 samples, while *Whitefish* had only 6).
* Handled missing/erroneous values (identified a record with Weight=0).

### 2. Preprocessing & Optimization
The following techniques were tested to improve model performance:
* **Scaling:** Comparison between *StandardScaler*, *MinMaxScaler*, and *Normalization*.
* **Balancing:** Applied **SMOTE (Synthetic Minority Over-sampling Technique)** to handle the imbalance of the minority classes (Whitefish, Parkki).
* **Feature Selection:** Iterative testing to identify the optimal number of features (Optimal result found at **4 features**).

### 3. Models Implemented
We trained and compared the following supervised learning algorithms:
* **Decision Tree**
* **Random Forest**
* **Support Vector Classifier (SVC)**
* **Naive Bayes**
* **MLP Classifier (Multi-Layer Perceptron Neural Network)**
* **Ensemble Methods:** Custom implementation of **Hard Voting** and **Soft Voting** classifiers.

## 📊 Key Results
The comparative analysis showed that **SVC** and **MLP (Neural Network)** were the top performers.

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
 

## 👥 Credits
This project was developed as a team effort for the *Machine Learning* course at the University of Cagliari.

* **Francesco Pili** - [GitHub](https://github.com/FranCBrain)
* **Francesco Marotto** -
* **Stefano Lilliu** - 


