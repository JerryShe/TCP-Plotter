#ifndef TCPWORKER_H
#define TCPWORKER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMetaType>


#include "datamanager.h"


class TcpWorker : public QObject
{
    Q_OBJECT

public:
    TcpWorker(DataManager* manager);
    ~TcpWorker();

    void start(DataManager* manager);

    QString getLocalIP();

signals:
    void connectionError(QTcpSocket* socket);
    void disconnected(QStringList socketData);
    void newConnection(QTcpSocket* socket);
    void connectionDone(QTcpSocket* socket);


public slots:
    bool openPort(const QString &port);
    void newConnectionTo(const QString &ip, const QString &port);
    bool sendTo(const QString &ip, const QString &port, const QString &message);
    short getServerPort();


private slots:
    void newConnectionFrom();
    void newConnectionToDone();
    void socketError(QAbstractSocket::SocketError);
    void socketDisconnected();



private:
    QTcpServer* server;
    QVector<QTcpSocket*> socketList;

    DataManager* dataManager;
};


#endif // TCPWORKER_H

