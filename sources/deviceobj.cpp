#include "deviceobj.h"


DeviceObj::DeviceObj()
{}

DeviceObj::~DeviceObj()
{}


DeviceObj::DeviceObj(QTcpSocket *socket)
{
    deviceSocket = socket;

    deviceDataStream.setDevice(socket);
    initialized = 0;
}


void DeviceObj::initialize()
{
    qDebug()<<"initializing";
    deviceDataStream.startTransaction();

    unsigned char strSize;
    QByteArray str;

    deviceDataStream >> strSize;
    if (strSize == 0)
    {
        char a[1] = {0};
        deviceSocket->write(a);
        return;
    }


    str.resize(strSize);
    deviceDataStream >> str;
    deviceName = QString::fromUtf8(str);

    deviceDataStream >> strSize;
    if (strSize != 0)
    {
        str.resize(strSize);
        deviceDataStream >> str;
        deviceDescription = QString::fromUtf8(str);
    }

    unsigned char size;
    deviceDataStream >> size;


    if (!deviceDataStream.commitTransaction())
    {
        return;
    }


    for (unsigned char i = 1; i <= size; i++)
    {
        StreamObj* strObj = new StreamObj(deviceSocket, i);

        streamByName.insert(strObj->getStreamName(), strObj);
        streamByIndex.append(strObj);
    }

    qDebug()<<"device " << deviceName << "in thread " << this->thread();
    qDebug()<<deviceName;
    qDebug()<<deviceDescription;
    qDebug()<<streamByIndex.size();
    emit deviceWasNamed();
}


void DeviceObj::confirmInitialization()
{
    initialized = 1;
    char a[1] = {1};
    deviceSocket->write(a);
}


void DeviceObj::requestNewName()
{
    qDebug()<<"request new name";
    initialized = 2;
    char a[1] = {2};
    deviceSocket->write(a);
}


bool DeviceObj::wasInitialized()
{
    if (initialized == 1)
        return true;
    return false;
}


void DeviceObj::receiveNewData()
{
    if (initialized == 1)
    {
        unsigned char streamIndex;
        deviceDataStream>>streamIndex;

        streamByIndex[streamIndex]->receiveNewData(deviceName);
    }
    else
    {
        if (initialized == 0)
            initialize();
        else
        {

            deviceDataStream.startTransaction();

            unsigned char strSize;
            deviceDataStream >> strSize;
            QByteArray str;
            str.resize(strSize);
            deviceDataStream >> str;

            if (!deviceDataStream.commitTransaction())
                return;

            deviceName = QString::fromUtf8(str);
            qDebug()<<"try new name :"<< deviceName;
            emit deviceWasNamed();
        }
    }
}


StreamObj *DeviceObj::getStream(const QString &StreamName)
{
    if (streamByName.contains(StreamName))
        return streamByName[StreamName];
    return 0;
}


StreamObj *DeviceObj::getStream(const unsigned char &StreamIndex)
{
    if (streamByIndex.size() < StreamIndex && StreamIndex > 0)
        return streamByIndex[StreamIndex];
    return 0;
}


QString DeviceObj::getDeviceName()
{
    return deviceName;
}


void DeviceObj::setDeviceName(const QString &DeviceName)
{
    deviceName = DeviceName;
}


QString DeviceObj::getDeviceDescription()
{
    return deviceDescription;
}


void DeviceObj::setDeviceDescription(const QString &DeviceDescription)
{
    deviceDescription = DeviceDescription;
}


QVector<QPair<bool, QString> > DeviceObj::getDeviceMessages()
{
    return messages;
}


void DeviceObj::sendMessage(const QString &message)
{
    deviceSocket->write(message.toUtf8().toStdString().c_str());
}


deviceInfo DeviceObj::getDeviceInfo()
{
    deviceInfo info;
    info.deviceName = deviceName;
    info.deviceDescription = deviceDescription;

    foreach (StreamObj* stream, streamByIndex) {
        info.streams.append(stream->getStreamInfo());
    }

    return info;
}
