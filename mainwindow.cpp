#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stade.h"
#include "zonemap.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDateTime>
#include <QSqlQuery>
#include <QPixmap>
#include <QPrinter>
#include <QPainter>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QChartView>
#include <QToolTip>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadStadiums();

    // Connect signals
    connect(ui->lineEdit_search, &QLineEdit::returnPressed,
            this, &MainWindow::on_lineEdit_search_returnPressed);
    connect(ui->lineEdit_search, &QLineEdit::textChanged,
            this, &MainWindow::on_lineEdit_search_textChanged);

    // Ajouté pour l'homologation
    connect(ui->pushButton_verify, &QPushButton::clicked,
            this, &MainWindow::on_pushButton_verify_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadStadiums()
{
    ui->tableView2->setModel(Stade::afficher());
    ui->tableView2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView2->setSelectionMode(QAbstractItemView::SingleSelection);
}

bool MainWindow::isNumber(const QString &str)
{
    bool ok;
    str.toInt(&ok);
    return ok;
}

bool MainWindow::isTextOnly(const QString &str)
{
    return QRegularExpression("^[a-zA-ZÀ-ÿ ]+$").match(str).hasMatch();
}

void MainWindow::on_pushButton_ajouter_clicked()
{
    QString idStr = ui->lineEdit_id->text().trimmed();
    QString name = ui->lineEdit_name->text().trimmed();
    QString location = ui->lineEdit_location->text().trimmed();
    QString capacityStr = ui->lineEdit_capacity->text().trimmed();

    if(idStr.isEmpty() || name.isEmpty() || location.isEmpty() || capacityStr.isEmpty()){
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis");
        return;
    }

    if(!isNumber(idStr) || !isNumber(capacityStr)){
        QMessageBox::warning(this, "Erreur", "ID et Capacité doivent être des nombres");
        return;
    }

    if(!isTextOnly(name) || !isTextOnly(location)){
        QMessageBox::warning(this, "Erreur", "Nom et Localisation doivent contenir seulement des lettres");
        return;
    }

    Stade s(name, location, capacityStr.toInt());
    s.setId(idStr.toInt());

    if(s.ajouter()){
        QMessageBox::information(this, "Succès", "Stade ajouté");
        loadStadiums();
        ui->lineEdit_id->clear();
        ui->lineEdit_name->clear();
        ui->lineEdit_location->clear();
        ui->lineEdit_capacity->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "ID déjà existant");
    }
}

void MainWindow::on_pushButton_modifier_clicked()
{
    QString idStr = ui->lineEdit_id->text().trimmed();
    QString name = ui->lineEdit_name->text().trimmed();
    QString location = ui->lineEdit_location->text().trimmed();
    QString capacityStr = ui->lineEdit_capacity->text().trimmed();

    if(idStr.isEmpty() || name.isEmpty() || location.isEmpty() || capacityStr.isEmpty()){
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis");
        return;
    }

    if(!isNumber(idStr) || !isNumber(capacityStr)){
        QMessageBox::warning(this, "Erreur", "ID et Capacité doivent être des nombres");
        return;
    }

    if(!isTextOnly(name) || !isTextOnly(location)){
        QMessageBox::warning(this, "Erreur", "Nom et Localisation doivent contenir seulement des lettres");
        return;
    }

    Stade s(name, location, capacityStr.toInt());
    s.setId(idStr.toInt());

    if(s.modifier()){
        QMessageBox::information(this, "Succès", "Stade modifié");
        loadStadiums();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification");
    }
}

