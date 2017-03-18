#include "devicemanagerwindow.h"
#include "ui_devicemanagerwindow.h"

#include <QDebug>


DeviceManagerWindow::DeviceManagerWindow(QWidget *parent) : QWidget(parent), ui(new Ui::DeviceManagerWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    deviceModel = new QStandardItemModel(this);
    dataModel = new QStandardItemModel(this);

    manager = DeviceManager::getDataManager();
    connect(manager, SIGNAL(deviceListChanged(deviceConnectionsInfo)), this, SLOT(updateDeviceData(deviceConnectionsInfo)));
    updateDeviceData(manager->getConnectionsInfo());
    this->show();
}


DeviceManagerWindow::~DeviceManagerWindow()
{
    delete ui;
}


void DeviceManagerWindow::updateDeviceData(deviceConnectionsInfo info)
{
    deviceModel->clear();
    QStandardItem* deviceRoot = deviceModel->invisibleRootItem();

    for (int i = 0; i < info.devices.size(); i++)
    {
        QStandardItem* deviceItem = new QStandardItem(info.devices[i].deviceName);
        deviceItem->setToolTip(info.devices[i].deviceDescription);

        deviceRoot->appendRow(deviceItem);

        for (int j = 0; j < info.devices[i].streams.size(); j++)
        {
            QStandardItem* streamItem = new QStandardItem(info.devices[i].streams[i].streamName);
            streamItem->setToolTip(info.devices[i].streams[j].streamDescription);

            deviceItem->appendRow(streamItem);

            for (int k = 0; k < info.devices[i].streams[j].streamVariables.size(); k++)
            {
                QStandardItem* varItem = new QStandardItem(info.devices[i].streams[j].streamVariables[k]);
                varItem->setToolTip(info.devices[i].streams[j].streamVariablesDescription[k]);
                streamItem->appendRow(varItem);
            }
        }
    }
    ui->DeviceView->setModel(deviceModel);
}


void DeviceManagerWindow::on_CloseButton_clicked()
{
    emit finished();
    this->close();
}

void DeviceManagerWindow::on_OkButton_clicked()
{
    emit finished();
    this->close();
}
