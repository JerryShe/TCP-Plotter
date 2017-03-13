#include "informationstructs.h"


packet::packet()
{}

packet::packet(const QString &DeviceName)
{
    deviceName = DeviceName;
}

void packet::addVar(double var)
{
    data.append(var);
}
