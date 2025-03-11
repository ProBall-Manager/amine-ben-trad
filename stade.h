#ifndef STADE_H
#define STADE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

class Stade {
public:
    Stade();
    Stade(QString nom, QString localisation, int capacite);

    bool ajouter();
    bool idExist(int id);
    bool supprimer(int id);
    bool modifier();
    static QSqlQueryModel* afficher();  // Fonction pour afficher les stades

    void setId(int id) { this->id = id; }
    void setNom(const QString& nom) { this->nom = nom; }
    void setLocalisation(const QString& localisation) { this->localisation = localisation; }
    void setCapacite(int capacite) { this->capacite = capacite; }

    int getId() const { return id; }
    QString getNom() const { return nom; }
    QString getLocalisation() const { return localisation; }
    int getCapacite() const { return capacite; }

private:
    int id;
    QString nom;
    QString localisation;
    int capacite;
};

#endif // STADE_H
