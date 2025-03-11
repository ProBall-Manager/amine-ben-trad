<<<<<<< HEAD
#include "stade.h"

Stade::Stade() : id(0), nom(""), localisation(""), capacite(0) {}

Stade::Stade(QString nom, QString localisation, int capacite)
    : id(0), nom(nom), localisation(localisation), capacite(capacite) {}

bool Stade::ajouter() {
    QSqlQuery query;

    // Vérifier si l'ID existe déjà
    if (idExist(id)) {
        qDebug() << "ID already exists!";
        return false;
    }

    query.prepare("INSERT INTO STADES (ID, NOM, LOCALISATION, CAPACITE) VALUES (:id, :nom, :localisation, :capacite)");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":localisation", localisation);
    query.bindValue(":capacite", capacite);

    if (!query.exec()) {
        qDebug() << "Error adding stadium:" << query.lastError();
        return false;
    }
    return true;
}

bool Stade::idExist(int id) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM STADES WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error checking ID existence:" << query.lastError();
        return false;
    }

    query.next();
    return query.value(0).toInt() > 0;
}

bool Stade::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM STADES WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error deleting stadium:" << query.lastError();
        return false;
    }
    return true;
}

bool Stade::modifier() {
    QSqlQuery query;
    query.prepare("UPDATE STADES SET NOM = :nom, LOCALISATION = :localisation, CAPACITE = :capacite WHERE ID = :id");
    query.bindValue(":nom", nom);
    query.bindValue(":localisation", localisation);
    query.bindValue(":capacite", capacite);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error modifying stadium:" << query.lastError();
        return false;
    }
    return true;
}

QSqlQueryModel* Stade::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID, NOM, LOCALISATION, CAPACITE FROM STADES");
    return model;
}
=======
#include "stade.h"

Stade::Stade() : id(0), nom(""), localisation(""), capacite(0) {}

Stade::Stade(QString nom, QString localisation, int capacite)
    : id(0), nom(nom), localisation(localisation), capacite(capacite) {}

bool Stade::ajouter() {
    QSqlQuery query;

    // Vérifier si l'ID existe déjà
    if (idExist(id)) {
        qDebug() << "ID already exists!";
        return false;
    }

    query.prepare("INSERT INTO STADES (ID, NOM, LOCALISATION, CAPACITE) VALUES (:id, :nom, :localisation, :capacite)");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":localisation", localisation);
    query.bindValue(":capacite", capacite);

    if (!query.exec()) {
        qDebug() << "Error adding stadium:" << query.lastError();
        return false;
    }
    return true;
}

bool Stade::idExist(int id) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM STADES WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error checking ID existence:" << query.lastError();
        return false;
    }

    query.next();
    return query.value(0).toInt() > 0;
}

bool Stade::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM STADES WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error deleting stadium:" << query.lastError();
        return false;
    }
    return true;
}

bool Stade::modifier() {
    QSqlQuery query;
    query.prepare("UPDATE STADES SET NOM = :nom, LOCALISATION = :localisation, CAPACITE = :capacite WHERE ID = :id");
    query.bindValue(":nom", nom);
    query.bindValue(":localisation", localisation);
    query.bindValue(":capacite", capacite);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error modifying stadium:" << query.lastError();
        return false;
    }
    return true;
}

QSqlQueryModel* Stade::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID, NOM, LOCALISATION, CAPACITE FROM STADES");
    return model;
}
>>>>>>> e4e7cd8 (first commit)
