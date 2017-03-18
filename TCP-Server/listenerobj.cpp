#include "listenerobj.h"

#include <QHostAddress>


ListenerObj::ListenerObj()
{}


ListenerObj::ListenerObj(QTcpSocket *socket)
{
    listenerSocket = socket;
    this->moveToThread(socket->thread());
    qDebug() << "Listener started " << this->thread();

    listenerDataStream.setDevice(socket);

    listenerType = 0;
    initialized = 0;

    connect(this, SIGNAL(send(QString)), this, SLOT(writeToSocket(QString)));
}


ListenerObj::~ListenerObj()
{}


void ListenerObj::initialize()
{
    QJsonDocument doc = readJson();

    if (doc.isNull())
        return;

    QJsonObject obj = doc.object();

    listenerName = obj["name"].toString();

    if (listenerName == "")
        return;

    listenerDescription = obj["description"].toString();

    emit listenerWasConnected();
}


void ListenerObj::confirmConnection()
{
    if (initialized == 1)
    {
        ///
    }
}


void ListenerObj::confirmInitialization()
{
    initialized = 1;
}


bool ListenerObj::wasInitialized()
{
    if (initialized == 1)
        return true;
    return false;
}


void ListenerObj::requestNewName()
{

}


void ListenerObj::sendMessage(message mess)
{
    if (mess.recipientName == listenerName)
        writeToSocket(mess.toJson());
}


void ListenerObj::sendData(packet pack)
{
    writeToSocket(pack.toJson());
}


QJsonDocument ListenerObj::readJson()
{
    listenerDataStream.startTransaction();
    int size;
    listenerDataStream >> size;
    QByteArray json;
    json.reserve(size);

    listenerDataStream >> json;

    if (!listenerDataStream.commitTransaction())
        return QJsonDocument::fromJson(0);

    return QJsonDocument::fromJson(json);
}


void ListenerObj::receiveNewData()
{
    if (initialized == 1)
    {
        QJsonDocument doc = readJson();

        if (!doc.isNull())
            parseJson(doc.object());
    }
    else
    {
        if (initialized == 0)
            initialize();
        else
            getNewName();
    }
}


void ListenerObj::getNewName()
{
    QJsonDocument doc = readJson();

    listenerName = doc.object()["new name"].toString();
    emit listenerWasConnected();
}


void ListenerObj::parseJson(QJsonObject obj)
{
    if (obj["type"].toString() == "message")
    {
        message mess(obj, listenerName);
        emit newMessage(mess);
    }

    else if (obj["type"].toString() == "command")
    {
        emit controlRequest(obj["command"].toString());
    }

    else if (obj["type"].toString() == "request")
    {

        if (obj["request"].toString() == "rights")
        {
            listenerRights rightsList(obj);
            emit rightsRequest(rightsList);
        }

        else if (obj["request"].toString() == "control")
        {
            emit controlRequest(obj["key"].toString());
        }
    }
}


void ListenerObj::writeToSocket(QByteArray json)
{
    int size = json.size();
    std::string strSize = std::to_string(size);
    json.prepend(strSize.c_str());

    listenerSocket->write(json);
}


void ListenerObj::writeToSocket(QString str)
{
    int size = str.size();
    QByteArray arr = str.toUtf8();
    std::string strSize = std::to_string(size);
    arr.prepend(strSize.c_str());

    listenerSocket->write(arr);
}


listenerInfo ListenerObj::getListenerInfo()
{
    listenerInfo info;
    info.name = listenerName;
    info.IP = listenerSocket->peerAddress().toString();
    info.port = QString::number(listenerSocket->peerPort());
    info.listenerType = listenerType;

    return info;
}


QString ListenerObj::getListenerName()
{
    return listenerName;
}


QTcpSocket* ListenerObj::getListenerSocket()
{
    return listenerSocket;
}
