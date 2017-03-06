#include "tcpworker.h"

#include <QHostInfo>
#include <QDebug>
#include <QNetworkInterface>

#include <QDataStream>
#include <QAbstractSocket>

TcpWorker *TcpWorker::getTcpWorker()
{
    static TcpWorker worker;
    return &worker;
}


TcpWorker::TcpWorker()
{
    qRegisterMetaType<QAbstractSocket::SocketError>();
    qRegisterMetaType<QVector<int>>();
}


TcpWorker::~TcpWorker()
{

}


QString TcpWorker::getLocalIP()
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


void TcpWorker::socketError(QAbstractSocket::SocketError err)
{
    QObject * object = QObject::sender();
    if (!object)
        return;
    QTcpSocket * socket = static_cast<QTcpSocket *>(object);

    emit connectionError(socket);
}


void TcpWorker::start()
{
    server = new QTcpServer();
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnectionFrom()));
}


bool TcpWorker::openPort(const QString &port)
{
    if (server->isListening())
            server->close();

    if(!server->listen(QHostAddress::Any, port.toShort()))
        return false;

    return true;
}

short TcpWorker::getServerPort()
{
    return server->serverPort();
}


bool TcpWorker::sendTo(const QString &ip, const QString &port, const QString &message)
{
    QTcpSocket* peerSocket = 0;
    short numPort = port.toShort();

    foreach (QTcpSocket* socket, socketList) {
        if (socket->peerAddress().toString() == ip && socket->peerPort() == numPort)
        {
            peerSocket = socket;
            break;
        }
    }

    if (peerSocket == 0)
    {
        qDebug()<<"sending falue";
        return false;
    }

    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out<<message;
    if (peerSocket->write(arrBlock) == -1)
        return false;

    return true;
}


void TcpWorker::socketDisconnected()
{
    qDebug()<<"disconected";
    QObject * object = QObject::sender();
    if (!object)
        return;

    QTcpSocket * socket = static_cast<QTcpSocket *>(object);
    emit disconnected(socket);


    int pos = socketList.indexOf(socket);
    if (pos != -1)
        socketList.removeAt(pos);
    else
    {
        qDebug()<<"Huiston! U nas problemi! Socket proeban! Povtoryau, socket proeban!";
    }
}


void TcpWorker::newConnectionTo(const QString &ip, const QString &port)
{
    QTcpSocket* socket = new QTcpSocket();
    socket->connectToHost(ip, port.toShort());

    connect(socket, SIGNAL(connected()), this, SLOT(newConnectionToDone()));

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

    socketList.append(socket);
}


void TcpWorker::newConnectionToDone()
{
    QObject * object = QObject::sender();
    QTcpSocket* socket = static_cast<QTcpSocket *>(object);
    if (socket == 0)
        return;

    emit connectionDone(socket);
}



void TcpWorker::newConnectionFrom()
{
    QTcpSocket * socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

    emit newConnection(socket);

    socketList.append(socket);
    qDebug()<<"connected";
}


void TcpWorker::readyRead()
{
    QObject * object = QObject::sender();
    if (!object)
        return;

    QTcpSocket * socket = static_cast<QTcpSocket *>(object);
    QByteArray arr =  socket->readAll();
    qDebug()<<QString::fromUtf8(arr);
}
