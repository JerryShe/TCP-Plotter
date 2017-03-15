#ifndef TCPWORKER_H
#define TCPWORKER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMetaType>


#include "datamanager.h"


class TcpReciver : public QTcpServer
{
    Q_OBJECT

public:
    TcpReciver(DataManager* manager);
    ~TcpReciver();

    QString getLocalIP();

signals:
    void connectionError(QTcpSocket*);
    void disconnected(QStringList);
    void connectionDone(QTcpSocket*);


public slots:
    bool openPort(const QString &port);
    void newConnectionTo(const QString &ip, const QString &port);
    short getServerPort();


private slots:
    void newConnectionToDone();
    void socketError(QAbstractSocket::SocketError);
    void socketDisconnected();

    void socketCreated(QTcpSocket* socket);

protected:
    void incomingConnection(qintptr socketDescriptor);


private:
    QVector<QTcpSocket*> socketList;

    DataManager* dataManager;
};


#endif // TCPWORKER_H
