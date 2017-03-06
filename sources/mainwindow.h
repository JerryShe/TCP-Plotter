#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "tcpworker.h"

#include <QTcpSocket>


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
    void deleteConnection(QTcpSocket* socket);

    void incommingConnection(QTcpSocket *socket);
    void on_ConnectTo_clicked();

private:
    Ui::MainWindow *ui;
    QThread* NetworkThread;
    TcpWorker* NetworkWorker;

    QList <QTcpSocket*> connectionList;
};

#endif // MAINWINDOW_H
