# Qt-Floyd-Warshall-Pathfinder
Application d'optimisation de trajets et calcul de plus courts chemins (Algorithme de Floyd-Warshall) en C++ avec interface Qt rééalisée au 2ème semestre du BUT INFORMATIQUE 1.

#  Qt Floyd-Warshall Pathfinder

Application d'optimisation de trajets développée en **C++** avec l'interface graphique **Qt**. Ce projet a été réalisé dans le cadre du **BUT Informatique** (Module Graphes & Algorithmique).

Il implémente l'**Algorithme de Floyd-Warshall** pour calculer et visualiser les plus courts chemins entre différentes villes à partir d'un jeu de données.

##  Objectifs Techniques

Ce projet met en œuvre des concepts avancés d'algorithmique et de gestion de données :
* **Algorithme de Floyd-Warshall** : Implémentation de la méthode de programmation dynamique pour trouver les plus courts chemins dans un graphe pondéré (complexité $O(N^3)$).
* **Modélisation de Graphe** : Représentation des villes (nœuds) et des liaisons (arêtes pondérées par le temps ou la distance).
* **Matrice d'Adjacence** : Manipulation de matrices pour stocker et mettre à jour les coûts de trajet.
* **Interface Qt** : Visualisation des résultats et sélection des points de départ/arrivée.

##  Fonctionnalités

1.  **Chargement du Graphe** : Lecture des données depuis des fichiers CSV (`villes.csv` pour les nœuds, `temps.csv` pour les poids des arêtes).
2.  **Calcul des Chemins** : Exécution de l'algorithme pour générer la matrice des plus courts chemins.
3.  **Requêtes Utilisateur** : Sélection de deux villes pour obtenir :
    * Le temps de trajet optimal.
    * La distance minimale.
    * L'itinéraire étape par étape.

##  Structure des Données

L'application nécessite deux fichiers CSV structurés pour construire le graphe :

* **`villes.csv`** : Liste des sommets du graphe (Villes).
  * *Format :* `id_ville;nom_ville;latitude;longitude`
* **`temps.csv`** : Liste des arcs pondérés (Liaisons).
  * *Format :* `id_ville_depart;id_ville_arrivee;temps_trajet` (ou distance)

> **Note :** Ces fichiers doivent être placés dans le répertoire de l'exécutable pour que le graphe puisse être initialisé.

##  Installation et Compilation

### Prérequis
* **Qt Creator** (Kit Desktop MinGW ou MSVC).
* Compilateur C++ compatible C++11 ou supérieur.

### Compilation
1.  Clonez le dépôt :
    ```bash
    git clone [https://github.com/VOTRE-NOM/Qt-Floyd-Warshall-Pathfinder.git](https://github.com/VOTRE-NOM/Qt-Floyd-Warshall-Pathfinder.git)
    ```
2.  Ouvrez le fichier `SAE.pro` dans Qt Creator.
3.  Compilez et lancez le projet.
4.  Assurez-vous que les fichiers `.csv` sont bien accessibles par l'application.

##  Auteur
**Mamadou Lamine DIALLO** - Étudiant en BUT Informatique
