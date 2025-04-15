#ifndef STADE_H
#define STADE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QChartView>
#include <QPieSeries>

class Stade
{
public:
    Stade();
    Stade(QString nom, QString localisation, int capacite);

    int getId() const;
    void setId(int newId);

    bool ajouter();
    bool supprimer(int id);
    bool modifier();
    static QSqlQueryModel* afficher();
    static QSqlQueryModel* trierParCapacite();
    static QChartView* getStatsChart();
    static QSqlQueryModel* rechercher(const QString &searchText);

private:
    int id;
    QString nom;
    QString localisation;
    int capacite;
    bool idExist(int id);
};

#endif // STADE_H
