#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QModelIndex>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include "stade.h"
#include "zonemap.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_ajouter_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_supprimer_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_pushButton_trier_clicked();
    void on_pushButton_stats_clicked();
    void on_pushButton_pdf_clicked();
    void on_pushButton_search_clicked();
    void on_lineEdit_search_returnPressed();
    void on_lineEdit_search_textChanged(const QString &text);
    void on_pushButton_zones_clicked();
    void on_tableView2_clicked(const QModelIndex &index);

    // Ajouté pour l'homologation
    void on_pushButton_verify_clicked();

private:
    Ui::MainWindow *ui;
    void loadStadiums();
    bool isNumber(const QString &str);
    bool isTextOnly(const QString &str);
    void exportToPdf(const QString &fileName);

    // Ajouté pour l'homologation
    void checkHomologation();
};

#endif // MAINWINDOW_H
