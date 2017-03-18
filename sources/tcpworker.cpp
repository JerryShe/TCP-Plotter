#include "tcpworker.h"

#include <QHostInfo>
#include <QDebug>
#include <QNetworkInterface>

#include <QDataStream>
#include <QAbstractSocket>

#include <threadedtcpsocket.h>


TcpReceiver::TcpReceiver(DeviceManager* manager)
{
    qRegisterMetaType<QAbstractSocket::SocketError>();
    qRegisterMetaType<QVector<int>>();
    dataManager = manager;
    qDebug()<<"start";
}


TcpReceiver::~TcpReceiver()
{}


QString TcpReceiver::getLocalIP()
{
    QStringList addrs;
    QStringList names;
    foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
    {
        if(interface.flags().testFlag(QNetworkInterface::IsUp) &&
                (!interface.flags().testFlag(QNetworkInterface::IsLoopBack)))
        {
            foreach(QNetworkAddressEntry entry, interface.addressEntries())
            {
                if(entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
                {
                    names.append(interface.humanReadableName());
                    addrs.append(entry.ip().toString());
                }
            }
        }
    }

    QString add;
    for (int i = 0; i < addrs.size(); i++)
        add += names[i] + ":" + '\n' + addrs[i] + "\n\n";

    return add;
}


void TcpReceiver::socketError(QAbstractSocket::SocketError err)
{
    QObject * object = QObject::sender();
    if (!object)
        return;

    QTcpSocket * socket = static_cast<QTcpSocket *>(object);
    qDebug()<<socket->errorString();

    emit connectionError(socket);
}


bool TcpReceiver::openPort(const QString &port)
{
    if (isListening())
            close();

    if(!listen(QHostAddress::Any, port.toShort()))
        return false;

    return true;
}

short TcpReceiver::getServerPort()
{
    return serverPort();
}


void TcpReceiver::socketDisconnected()
{
    qDebug()<<"disconected";
    QObject * object = QObject::sender();
    if (!object)
        return;

    QTcpSocket * socket = static_cast<QTcpSocket *>(object);

    int pos = socketList.indexOf(socket);
    if (pos != -1)
    {
        socketList.removeAt(pos);
    }
    else
    {
        qDebug()<<"Huiston! U nas problemi! Socket poteryan! Povtoryau, socket poteryan!";
    }
}


void TcpReceiver::newConnectionTo(const QString &ip, const QString &port)
{
    qDebug()<<"connecting";

    ThreadedTcpSocket* socketThread = new ThreadedTcpSocket(ip, port.toShort(), this);
    connect(socketThread, SIGNAL(socketCreated(QTcpSocket*)), this, SLOT(socketCreated(QTcpSocket*)));
    socketThread->start();
}


void TcpReceiver::newConnectionToDone()
{
    QObject * object = QObject::sender();
    QTcpSocket* socket = static_cast<QTcpSocket *>(object);
    if (socket == 0)
        return;

    emit connectionDone(socket);
}


void TcpReceiver::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"connecting";

    ThreadedTcpSocket* socketThread = new ThreadedTcpSocket(socketDescriptor, this);
    connect(socketThread, SIGNAL(socketCreated(QTcpSocket*)), this, SLOT(socketCreated(QTcpSocket*)));
    socketThread->start();
}


void TcpReceiver::socketCreated(QTcpSocket *socket)
{
    dataManager->createDevice(socket);

    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

    connect(socket, SIGNAL(disconnected()), dataManager, SLOT(deleteDevice()));

    if (socket->bytesAvailable() > 0)
        emit socket->readyRead();

    socketList.append(socket);

    qDebug()<<"connected";
}
