#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QPair>

#include "deviceobj.h"
#include "streamobj.h"
#include "informationstructs.h"


class DataManager : public QObject
{
    Q_OBJECT

public:
    static DataManager *getDataManager();

public slots:
    void socketReadyRead();

    StreamObj* getConection(const QString &DeviceName, const QString &StreamName);
    StreamObj* getConection(const QString &DeviceName, const unsigned char &StreamIndex);

    void sendMessage(const QString &DeviceName, const QString &Message);

    void deleteDevice(QTcpSocket* socket);
    void deleteDevice(const QString &DeviceName);

    connectionsInfo getConnectionsInfo();


signals:
    void deviceDisconnected(QString);
    void newMessage(QPair<QString,QString>);
    void deviceListChanged(connectionsInfo);


private slots:
    void deviceWasNamed();


private:
    DataManager();
    ~DataManager();

    char addNewDevice(QTcpSocket* socket);

    QMap <QTcpSocket*, DeviceObj*> deviceBySocket;
    QMap <QString, DeviceObj*> deviceByName;
};


#endif // DATAMANAGER_H
