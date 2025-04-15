#include "stade.h"
#include <QSqlError>
#include <QDebug>
#include <QPieSlice>

Stade::Stade() : id(0), nom(""), localisation(""), capacite(0) {}

Stade::Stade(QString nom, QString localisation, int capacite)
    : id(0), nom(nom), localisation(localisation), capacite(capacite) {}

int Stade::getId() const { return id; }
void Stade::setId(int newId) { id = newId; }

bool Stade::ajouter()
{
    QSqlQuery query;
    if(idExist(id)) {
        qDebug() << "ID already exists!";
        return false;
    }

    query.prepare("INSERT INTO STADES (ID, NOM, LOCALISATION, CAPACITE) VALUES (:id, :nom, :localisation, :capacite)");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":localisation", localisation);
    query.bindValue(":capacite", capacite);

    if(!query.exec()) {
        qDebug() << "Error adding stadium:" << query.lastError();
        return false;
    }
    return true;
}

bool Stade::idExist(int id)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM STADES WHERE ID = :id");
    query.bindValue(":id", id);

    if(!query.exec()) {
        qDebug() << "Error checking ID existence:" << query.lastError();
        return false;
    }

    query.next();
    return query.value(0).toInt() > 0;
}

bool Stade::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM STADES WHERE ID = :id");
    query.bindValue(":id", id);

    if(!query.exec()) {
        qDebug() << "Error deleting stadium:" << query.lastError();
        return false;
    }
    return true;
}

bool Stade::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE STADES SET NOM = :nom, LOCALISATION = :localisation, CAPACITE = :capacite WHERE ID = :id");
    query.bindValue(":nom", nom);
    query.bindValue(":localisation", localisation);
    query.bindValue(":capacite", capacite);
    query.bindValue(":id", id);

    if(!query.exec()) {
        qDebug() << "Error modifying stadium:" << query.lastError();
        return false;
    }
    return true;
}

QSqlQueryModel* Stade::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID, NOM, LOCALISATION, CAPACITE FROM STADES");
    return model;
}

QSqlQueryModel* Stade::trierParCapacite()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID, NOM, LOCALISATION, CAPACITE FROM STADES ORDER BY CAPACITE ASC");
    return model;
}

QChartView* Stade::getStatsChart()
{
    QSqlQuery query;
    query.exec("SELECT CAPACITE, COUNT(*) FROM STADES GROUP BY CAPACITE");

    QPieSeries *series = new QPieSeries();
    int total = 0;

    while(query.next()) {
        int capacity = query.value(0).toInt();
        int count = query.value(1).toInt();
        total += count;
        QPieSlice *slice = series->append(QString("Capacité %1").arg(capacity), count);
        slice->setLabelVisible(true);
    }

    for(QPieSlice *slice : series->slices()) {
        double percentage = (slice->value() / total) * 100.0;
        slice->setLabel(QString("%1 (%2%)").arg(slice->label(), QString::number(percentage, 'f', 1)));
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des capacités des stades");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(640, 480);

    return chartView;
}

QSqlQueryModel* Stade::rechercher(const QString &searchText)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT ID, NOM, LOCALISATION, CAPACITE FROM STADES "
                  "WHERE NOM LIKE :search OR LOCALISATION LIKE :search");
    query.bindValue(":search", "%" + searchText + "%");

    if(!query.exec()) {
        qDebug() << "Search error:" << query.lastError();
    }

    model->setQuery(query);
    return model;
}
