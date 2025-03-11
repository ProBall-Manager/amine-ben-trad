#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Stade
{
public:
    Stade();
    Stade(int, QString, QString, QString);

    int getId();
    QString getNom();
    QString getLieu();
    QString getStatut();

    void setId(int);
    void setNom(QString);
    void setLieu(QString);
    void setStatut(QString);

    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimer(int);
    bool modifier(int);

private:
    int id_terrain;
    QString nom_terrain, lieu, statut;
};