void MainWindow::on_pushButton_supprimer_clicked()
{
    QString idStr = ui->lineEdit_id->text().trimmed();

    if(idStr.isEmpty()){
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID");
        return;
    }

    if(!isNumber(idStr)){
        QMessageBox::warning(this, "Erreur", "L'ID doit être un nombre");
        return;
    }

    if(Stade().supprimer(idStr.toInt())){
        QMessageBox::information(this, "Succès", "Stade supprimé");
        loadStadiums();
        ui->lineEdit_id->clear();
        ui->lineEdit_name->clear();
        ui->lineEdit_location->clear();
        ui->lineEdit_capacity->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression");
    }
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    if(!index.isValid()) return;

    int row = index.row();
    QAbstractItemModel *model = ui->tableView2->model();

    ui->lineEdit_id->setText(model->index(row, 0).data().toString());
    ui->lineEdit_name->setText(model->index(row, 1).data().toString());
    ui->lineEdit_location->setText(model->index(row, 2).data().toString());
    ui->lineEdit_capacity->setText(model->index(row, 3).data().toString());
}

void MainWindow::on_pushButton_trier_clicked()
{
    ui->tableView2->setModel(Stade::trierParCapacite());
}

void MainWindow::on_pushButton_stats_clicked()
{
    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("Statistiques des capacités");
    statsDialog->setMinimumSize(700, 500);

    QVBoxLayout *layout = new QVBoxLayout(statsDialog);
    QChartView *chartView = Stade::getStatsChart();
    layout->addWidget(chartView);

    statsDialog->exec();
}

void MainWindow::on_pushButton_pdf_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF",
                                                    QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".pdf",
                                                    "Fichiers PDF (*.pdf)");

    if (!fileName.isEmpty()) {
        exportToPdf(fileName);
        QMessageBox::information(this, "Succès", "Export PDF terminé avec succès!");
    }
}

void MainWindow::exportToPdf(const QString &fileName)
{
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setOutputFileName(fileName);

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::warning(this, "Erreur", "Impossible de créer le fichier PDF");
        return;
    }

    // Draw title
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(100, 50, "Rapport des Stades - Proball Manager");

    // Draw date
    painter.setFont(QFont("Arial", 10));
    painter.drawText(100, 80, QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"));

    // Draw table (starting at y=100)
    QPixmap tablePixmap = ui->tableView2->grab();
    painter.drawPixmap(50, 100, tablePixmap.scaledToWidth(printer.width() - 100, Qt::SmoothTransformation));

    // Draw statistics chart (starting after table)
    int chartY = 100 + tablePixmap.height() + 20;

    QDialog chartDialog;
    QVBoxLayout layout(&chartDialog);
    QChartView *chartView = Stade::getStatsChart();
    chartView->setMinimumSize(600, 400);
    layout.addWidget(chartView);
    chartDialog.show();

    QPixmap chartPixmap = chartView->grab();
    painter.drawPixmap(50, chartY, chartPixmap.scaledToWidth(printer.width() - 100, Qt::SmoothTransformation));

    // Footer
    painter.setFont(QFont("Arial", 8));
    painter.drawText(50, printer.height() - 30, "Généré par Proball Manager");

    painter.end();
}

void MainWindow::on_pushButton_search_clicked()
{
    QString searchText = ui->lineEdit_search->text().trimmed();
    if(searchText.isEmpty()) {
        loadStadiums(); // Reset view if search is empty
    } else {
        ui->tableView2->setModel(Stade::rechercher(searchText));
    }
}

void MainWindow::on_lineEdit_search_returnPressed()
{
    on_pushButton_search_clicked(); // Same functionality when pressing Enter
}

void MainWindow::on_lineEdit_search_textChanged(const QString &text)
{
    if(text.length() > 2 || text.isEmpty()) {
        on_pushButton_search_clicked();
    }
}

