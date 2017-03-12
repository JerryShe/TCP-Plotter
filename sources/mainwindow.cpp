#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graphwindow.h"

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

    NetworkThread = new QThread(this);
    connect(this, SIGNAL(destroyed(QObject*)), NetworkThread, SLOT(quit()));
    NetworkWorker = new TcpWorker(Manager);
    NetworkWorker->moveToThread(NetworkThread);

    NetworkThread->start();
    ManagerThread->start();


    NetworkWorker->start(Manager);
    NetworkWorker->openPort("666");
    ui->logsList->addItem("Server started at port 666");


    connect(NetworkWorker, &TcpWorker::connectionDone, [this](QTcpSocket* socket){addNewConnection(socket, false);});
    connect(NetworkWorker, SIGNAL(newConnection(QTcpSocket*)), this, SLOT(incommingConnection(QTcpSocket*)));
    connect(NetworkWorker, SIGNAL(disconnected(QStringList)), this, SLOT(deleteConnection(QStringList)));

    ui->IPLabel->setText(NetworkWorker->getLocalIP());
}


MainWindow::~MainWindow()
{
    NetworkThread->wait();
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


void MainWindow::addNewConnection(QTcpSocket *socket, const bool type)
{
    ui->connectionTable->insertRow(0);
    ui->connectionTable->setItem(0, 0, new QTableWidgetItem(socket->peerName()));
    ui->connectionTable->setItem(0, 1, new QTableWidgetItem(socket->peerAddress().toString()));
    ui->connectionTable->setItem(0, 2, new QTableWidgetItem(QString::number(socket->peerPort())));

    QColor rowColor;
    if (type)
        rowColor.setRgb(230,230,230);
    else
        rowColor.setRgb(56,245,188);

    for (int i = 0; i < 3; i++)
        ui->connectionTable->item(0, i)->setBackgroundColor(rowColor);

    ui->logsList->addItem("New connection with " + socket->peerName() + " by IP: " + socket->peerAddress().toString());
}


void MainWindow::deleteConnection(QStringList socketData)
{
    int pos = -1;
    //qDebug()<<socketData;
    for (int i = 0; i < ui->connectionTable->rowCount(); i++)
    {
        if (//ui->connectionTable->item(i, 0)->text() == socketData[0] &&
                ui->connectionTable->item(i, 1)->text() == socketData[1] &&
                ui->connectionTable->item(i, 2)->text() == socketData[2])
            pos = i;
    }

    if (pos != -1)
    {
        ui->logsList->addItem("Disconnected with " + ui->connectionTable->item(pos,0)->text()
                              + " with IP: " + ui->connectionTable->item(pos,1)->text());

        ui->connectionTable->removeRow(pos);
    }
}



void MainWindow::on_ConnectTo_clicked()
{
    NetworkWorker->newConnectionTo(ui->ConnectIP->text(), ui->ConnectPort->text());
}


void MainWindow::incommingConnection(QTcpSocket *socket)
{
    int result = QMessageBox::question(this, "New connection", "New device wants to connect: \n"
                                      "\n" + socket->peerName() +
                                      "\n" + socket->peerAddress().toString() +
                                      "\n Establish a connection?",
                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if (result != QMessageBox::Yes)
    {
        socket->disconnectFromHost();
    }
    else
    {
        addNewConnection(socket, true);
    }
}
