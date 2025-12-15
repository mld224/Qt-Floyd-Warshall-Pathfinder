#include "fenetreprincipale.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QStyleFactory>

const int INF = 1e9;

FenetrePrincipale::FenetrePrincipale(QWidget *parent) : QMainWindow(parent)
{
    initialiserInterface();
    appliquerStyles();
    chargerVilles();
    initialiserMatriceDistances();
    chargerTempsTrajets();
    algorithmeFloydWarshall();
}

FenetrePrincipale::~FenetrePrincipale()
{
    delete modeleTableau;
}

void FenetrePrincipale::initialiserInterface()
{
    QWidget *widgetCentral = new QWidget(this);
    QVBoxLayout *layoutPrincipal = new QVBoxLayout(widgetCentral);

    etiquetteTitre = new QLabel("Calculateur de temps de trajet", this);
    etiquetteTitre->setAlignment(Qt::AlignCenter);
    etiquetteTitre->setStyleSheet("font-size: 20px; font-weight: bold; padding: 10px; color: white;");

    groupeSelection = new QGroupBox("Sélection des villes");
    QHBoxLayout *layoutCombo = new QHBoxLayout();

    boiteDe = new QComboBox();
    boiteVers = new QComboBox();
    boiteDe->setFixedWidth(150);
    boiteVers->setFixedWidth(150);

    boutonCalculer = new QPushButton("Calculer");
    boutonCalculer->setFixedWidth(100);
    boutonVider = new QPushButton("Vider");
    boutonVider->setFixedWidth(100);
    boutonVider->setStyleSheet("background-color: #e74c3c; color: white;");

    layoutCombo->addWidget(new QLabel("De :"));
    layoutCombo->addWidget(boiteDe);
    layoutCombo->addWidget(new QLabel("À :"));
    layoutCombo->addWidget(boiteVers);
    layoutCombo->addWidget(boutonCalculer);
    layoutCombo->addWidget(boutonVider);
    groupeSelection->setLayout(layoutCombo);

    groupeTableau = new QGroupBox();
    groupeTableau->setTitle("Veuillez choisir un ensemble de villes pour connaître leur chemin");
    QVBoxLayout *layoutTableau = new QVBoxLayout();

    filtreVilles = new QLineEdit();
    filtreVilles->setPlaceholderText("Filtrer les villes...");

    listeVilles = new QListWidget();
    listeVilles->setSelectionMode(QAbstractItemView::MultiSelection);

    boutonGenererTableau = new QPushButton("Générer le tableau");
    boutonOrdreOptimal = new QPushButton("Ordre optimal de visite");

    vueTableau = new QTableView();
    modeleTableau = new QStandardItemModel(this);
    vueTableau->setModel(modeleTableau);
    vueTableau->setStyleSheet("background-color: black; color: white; gridline-color: black;");
    vueTableau->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    vueTableau->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    layoutTableau->addWidget(filtreVilles);
    layoutTableau->addWidget(listeVilles);
    layoutTableau->addWidget(boutonGenererTableau);
    layoutTableau->addWidget(boutonOrdreOptimal);
    layoutTableau->addWidget(vueTableau);
    groupeTableau->setLayout(layoutTableau);

    layoutPrincipal->addWidget(etiquetteTitre);
    layoutPrincipal->addWidget(groupeSelection);
    layoutPrincipal->addWidget(groupeTableau);
    setCentralWidget(widgetCentral);
    resize(900, 650);

    connect(boutonCalculer, &QPushButton::clicked, this, &FenetrePrincipale::surCalculerTemps);
    connect(boutonGenererTableau, &QPushButton::clicked, this, &FenetrePrincipale::surGenererTableau);
    connect(boutonVider, &QPushButton::clicked, this, &FenetrePrincipale::surViderSelection);
    connect(filtreVilles, &QLineEdit::textChanged, this, &FenetrePrincipale::surFiltrerVilles);
    connect(boutonOrdreOptimal, &QPushButton::clicked, this, &FenetrePrincipale::surOrdreVisiteOptimal);
}

