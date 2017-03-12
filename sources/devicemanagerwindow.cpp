#include "devicemanagerwindow.h"
#include "ui_devicemanagerwindow.h"

DeviceManagerWindow::DeviceManagerWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceManagerWindow)
{
    ui->setupUi(this);
}

DeviceManagerWindow::~DeviceManagerWindow()
{
    delete ui;
}
