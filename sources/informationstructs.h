#ifndef INFORMATIONSTRUCTS_H
#define INFORMATIONSTRUCTS_H


#include <QString>
#include <QVector>
#include <QTcpSocket>
#include <QMetaType>

struct streamInfo
{
    QString streamName;
    QString streamDescription;
    QStringList streamVariables;
    QStringList streamVariablesDescription;
};


struct deviceInfo
{
    QString deviceName;
    QString deviceDescription;
    QVector <streamInfo> streams;
};


struct deviceConnectionsInfo
{
    QVector <deviceInfo> devices;
};


struct deviceConnectionInfo
{
    QString peerIP;
    QString peerPort;
    QString deviceName;
    QString deviceDescription;
    QTcpSocket* deviceSocket;
};


struct packet
{
    packet();
    packet(const QString &DeviceName);

    QString deviceName;
    unsigned char streamIndex;
    unsigned int time;
    QVector <double> data;

    void addVar(double var);
};


Q_DECLARE_METATYPE(deviceConnectionInfo)
Q_DECLARE_METATYPE(connectionsInfo)
Q_DECLARE_METATYPE(deviceInfo)
Q_DECLARE_METATYPE(streamInfo)
Q_DECLARE_METATYPE(packet)

#endif // INFORMATIONSTRUCTS_H
