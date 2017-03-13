QT       += core gui printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TCP-Plotter
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        mainwindow.cpp \
    tcpworker.cpp \
    graphwindow.cpp \
    qcustomplot.cpp \
    datamanager.cpp \
    streamobj.cpp \
    deviceobj.cpp \
    informationstructs.cpp \
    devicemanagerwindow.cpp \
    threadedtcpsocket.cpp

HEADERS  += mainwindow.h \
    tcpworker.h \
    graphwindow.h \
    qcustomplot.h \
    datamanager.h \
    streamobj.h \
    deviceobj.h \
    informationstructs.h \
    devicemanagerwindow.h \
    threadedtcpsocket.h

FORMS    += mainwindow.ui \
    graphwindow.ui \
    devicemanagerwindow.ui
