#include "datamanager.h"
#include "QThread"


DataManager *DataManager::getDataManager()
{
    static DataManager manager;
    return &manager;
}


DataManager::DataManager()
{
}


DataManager::~DataManager()
{
    foreach (DeviceObj* device, deviceByName) {
        qDebug()<<"delete device";
        delete device;
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


void DataManager::deleteDevice(QTcpSocket *socket)
{
    delete deviceBySocket[socket];
    deviceBySocket.remove(socket);
    emit deviceListChanged(this->getConnectionsInfo());
}


void DataManager::deleteDevice(const QString &DeviceName)
{
    delete deviceByName[DeviceName];
    deviceByName.remove(DeviceName);
}


void DataManager::socketReadyRead()
{
    QObject * object = QObject::sender();
    if (!object)
        return;

    QTcpSocket * socket = static_cast<QTcpSocket *>(object);

    if (deviceBySocket.contains(socket))
        deviceBySocket[socket]->receiveNewData();
    else
        addNewDevice(socket);
}


char DataManager::addNewDevice(QTcpSocket *socket)
{
    DeviceObj* device = new DeviceObj(socket);
    QThread* deviceThread = new QThread(device);
    device->moveToThread(deviceThread);

    connect(device, SIGNAL(deviceWasNamed()), this, SLOT(deviceWasNamed()));
    connect(device, SIGNAL(destroyed(QObject*)), deviceThread, SLOT(quit()));

    device->initialize();

    deviceBySocket.insert(socket, device);    
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

    emit deviceListChanged(this->getConnectionsInfo());
}


