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

StreamObj::StreamObj(QTcpSocket* socket, unsigned char StreamIndex)
{
    deviceSocket = socket;
    streamIndex = StreamIndex;

    QDataStream in(socket);

    unsigned char strSize;
    QByteArray str;

    in >> strSize;
    if (strSize == 0)
    {
        ///невалидное имя
        return;
    }
    str.resize(strSize);
    in >> str;
    streamName.fromUtf8(str);

    in >> strSize;
    if (strSize != 0)
    {
        str.resize(strSize);
        in >> str;
        streamDescription.fromUtf8(str);
    }


    in >> timeInterval;

    unsigned char size;
    in >> size;
    for (int i = 0; i < size; i++)
    {
        in >> strSize;
        if (strSize == 0)
        {
            ///невалидное имя переменной
            return;
        }
        str.resize(strSize);
        in >> str;
        streamVariables.append(QString::fromUtf8(str));


        in >> strSize;
        if (strSize != 0)
        {
            str.resize(strSize);
            in >> str;
            streamVariablesDescription.append(QString::fromUtf8(str));
        }
    }
}


StreamObj::~StreamObj()
{

}


void StreamObj::receiveNewData(const QString &deviceName)
{
    QDataStream in(deviceSocket);

    packet pack(deviceName);

    unsigned int time;
    if (timeInterval == 0)
        in>>time;
    else
        time += timeInterval;

    pack.streamIndex = streamIndex;
    pack.time = time;

    double temp;
    for (int i = 0; i < streamVariables.size(); i++)
    {
        in>>temp;
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
