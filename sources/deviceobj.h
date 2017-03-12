#ifndef DEVICEOBJ_H
#define DEVICEOBJ_H

#include <QObject>
#include <QTcpSocket>
#include <QPair>
#include <QDataStream>


#include "streamobj.h"
#include "informationstructs.h"


class DeviceObj : public QObject
{
    Q_OBJECT

public:
    DeviceObj();
    DeviceObj(QTcpSocket* socket);
    ~DeviceObj();

    void initialize();
    bool wasInitialized();
    void confirmInitialization();
    void requestNewName();


    void receiveNewData();

    StreamObj *getStream(const QString &StreamName);
    StreamObj *getStream(const unsigned char &StreamIndex);


    char getDeviceIndex();
    void setDeviceIndex(const unsigned char &DeviceIndex);

    QString getDeviceName();
    void setDeviceName(const QString &DeviceName);

    QString getDeviceDescription();
    void setDeviceDescription(const QString &DeviceDescription);

    QVector <QPair<bool, QString>> getDeviceMessages();
    void sendMessage(const QString &message);

    deviceInfo getDeviceInfo();


signals:
    void deviceWasNamed();

private:
    unsigned char initialized;                              //0 - не готово, 1 - готово, 2 - смена имени

    QString deviceName;
    QString deviceDescription;

    QTcpSocket* deviceSocket;
    QDataStream deviceDataStream;

    QMap <QString, StreamObj*> streamByName;
    QVector <StreamObj*> streamByIndex;

    QVector <QPair<bool, QString>> messages;       // 0 - полученное, 1 - отправленное
};


#endif // DEVICEOBJ_H
