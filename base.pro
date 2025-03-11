QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    connection.cpp \
    main.cpp \
    mainwindow.cpp \
    stade.cpp

HEADERS += \
    connection.h \
    mainwindow.h \
    stade.h

FORMS += \
    mainwindow.ui
