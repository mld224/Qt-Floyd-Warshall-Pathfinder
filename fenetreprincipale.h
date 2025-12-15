#ifndef FENETREPRINCIPALE_H
#define FENETREPRINCIPALE_H

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QListWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QVector>
#include <QMap>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>

class FenetrePrincipale : public QMainWindow
{
    Q_OBJECT

public:
    explicit FenetrePrincipale(QWidget *parent = nullptr);
    ~FenetrePrincipale();

private slots:
    void surCalculerTemps();
    void surGenererTableau();
    void surViderSelection();
    void surFiltrerVilles(const QString &texte);
    void surOrdreVisiteOptimal();

private:
    void initialiserInterface();
    void appliquerStyles();
    void chargerVilles();
    void chargerTempsTrajets();
    void initialiserMatriceDistances();
    void algorithmeFloydWarshall();
    int indiceVille(const QString &nomVille);
    QString reconstruireChemin(int u, int v);

    QComboBox *boiteDe;
    QComboBox *boiteVers;
    QPushButton *boutonCalculer;
    QPushButton *boutonVider;
    QPushButton *boutonGenererTableau;
    QPushButton *boutonOrdreOptimal;
    QListWidget *listeVilles;
    QTableView *vueTableau;
    QStandardItemModel *modeleTableau;
    QLineEdit *filtreVilles;

    QGroupBox *groupeSelection;
    QGroupBox *groupeTableau;
    QLabel *etiquetteTitre;

    QMap<QString, int> indicesVilles;
    QVector<QVector<int>> matriceDistances;
    QVector<QVector<int>> prochaineVille;
};

#endif // FENETREPRINCIPALE_H
