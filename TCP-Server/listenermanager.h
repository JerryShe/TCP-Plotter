#ifndef LISTENERMANAGER_H
#define LISTENERMANAGER_H

#include <QObject>
#include "listenerobj.h"
#include "informationstructs.h"
#include "devicemanager.h"
#include <QTcpSocket>


class ListenerManager : public QObject
{
    Q_OBJECT

public:
    ListenerManager(DeviceManager* manager);
    ~ListenerManager();

signals:
    void listenerDisconnected(listenerInfo);
    void listenerConnected(listenerInfo);

    void newMessage(message);
    void listenerListChanged(listenerList);

    void listenerRequestRights(listenerInfo, listenerRights);
    void listenerRequestControl(listenerInfo, QString);

    void masterCommand(QString);



public slots:
    void createListener(QTcpSocket* socket);
    void confirmConnection(QTcpSocket* socket);
    void confirmRights(const QString name, listenerRights rights);

    void changeMaster(const QString &masterName);

    void deleteListener();
    void deleteListener(QTcpSocket* socket);
    void deleteListener(const QString &name);

    void sendMessage(const QString &name, message mess);

    listenerList getListenerList();

private slots:
    void listenerWasConnected();

    void sendMessage(message mess);
    void requestRights(listenerRights rights);
    void requestControl(const QString key);
    void requestCommand(const QString command);

private:
    QMap <QTcpSocket*, ListenerObj*> listenerBySocket;
    QMap <QString, ListenerObj*> listenerByName;

    QMap <ListenerObj*, listenerRights> listenerPrivileges;

    ListenerObj* master;

    DeviceManager* deviceManager;
};

#endif // LISTENERMANAGER_H
