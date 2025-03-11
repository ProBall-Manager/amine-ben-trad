#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

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

void MainWindow::on_pushButton_ajouter_clicked()
{
    int id = ui->lineEdit_id->text().toInt();
    QString name = ui->lineEdit_name->text().trimmed();
    QString location = ui->lineEdit_location->text().trimmed();
    int capacity = ui->lineEdit_capacity->text().toInt();

    Stade newStade(name, location, capacity);
    newStade.setId(id);

    if (newStade.ajouter()) {
        QMessageBox::information(this, "Success", "Stadium added successfully!");
        loadStadiums();
    } else {
        QMessageBox::critical(this, "Error", "id existe");
    }
}

void MainWindow::on_pushButton_modifier_clicked()
{
    int id = ui->lineEdit_id->text().toInt();
    QString name = ui->lineEdit_name->text().trimmed();
    QString location = ui->lineEdit_location->text().trimmed();
    int capacity = ui->lineEdit_capacity->text().toInt();

    Stade stade(name, location, capacity);
    stade.setId(id);

    if (stade.modifier()) {
        QMessageBox::information(this, "Success", "Stadium modified successfully!");
        loadStadiums();
    } else {
        QMessageBox::critical(this, "Error", "Failed to modify stadium.");
    }
}

void MainWindow::on_pushButton_supprimer_clicked()
{
    int id = ui->lineEdit_id->text().toInt();

    if (Stade().supprimer(id)) {
        QMessageBox::information(this, "Success", "Stadium deleted successfully!");
        loadStadiums();
    } else {
        QMessageBox::critical(this, "Error", "Failed to delete stadium.");
    }
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    if (!index.isValid()) return; // Ensure the index is valid

    // Get the ID of the selected row
    int row = index.row();
    QString id = ui->tableView->model()->index(row, 0).data().toString(); // Assuming ID is in column 0

    // Fetch stadium details from the database
    QSqlQuery query;
    query.prepare("SELECT ID, NOM, LOCALISATION, CAPACITE FROM STADES WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        // Auto-fill input fields with correct column names
        ui->lineEdit_id->setText(query.value("ID").toString());
        ui->lineEdit_name->setText(query.value("NOM").toString());
        ui->lineEdit_location->setText(query.value("LOCALISATION").toString());
        ui->lineEdit_capacity->setText(query.value("CAPACITE").toString());
    } else {
        QMessageBox::warning(this, "Error", "Failed to fetch stadium details.");
    }
}


