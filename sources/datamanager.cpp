#include "datamanager.h"
#include "QThread"


DataManager *DataManager::getDataManager()
{
    static DataManager manager;
    return &manager;
}


DataManager::DataManager()
{
    qRegisterMetaType<deviceConnectionInfo>();
}


DataManager::~DataManager()
{
    foreach (DeviceObj* device, deviceByName) {
        qDebug()<<"delete device";
        QThread* deviceThread = device->thread();
        deviceThread->quit();
        deviceThread->wait();
        delete device;
        delete deviceThread;
    }
}


connectionsInfo DataManager::getConnectionsInfo()
{
    connectionsInfo info;

    foreach (DeviceObj* device, deviceByName) {
        info.devices.append(device->getDeviceInfo());
    }

    return info;
}


StreamObj *DataManager::getConection(const QString &DeviceName, const QString &StreamName)
{
    return deviceByName[DeviceName]->getStream(StreamName);
}


StreamObj *DataManager::getConection(const QString &DeviceName, const unsigned char &StreamIndex)
{
    return deviceByName[DeviceName]->getStream(StreamIndex);
}


void DataManager::sendMessage(const QString &DeviceName, const QString &Message)
{
    deviceByName[DeviceName]->sendMessage(Message);
}


void DataManager::deleteDevice()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(QObject::sender());
    deleteDevice(socket);
}


void DataManager::deleteDevice(QTcpSocket* socket)
{
    qDebug()<< "delete device by socket";

    if (deviceBySocket.contains(socket) && socket != 0)
    {
        DeviceObj* device = deviceBySocket[socket];

        emit deviceDisconnected(device->getDeviceConnectionInfo());

        deviceByName.remove(device->getDeviceName());
        deviceBySocket.remove(socket);

        QThread* deviceThread = device->thread();
        deviceThread->quit();
        deviceThread->wait();
        delete device;
        delete deviceThread;

        emit deviceListChanged(this->getConnectionsInfo());
    }
}


void DataManager::deleteDevice(const QString &DeviceName)
{
    qDebug()<< "delete device by name";
    if (deviceByName.contains(DeviceName))
    {
        DeviceObj* device = deviceByName[DeviceName];

        emit deviceDisconnected(device->getDeviceConnectionInfo());

        deviceByName.remove(DeviceName);
        deviceBySocket.remove(device->getDeviceSocket());

        QThread* deviceThread = device->thread();
        deviceThread->quit();
        deviceThread->wait();
        delete device;
        delete deviceThread;

        emit deviceListChanged(this->getConnectionsInfo());
    }
}


void DataManager::createDevice(QTcpSocket *socket)
{
    DeviceObj* device = new DeviceObj(socket);

    connect(socket, SIGNAL(readyRead()), device, SLOT(receiveNewData()));
    connect(device, SIGNAL(deviceWasNamed()), this, SLOT(deviceWasNamed()));

    deviceBySocket.insert(socket, device);
}


void DataManager::confirmConnection(QTcpSocket *socket)
{
    if (deviceBySocket.contains(socket))
        deviceBySocket[socket]->confirmConnection();
}


void DataManager::deviceWasNamed()
{
    DeviceObj* device = static_cast<DeviceObj *>(QObject::sender());

    if (deviceByName.contains(device->getDeviceName()))
    {
        device->requestNewName();
        return;
    }
    else
        device->confirmInitialization();

    deviceByName.insert(device->getDeviceName(), device);

    qDebug()<<"device connected";
    emit deviceListChanged(this->getConnectionsInfo());
    emit deviceConnected(device->getDeviceConnectionInfo());
}
