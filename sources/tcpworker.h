#ifndef TCPWORKER_H
#define TCPWORKER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include <QMetaType>


class TcpWorker : public QObject
{
    Q_OBJECT

public:
    static TcpWorker* getTcpWorker();
    void start();

    QString getLocalIP();

signals:
    void connectionError(QTcpSocket* socket);
    void disconnected(QTcpSocket* socket);
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
    void readyRead();
    void socketError(QAbstractSocket::SocketError);
    void socketDisconnected();



private:
    TcpWorker();
    ~TcpWorker();

    QTcpServer* server;
    QList <QTcpSocket*> socketList;

};


#endif // TCPWORKER_H

