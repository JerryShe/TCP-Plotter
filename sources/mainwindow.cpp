#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graphwindow.h"
#include "informationstructs.h"

#include <QMdiSubWindow>
#include <QMdiArea>
#include <QMessageBox>


#include <QDebug>



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();

    delete ui->tab1;
    ui->tabWidget->addTab(new QMdiArea(ui->tabWidget), "Page " + QString::number(ui->tabWidget->count() + 1));

    QPushButton* newTabButton = new QPushButton("+", ui->tabWidget);
    connect(newTabButton, SIGNAL(clicked(bool)), this, SLOT(addNewTab()));
    newTabButton->setFixedSize(25,25);
    ui->tabWidget->setCornerWidget(newTabButton, Qt::TopLeftCorner);
    newTabButton->show();

    qDebug()<<"gui " << this->thread();

    ManagerThread = new QThread(this);
    connect(this, SIGNAL(destroyed(QObject*)), ManagerThread, SLOT(quit()));
    Manager = DataManager::getDataManager();
    Manager->moveToThread(ManagerThread);

    //NetworkThread = new QThread(this);
    //connect(this, SIGNAL(destroyed(QObject*)), NetworkThread, SLOT(quit()));
    //NetworkWorker->moveToThread(NetworkThread);

    //NetworkThread->start();
    ManagerThread->start();

    NetworkWorker = new TcpWorker(Manager);
    qDebug()<<"manager "<<Manager->thread();

    NetworkWorker->openPort("666");
    ui->logsList->addItem("Server started at port 666");

    connect(Manager, SIGNAL(deviceDisconnected(deviceConnectionInfo)), this, SLOT(deleteConnection(deviceConnectionInfo)));
    connect(Manager, SIGNAL(deviceConnected(deviceConnectionInfo)), this, SLOT(addNewConnection(deviceConnectionInfo)));

    ui->IPLabel->setText(NetworkWorker->getLocalIP());
}


MainWindow::~MainWindow()
{
    //NetworkThread->wait();
    ManagerThread->wait();
    delete ui;
}


void MainWindow::addNewTab()
{
    ui->tabWidget->addTab(new QMdiArea(this), "Page" + QString::number(ui->tabWidget->count() + 1));
}


void MainWindow::on_actionAdd_graphs_window_triggered()
{
    QMdiArea* workArea = static_cast<QMdiArea*>(ui->tabWidget->currentWidget());

    if (workArea == 0)
    {
        qDebug()<<"work area wtf";
        return;
    }

    GraphWindow* newGraph = new GraphWindow(workArea);
    QMdiSubWindow* newSub = workArea->addSubWindow(newGraph);

    newGraph->setWindow(newSub);
    newSub->setGeometry(newGraph->x(), newGraph->y(), ui->tabWidget->currentWidget()->width()/2, ui->tabWidget->currentWidget()->height()/2);
    newSub->show();
}


void MainWindow::on_ChangePort_clicked()
{
    if (    ui->OpeningPort->text().isEmpty() ||
            ui->OpeningPort->text().toShort() == NetworkWorker->getServerPort() ||
            ui->OpeningPort->text().toUShort() > 65535 || ui->OpeningPort->text().toUShort() < 1)
        return;

    if (!NetworkWorker->openPort(ui->OpeningPort->text()))
    {
        QMessageBox::information(this, QObject::tr("Error!"), QObject::tr("Failed to change port!"));
        return;
    }

    ui->logsList->addItem("Server port changed to " + ui->OpeningPort->text());
}


void MainWindow::addNewConnection(deviceConnectionInfo deviceInfo)
{
    int result = QMessageBox::question(this, "New connection", "New device '"
                                      + deviceInfo.deviceName + "' "
                                      "with IP "
                                      "\n" + deviceInfo.peerIP +
                                      "\n wants to connect."
                                      "\n\n Establish a connection?",
                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if (result == QMessageBox::Yes)
        Manager->confirmConnection(deviceInfo.deviceSocket);
    else
        Manager->deleteDevice(deviceInfo.deviceSocket);


    qDebug()<<"add new connection with " << deviceInfo.deviceName;
    ui->connectionTable->insertRow(0);
    ui->connectionTable->setItem(0, 0, new QTableWidgetItem(deviceInfo.deviceName));
    ui->connectionTable->item(0,0)->setToolTip(deviceInfo.deviceDescription);

    ui->connectionTable->setItem(0, 1, new QTableWidgetItem(deviceInfo.peerIP));
    ui->connectionTable->setItem(0, 2, new QTableWidgetItem(deviceInfo.peerPort));

    ui->logsList->addItem("New connection with " + deviceInfo.deviceName + " by IP: " + deviceInfo.peerIP);
}


void MainWindow::deleteConnection(deviceConnectionInfo deviceInfo)
{
    qDebug()<<"delete connection with " << deviceInfo.deviceName;

    int pos = -1;
    for (int i = 0; i < ui->connectionTable->rowCount(); i++)
        if (ui->connectionTable->item(i, 0)->text() == deviceInfo.deviceName)
            pos = i;

    if (pos != -1)
    {
        ui->logsList->addItem("Disconnected with " + deviceInfo.deviceName
                              + " with IP: " + ui->connectionTable->item(pos,1)->text());

        ui->connectionTable->removeRow(pos);
    }
}


void MainWindow::on_ConnectTo_clicked()
{
    NetworkWorker->newConnectionTo(ui->ConnectIP->text(), ui->ConnectPort->text());
}
