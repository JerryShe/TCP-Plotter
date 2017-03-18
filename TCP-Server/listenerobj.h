#ifndef LISTENEROBJ_H
#define LISTENEROBJ_H

#include <QObject>
#include <QDataStream>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "informationstructs.h"

class ListenerObj : public QObject
{
    Q_OBJECT
public:
    ListenerObj();
    ListenerObj(QTcpSocket* socket);
    ~ListenerObj();

signals:
    void listenerWasConnected();
    void newMessage(message);
    void send(QByteArray);

    void rightsRequest(listenerRights);
    void controlRequest(QString);
    void commandRequest(QString);

public slots:
    void confirmConnection();
    void confirmInitialization();
    bool wasInitialized();
    void initialize();
    void requestNewName();

    void sendMessage(message mess);
    void sendData(packet pack);
    void receiveNewData();

    listenerInfo getListenerInfo();

    QString getListenerName();
    QTcpSocket* getListenerSocket();

private slots:
    void getNewName();

    void writeToSocket(QByteArray json);
    void writeToSocket(QString str);

    QJsonDocument readJson();
    void parseJson(QJsonObject obj);

private:
    bool listenerType;
    unsigned char initialized;

    QString listenerName;
    QString listenerDescription;
    QTcpSocket* listenerSocket;

    QDataStream listenerDataStream;
};

#endif // LISTENEROBJ_H
