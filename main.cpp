#include <QApplication>
#include "mainwindow.h"
#include "connection.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Connection conn;
    if (!conn.createconnect()) {
        return -1;
    }
    MainWindow w;
    w.show();
    return app.exec();
}
