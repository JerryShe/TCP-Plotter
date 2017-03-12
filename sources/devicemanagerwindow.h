#ifndef DEVICEMANAGERWINDOW_H
#define DEVICEMANAGERWINDOW_H

#include <QWidget>

namespace Ui {
class DeviceManagerWindow;
}

class DeviceManagerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceManagerWindow(QWidget *parent = 0);
    ~DeviceManagerWindow();

private:
    Ui::DeviceManagerWindow *ui;
};

#endif // DEVICEMANAGERWINDOW_H
