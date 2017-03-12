#ifndef STREAMOBJ_H
#define STREAMOBJ_H

#include <QObject>
#include <QTcpSocket>
#include <QPair>
#include <QDataStream>

#include "informationstructs.h"


struct packet
{
    packet(const QString &DeviceName);

    QString deviceName;
    unsigned char streamIndex;
    unsigned int time;
    QVector <double> data;

    void addVar(double var);
};



class StreamObj : public QObject
{
    Q_OBJECT

public:
    StreamObj();
    StreamObj(QTcpSocket* socket, unsigned char StreamIndex, QDataStream &dataStream);
    ~StreamObj();


    void receiveNewData(const QString &deviceName, QDataStream &dataStream);


    QString getStreamName();
    void setStreamName(const QString &StreamName);

    QString getStreamDescription();
    void setStreamDescription(const QString &StreamDescription);

    QStringList getStreamVariables();
    void setStreamVariables(const QStringList &StreamVariables);

    unsigned char getStreamIndex();
    void setStreamIndex(const unsigned char &index);

    QVector <packet> getAllData();

    streamInfo getStreamInfo();


signals:
    void newData(packet pack);


private:
    unsigned int timeInterval;                  //  == 0 - время явно указывается в пакете, != 0 - t += timeInterval

    unsigned char streamIndex;

    QString streamName;
    QString streamDescription;
    QStringList streamVariables;
    QStringList streamVariablesDescription;

    QTcpSocket* deviceSocket;

    QVector <packet> dataList;
};

#endif // STREAMOBJ_H
