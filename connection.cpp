#include "connection.h"

Connection::Connection() {}

bool Connection::createconnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_PRojet2A");  // Data source name
    db.setUserName("proballManager");        // Username
    db.setPassword("foot123456");            // Password

    if (db.open())
        test = true;

    return test;
}