void FenetrePrincipale::appliquerStyles()
{
    setStyle(QStyleFactory::create("Fusion"));
    this->setStyleSheet(R"(
        QMainWindow { background-color: #121212; }
        QGroupBox {
            font-size: 16px;
            font-weight: bold;
            padding: 10px;
            color: white;
            border: 1px solid #444;
        }
        QPushButton {
            font-size: 14px;
            background-color: #4CAF50;
            color: black;
            padding: 8px;
        }
        QComboBox, QLineEdit, QListWidget, QLabel {
            font-size: 14px;
            color: white;
            background-color: #1e1e1e;
        }
    )");
}

void FenetrePrincipale::chargerVilles()
{
    QFile fichier("villes.csv");
    if (!fichier.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Erreur", "Fichier villes.csv non trouvé !");
        return;
    }

    QTextStream in(&fichier);
    in.readLine();
    while (!in.atEnd()) {
        QString ligne = in.readLine().trimmed();
        if (ligne.isEmpty()) continue;

        QStringList morceaux = ligne.split(',');
        if (morceaux.size() < 1) continue;

        QString ville = morceaux[0].trimmed();
        if (ville.isEmpty()) continue;

        if (!indicesVilles.contains(ville)) {
            indicesVilles[ville] = boiteDe->count();
            boiteDe->addItem(ville);
            boiteVers->addItem(ville);
            listeVilles->addItem(ville);
        }
    }
    fichier.close();
}

void FenetrePrincipale::chargerTempsTrajets()
{
    QFile fichier("temps.csv");
    if (!fichier.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Erreur", "Fichier temps.csv non trouvé !");
        return;
    }

    QTextStream in(&fichier);
    while (!in.atEnd()) {
        QString ligne = in.readLine().trimmed();
        if (ligne.isEmpty()) continue;

        QStringList morceaux = ligne.split(',');
        if (morceaux.size() < 3) continue;

        QString depart = morceaux[0].trimmed();
        QString arrivee = morceaux[1].trimmed();
        int temps = morceaux[2].toInt();

        int u = indiceVille(depart);
        int v = indiceVille(arrivee);
        if (u == -1 || v == -1) continue;

        matriceDistances[u][v] = temps;
        matriceDistances[v][u] = temps;
        prochaineVille[u][v] = v;
        prochaineVille[v][u] = u;
    }
    fichier.close();
}

void FenetrePrincipale::initialiserMatriceDistances()
{
    int n = indicesVilles.size();
    matriceDistances.resize(n, QVector<int>(n, INF));
    prochaineVille.resize(n, QVector<int>(n, -1));
    for (int i = 0; i < n; ++i)
        matriceDistances[i][i] = 0;
}

void FenetrePrincipale::algorithmeFloydWarshall()
{
    int n = indicesVilles.size();
    for (int k = 0; k < n; ++k)
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (matriceDistances[i][k] + matriceDistances[k][j] < matriceDistances[i][j]) {
                    matriceDistances[i][j] = matriceDistances[i][k] + matriceDistances[k][j];
                    prochaineVille[i][j] = prochaineVille[i][k];
                }
}

int FenetrePrincipale::indiceVille(const QString &nomVille)
{
    return indicesVilles.contains(nomVille) ? indicesVilles[nomVille] : -1;
}

QString FenetrePrincipale::reconstruireChemin(int u, int v)
{
    if (prochaineVille[u][v] == -1) return "";
    QStringList chemin;
    while (u != v) {
        chemin << indicesVilles.key(u);
        u = prochaineVille[u][v];
    }
    chemin << indicesVilles.key(v);
    return chemin.join(" -> ");
}

void FenetrePrincipale::surCalculerTemps()
{
    QString de = boiteDe->currentText();
    QString vers = boiteVers->currentText();
    int u = indiceVille(de);
    int v = indiceVille(vers);

    if (u == -1 || v == -1) {
        QMessageBox::warning(this, "Erreur", "Ville non trouvée !");
        return;
    }

    int temps = matriceDistances[u][v];
    QString chemin = reconstruireChemin(u, v);
    if (temps == INF)
        QMessageBox::information(this, "Résultat", "Aucun chemin trouvé.");
    else
        QMessageBox::information(this, "Résultat", QString("Temps : %1 min\nChemin : %2").arg(temps).arg(chemin));
}

void FenetrePrincipale::surGenererTableau()
{
    QStringList villesSelectionnees;
    for (QListWidgetItem *item : listeVilles->selectedItems())
        villesSelectionnees << item->text();

    int n = villesSelectionnees.size();
    modeleTableau->clear();
    modeleTableau->setRowCount(n);
    modeleTableau->setColumnCount(n);

    for (int i = 0; i < n; ++i) {
        modeleTableau->setHorizontalHeaderItem(i, new QStandardItem(villesSelectionnees[i]));
        modeleTableau->setVerticalHeaderItem(i, new QStandardItem(villesSelectionnees[i]));
    }

    for (int i = 0; i < n; ++i) {
        int u = indiceVille(villesSelectionnees[i]);
        for (int j = 0; j < n; ++j) {
            int v = indiceVille(villesSelectionnees[j]);
            int t = matriceDistances[u][v];
            modeleTableau->setItem(i, j, new QStandardItem(t == INF ? "-" : QString::number(t)));
        }
    }
}

void FenetrePrincipale::surViderSelection()
{
    listeVilles->clearSelection();
}

void FenetrePrincipale::surFiltrerVilles(const QString &texte)
{
    for (int i = 0; i < listeVilles->count(); ++i) {
        QListWidgetItem *item = listeVilles->item(i);
        item->setHidden(!item->text().contains(texte, Qt::CaseInsensitive));
    }
}

void FenetrePrincipale::surOrdreVisiteOptimal()
{
    QStringList villesSelectionnees;
    for (QListWidgetItem *item : listeVilles->selectedItems())
        villesSelectionnees << item->text();

    if (villesSelectionnees.size() < 2) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez au moins deux villes.");
        return;
    }

    QVector<int> indices;
    QMap<int, QString> correspondanceInverse;
    for (const QString &ville : villesSelectionnees) {
        int idx = indiceVille(ville);
        if (idx != -1) {
            indices.append(idx);
            correspondanceInverse[idx] = ville;
        }
    }

    QVector<bool> visite(matriceDistances.size(), false);
    QVector<int> chemin;
    int courant = indices[0];
    chemin.append(courant);
    visite[courant] = true;

    while (chemin.size() < indices.size()) {
        int prochain = -1;
        int meilleur = INF;
        for (int i : indices) {
            if (!visite[i] && matriceDistances[courant][i] < meilleur) {
                meilleur = matriceDistances[courant][i];
                prochain = i;
            }
        }
        if (prochain == -1) {
            QMessageBox::warning(this, "Erreur", "Impossible de relier toutes les villes sélectionnées.");
            return;
        }
        chemin.append(prochain);
        visite[prochain] = true;
        courant = prochain;
    }

    int total = 0;
    QStringList cheminAffichage;
    for (int i = 0; i < chemin.size(); ++i) {
        cheminAffichage << correspondanceInverse[chemin[i]];
        if (i + 1 < chemin.size())
            total += matriceDistances[chemin[i]][chemin[i + 1]];
    }

    QMessageBox::information(this, "Ordre optimal de visite",
                             QString("Ordre : %1\n\nDurée totale : %2 minutes")
                                 .arg(cheminAffichage.join(" -> "))
                                 .arg(total));
}
