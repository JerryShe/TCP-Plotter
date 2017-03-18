#ifndef DEVICEMANAGERWINDOW_H
#define DEVICEMANAGERWINDOW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>

#include "datamanager.h"
#include "informationstructs.h"

namespace Ui
{
    class DeviceManagerWindow;
}


class DeviceManagerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceManagerWindow(QWidget *parent = 0);
    ~DeviceManagerWindow();

private slots:
    void updateDeviceData(deviceConnectionsInfo deviceInfo);


    void on_CloseButton_clicked();

    void on_OkButton_clicked();


signals:
    finished();


private:
    Ui::DeviceManagerWindow *ui;

    QStandardItemModel* deviceModel;
    QStandardItemModel* dataModel;


    DeviceManager* manager;
};

#endif // DEVICEMANAGERWINDOW_H
