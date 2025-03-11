#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadStadiums();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadStadiums()
{
    ui->tableView->setModel(Stade::afficher());
}

// Fonction pour vérifier si une chaîne contient uniquement des lettres
bool isOnlyLetters(const QString &text)
{
    return QRegularExpression("^[A-Za-zÀ-ÖØ-öø-ÿ\\s]+$").match(text).hasMatch();
}

// Fonction pour vérifier si une chaîne contient uniquement des chiffres
bool isOnlyNumbers(const QString &text)
{
    return QRegularExpression("^[0-9]+$").match(text).hasMatch();
}

void MainWindow::on_pushButton_ajouter_clicked()
{
    QString idStr = ui->lineEdit_id->text().trimmed();
    QString name = ui->lineEdit_name->text().trimmed();
    QString location = ui->lineEdit_location->text().trimmed();
    QString capacityStr = ui->lineEdit_capacity->text().trimmed();

    // Vérification des champs vides
    if (idStr.isEmpty() || name.isEmpty() || location.isEmpty() || capacityStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis !");
        return;
    }

    // Vérification des entrées
    if (!isOnlyNumbers(idStr) || !isOnlyNumbers(capacityStr)) {
        QMessageBox::warning(this, "Erreur", "L'ID et la capacité doivent contenir uniquement des chiffres !");
        return;
    }

    if (!isOnlyLetters(name) || !isOnlyLetters(location)) {
        QMessageBox::warning(this, "Erreur", "Le nom et la localisation doivent contenir uniquement des lettres !");
        return;
    }

    int id = idStr.toInt();
    int capacity = capacityStr.toInt();

    Stade newStade(name, location, capacity);
    newStade.setId(id);

    if (newStade.ajouter()) {
        QMessageBox::information(this, "Succès", "Stade ajouté avec succès !");
        loadStadiums();
        ui->lineEdit_id->clear();
        ui->lineEdit_name->clear();
        ui->lineEdit_location->clear();
        ui->lineEdit_capacity->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "L'ID existe déjà !");
    }
}

void MainWindow::on_pushButton_modifier_clicked()
{
    QString idStr = ui->lineEdit_id->text().trimmed();
    QString name = ui->lineEdit_name->text().trimmed();
    QString location = ui->lineEdit_location->text().trimmed();
    QString capacityStr = ui->lineEdit_capacity->text().trimmed();

    // Vérification des champs vides
    if (idStr.isEmpty() || name.isEmpty() || location.isEmpty() || capacityStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis !");
        return;
    }

    // Vérification des entrées
    if (!isOnlyNumbers(idStr) || !isOnlyNumbers(capacityStr)) {
        QMessageBox::warning(this, "Erreur", "L'ID et la capacité doivent contenir uniquement des chiffres !");
        return;
    }

    if (!isOnlyLetters(name) || !isOnlyLetters(location)) {
        QMessageBox::warning(this, "Erreur", "Le nom et la localisation doivent contenir uniquement des lettres !");
        return;
    }

    int id = idStr.toInt();
    int capacity = capacityStr.toInt();

    Stade stade(name, location, capacity);
    stade.setId(id);

    if (stade.modifier()) {
        QMessageBox::information(this, "Succès", "Stade modifié avec succès !");
        loadStadiums();
        ui->lineEdit_id->clear();
        ui->lineEdit_name->clear();
        ui->lineEdit_location->clear();
        ui->lineEdit_capacity->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification du stade.");
    }
}

void MainWindow::on_pushButton_supprimer_clicked()
{
    QString idStr = ui->lineEdit_id->text().trimmed();

    // Vérification champ vide
    if (idStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "L'ID du stade à supprimer est requis !");
        return;
    }

    // Vérification ID uniquement en chiffres
    if (!isOnlyNumbers(idStr)) {
        QMessageBox::warning(this, "Erreur", "L'ID doit contenir uniquement des chiffres !");
        return;
    }

    int id = idStr.toInt();

    if (Stade().supprimer(id)) {
        QMessageBox::information(this, "Succès", "Stade supprimé avec succès !");
        loadStadiums();
        ui->lineEdit_id->clear();
        ui->lineEdit_name->clear();
        ui->lineEdit_location->clear();
        ui->lineEdit_capacity->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression du stade.");
    }
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    if (!index.isValid()) return;

    int row = index.row();
    QString id = ui->tableView->model()->index(row, 0).data().toString();

    QSqlQuery query;
    query.prepare("SELECT ID, NOM, LOCALISATION, CAPACITE FROM STADES WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        ui->lineEdit_id->setText(query.value("ID").toString());
        ui->lineEdit_name->setText(query.value("NOM").toString());
        ui->lineEdit_location->setText(query.value("LOCALISATION").toString());
        ui->lineEdit_capacity->setText(query.value("CAPACITE").toString());
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de récupérer les détails du stade.");
    }
}
