#include "streamobj.h"
#include <QDataStream>



packet::packet(const QString &DeviceName)
{
    deviceName = DeviceName;
}

void packet::addVar(double var)
{
    data.append(var);
}




StreamObj::StreamObj()
{}

StreamObj::StreamObj(QTcpSocket* socket, unsigned char StreamIndex, QDataStream &dataStream)
{
    deviceSocket = socket;
    streamIndex = StreamIndex;


    unsigned char strSize;
    QByteArray str;

    dataStream >> strSize;
    if (strSize == 0)
    {
        ///невалидное имя
        return;
    }
    str.resize(strSize);
    dataStream >> str;
    streamName = QString::fromUtf8(str);

    dataStream >> strSize;
    if (strSize != 0)
    {
        str.resize(strSize);
        dataStream >> str;
        streamDescription = QString::fromUtf8(str);
    }


    dataStream >> timeInterval;

    unsigned char size;
    dataStream >> size;
    qDebug()<<dataStream.atEnd();
    qDebug()<<"var size "<<size;

    for (int i = 0; i < size; i++)
    {
        dataStream >> strSize;
        if (strSize == 0)
        {
            ///невалидное имя переменной
            return;
        }
        str.resize(strSize);
        dataStream >> str;
        streamVariables.append(QString::fromUtf8(str));


        dataStream >> strSize;
        if (strSize != 0)
        {
            str.resize(strSize);
            dataStream >> str;
            streamVariablesDescription.append(QString::fromUtf8(str));
        }
    }

    qDebug()<<streamName;
    qDebug()<<streamDescription;
    qDebug()<<streamVariables;
    qDebug()<<streamVariablesDescription;
}


StreamObj::~StreamObj()
{

}


void StreamObj::receiveNewData(const QString &deviceName, QDataStream &dataStream)
{
    packet pack(deviceName);

    unsigned int time;
    if (timeInterval == 0)
        dataStream>>time;
    else
        time += timeInterval;

    pack.streamIndex = streamIndex;
    pack.time = time;

    double temp;
    for (int i = 0; i < streamVariables.size(); i++)
    {
        dataStream>>temp;
        pack.addVar(temp);
    }

    emit newData(pack);
}


QString StreamObj::getStreamName()
{
    return streamName;
}


void StreamObj::setStreamName(const QString &StreamName)
{
    streamName = StreamName;
}


QString StreamObj::getStreamDescription()
{
    return streamDescription;
}


void StreamObj::setStreamDescription(const QString &StreamDescription)
{
    streamDescription = StreamDescription;
}


QStringList StreamObj::getStreamVariables()
{
    return streamVariables;
}


void StreamObj::setStreamVariables(const QStringList &StreamVariables)
{
    streamVariables = StreamVariables;
}


QVector<packet> StreamObj::getAllData()
{
    return dataList;
}


streamInfo StreamObj::getStreamInfo()
{
    streamInfo info;
    info.streamName = streamName;
    info.streamDescription = streamDescription;
    info.streamVariables = streamVariables;
    info.streamVariablesDescription;

    return info;
}