void MainWindow::on_pushButton_zones_clicked()
{
    QModelIndexList selected = ui->tableView2->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un stade");
        return;
    }

    int row = selected.first().row();
    QString name = ui->tableView2->model()->index(row, 1).data().toString();
    int capacity = ui->tableView2->model()->index(row, 3).data().toInt();

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Plan des Zones - " + name);
    dialog->setFixedSize(1100, 800);

    // Create zone map
    ZoneMap *zoneMap = new ZoneMap(dialog);
    zoneMap->setStadiumInfo(name, capacity);

    // Create controls
    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
    QHBoxLayout *controlsLayout = new QHBoxLayout();

    // Zone selection
    QComboBox *zoneCombo = new QComboBox();
    zoneCombo->addItem("Places Générales", ZoneMap::GENERAL_SEATING);
    zoneCombo->addItem("Zone VIP", ZoneMap::VIP);
    zoneCombo->addItem("Zone Famille", ZoneMap::FAMILY);
    zoneCombo->addItem("Supporters Adverses", ZoneMap::AWAY_FANS);
    zoneCombo->addItem("Zone Media", ZoneMap::MEDIA);

    // Buttons
    QPushButton *addBtn = new QPushButton("+50 Places");
    QPushButton *removeBtn = new QPushButton("-50 Places");

    // Layout
    controlsLayout->addWidget(new QLabel("Zone:"));
    controlsLayout->addWidget(zoneCombo);
    controlsLayout->addWidget(addBtn);
    controlsLayout->addWidget(removeBtn);

    mainLayout->addWidget(zoneMap);
    mainLayout->addLayout(controlsLayout);

    // CONNECTIONS - This was missing before
    connect(addBtn, &QPushButton::clicked, [zoneMap, zoneCombo]() {
        ZoneMap::ZoneType type = static_cast<ZoneMap::ZoneType>(
            zoneCombo->currentData().toInt());
        zoneMap->updateZoneOccupancy(type, 50);
    });

    connect(removeBtn, &QPushButton::clicked, [zoneMap, zoneCombo]() {
        ZoneMap::ZoneType type = static_cast<ZoneMap::ZoneType>(
            zoneCombo->currentData().toInt());
        zoneMap->updateZoneOccupancy(type, -50);
    });

    dialog->exec();
}




void MainWindow::on_tableView2_clicked(const QModelIndex &index)
{
    if(!index.isValid()) return;

    int row = index.row();
    QAbstractItemModel *model = ui->tableView2->model();

    ui->lineEdit_id->setText(model->index(row, 0).data().toString());
    ui->lineEdit_name->setText(model->index(row, 1).data().toString());
    ui->lineEdit_location->setText(model->index(row, 2).data().toString());
    ui->lineEdit_capacity->setText(model->index(row, 3).data().toString());

    // Ajouté pour l'homologation
    if(ui->spinBox_chairs->value() > 0) // Si des valeurs sont déjà chargées
        checkHomologation();
}

// Nouveaux méthodes pour l'homologation
void MainWindow::on_pushButton_verify_clicked()
{
    checkHomologation();
}

void MainWindow::checkHomologation()
{
    // Récupérer la capacité depuis l'interface
    int capacity = ui->lineEdit_capacity->text().toInt();
    if(capacity <= 0) {
        QMessageBox::warning(this, "Erreur", "Capacité invalide");
        return;
    }

    // Calculer les conditions
    int goodChairs = ui->spinBox_chairs->value();
    int workingLights = ui->spinBox_lights->value();
    int securityStaff = ui->spinBox_staff->value();

    bool chairsOK = (goodChairs >= (capacity * 0.8));
    bool lightsOK = (workingLights >= 14);
    bool staffOK = (securityStaff >= 50);

    // Afficher le résultat
    if(chairsOK && lightsOK && staffOK) {
        ui->label_homologation_status->setText("✅ Homologué - Prêt pour match");
        ui->label_homologation_status->setStyleSheet("color: green; font-weight: bold;");
    } else {
        ui->label_homologation_status->setText("❌ Non homologué - Problèmes détectés");
        ui->label_homologation_status->setStyleSheet("color: red; font-weight: bold;");
    }

    // Afficher les détails
    QString details = "Conditions d'homologation:\n";
    details += QString("• Chaises en bon état: %1/%2 (%3% - besoin ≥80%)\n")
                   .arg(goodChairs)
                   .arg(capacity)
                   .arg(qRound((goodChairs*100.0)/capacity));
    details += QString("• Lumières fonctionnelles: %1/16 (besoin ≥14)\n")
                   .arg(workingLights);
    details += QString("• Agents de sécurité: %1 (besoin ≥50)")
                   .arg(securityStaff);

    ui->textEdit_homologation_details->setText(details);
}
