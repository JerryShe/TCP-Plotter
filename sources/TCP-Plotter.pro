QT       += core gui printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TCP-Plotter
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        mainwindow.cpp \
    tcpworker.cpp \
    graphwindow.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    tcpworker.h \
    graphwindow.h \
    qcustomplot.h

FORMS    += mainwindow.ui \
    graphwindow.ui
