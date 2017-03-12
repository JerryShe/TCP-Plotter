#ifndef INFORMATIONSTRUCTS_H
#define INFORMATIONSTRUCTS_H


#include <QString>
#include <QVector>

struct streamInfo
{
    QString streamName;
    QString streamDescription;
    QStringList streamVariables;
    QStringList streamVariablesDescription;
};


struct deviceInfo
{
    QString deviceName;
    QString deviceDescription;
    QVector <streamInfo> streams;
};


struct connectionsInfo
{
    QVector <deviceInfo> devices;
};


#endif // INFORMATIONSTRUCTS_H
