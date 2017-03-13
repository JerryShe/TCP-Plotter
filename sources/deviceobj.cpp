#include "deviceobj.h"


DeviceObj::DeviceObj()
{}

DeviceObj::~DeviceObj()
{
    qDebug()<<"delete socket";
    delete deviceSocket;
}


DeviceObj::DeviceObj(QTcpSocket *socket)
{
    deviceSocket = socket;
    this->moveToThread(deviceSocket->thread());
    qDebug() << "Device started " << this->thread();

    deviceDataStream.setDevice(socket);
    initialized = 0;

    connect(this, SIGNAL(sendToDevice(QString)), this, SLOT(writeToSocket(QString)));
    connect(this, SIGNAL(sendToDevice(char*)), this, SLOT(writeToSocket(char*)));
}


void DeviceObj::writeToSocket(QString str)
{
    char size[1] {str.size()};
    const char* mass = str.toUtf8().toStdString().c_str();

    deviceSocket->write((unsigned char)0);
    deviceSocket->write(size);
    deviceSocket->write(mass, str.size());
}


void DeviceObj::writeToSocket(char *c)
{
    qDebug()<<"write to socket";
    deviceSocket->write(c,2);
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
        abortInitialize();
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


    for (unsigned char i = 1; i <= size; i++)
    {
        StreamObj* strObj = new StreamObj(deviceSocket, i, deviceDataStream);

        streamByName.insert(strObj->getStreamName(), strObj);
        streamByIndex.append(strObj);
    }


    if (!deviceDataStream.commitTransaction())
    {
        return;
    }

    qDebug()<<"device " << deviceName << "in thread " << this->thread();
    qDebug()<<deviceName;
    qDebug()<<deviceDescription;
    qDebug()<<streamByIndex.size();

    emit deviceWasNamed();
}


void DeviceObj::confirmConnection()
{
    qDebug()<<"confirm connection";

    if (initialized == 1)
    {
        char a[2] = {1,1};
        emit sendToDevice(a);
    }
}


void DeviceObj::confirmInitialization()
{
    qDebug()<<"confirm initialization";
    initialized = 1;
}


void DeviceObj::requestNewName()
{
    qDebug()<<"request new name";
    initialized = 2;
    char a[2] = {2,2};
    emit sendToDevice(a);
}


void DeviceObj::abortInitialize()
{
    qDebug()<<"abort initialize";
    initialized = 0;
    char a[2] = {0,0};
    emit sendToDevice(a);
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
        deviceDataStream.startTransaction();

        unsigned char streamIndex;
        deviceDataStream>>streamIndex;
        qDebug()<<"receive at "<< streamIndex;

        if (streamIndex != 0 && streamIndex - 1 < streamByIndex.size())
            if (streamByIndex[streamIndex - 1]->receiveNewData(deviceName, deviceDataStream))
                receiveNewData();
    }
    else
    {
        if (initialized == 0)
            initialize();
        else
            getNewName();
    }
}


void DeviceObj::getNewName()
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


QTcpSocket* DeviceObj::getDeviceSocket()
{
    return deviceSocket;
}


QVector<QPair<bool, QString> > DeviceObj::getDeviceMessages()
{
    return messages;
}


void DeviceObj::sendMessage(const QString &message)
{
    deviceSocket->write(message.toUtf8().toStdString().c_str());
}


deviceConnectionInfo DeviceObj::getDeviceConnectionInfo()
{
    deviceConnectionInfo info;
    info.peerIP = deviceSocket->peerAddress().toString();
    info.peerPort = QString::number(deviceSocket->peerPort());
    info.deviceName = deviceName;
    info.deviceDescription = deviceDescription;
    info.deviceSocket = deviceSocket;

    return info;
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
