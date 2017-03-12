#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTcpSocket>


#include "tcpworker.h"
#include "datamanager.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionAdd_graphs_window_triggered();
    void addNewTab();

    void on_ChangePort_clicked();

    void addNewConnection(QTcpSocket *socket, const bool type);
    void deleteConnection(QStringList socketData);

    void incommingConnection(QTcpSocket *socket);
    void on_ConnectTo_clicked();

private:
    Ui::MainWindow *ui;

    QThread* NetworkThread;
    TcpWorker* NetworkWorker;

    QThread* ManagerThread;
    DataManager* Manager;

};

#endif // MAINWINDOW_H
