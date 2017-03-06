#include "graphwindow.h"
#include "ui_graphwindow.h"

GraphWindow::GraphWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GraphWindow)
{
    ui->setupUi(this);
}

GraphWindow::~GraphWindow()
{
    delete ui;
}


void GraphWindow::setWindow(QMdiSubWindow *win)
{
    Window = win;

    QPixmap pm = QPixmap(1,1);
    pm.fill(QColor(0,0,0,0));
    Window->setWindowIcon(QIcon(pm));
    Window->setWindowTitle("Noname graph");

    QCPGraph* grap = ui->graph->addGraph();
    grap->addData(1,1);
    grap->addData(2,2);
    grap->addData(3,3);
    grap->addData(4,4);
    grap->addData(5,5);
    grap->addData(6,6);
    grap->addData(7,7);
    grap->addData(10,10);
    //ui->graph->replot();
}
