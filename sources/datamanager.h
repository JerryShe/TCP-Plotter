#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QPair>

#include "deviceobj.h"
#include "streamobj.h"
#include "informationstructs.h"


class DeviceManager : public QObject
{
    Q_OBJECT

public:
    static DeviceManager *getDataManager();

public slots:    
    void createDevice(QTcpSocket* socket);

    StreamObj* getConection(const QString &DeviceName, const QString &StreamName);
    StreamObj* getConection(const QString &DeviceName, const unsigned char &StreamIndex);

    void sendMessage(const QString &DeviceName, const QString &Message);

    void deleteDevice();
    void deleteDevice(QTcpSocket* socket);
    void deleteDevice(const QString &DeviceName);

    deviceConnectionsInfo getConnectionsInfo();

    void confirmConnection(QTcpSocket* socket);

signals:
    void deviceConnected(deviceConnectionInfo);
    void deviceDisconnected(deviceConnectionInfo);

    void newMessage(QPair<QString,QString>);
    void deviceListChanged(deviceConnectionsInfo);


private slots:
    void deviceWasNamed();


private:
    DeviceManager();
    ~DeviceManager();

    QMap <QTcpSocket*, DeviceObj*> deviceBySocket;
    QMap <QString, DeviceObj*> deviceByName;
};


#endif // DATAMANAGER_H
