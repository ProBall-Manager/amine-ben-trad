QT       += core gui widgets sql charts printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    connection.cpp \
    main.cpp \
    mainwindow.cpp \
    stade.cpp \
    zonemap.cpp

HEADERS += \
    connection.h \
    mainwindow.h \
    stade.h \
    zonemap.h

FORMS += \
    mainwindow.ui

RESOURCES +=
