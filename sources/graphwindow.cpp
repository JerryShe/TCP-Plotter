#include "graphwindow.h"
#include "ui_graphwindow.h"
#include "devicemanagerwindow.h"

#include <QColorDialog>
#include <QDialog>


#include <QDebug>

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

    ui->RangeSlider->setValue(100);

    connect(ui->graph->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(updateRangeData(QCPRange)));
    ui->graph->xAxis->setRange(0, 50);


    QCPGraph* grap = ui->graph->addGraph();

    addPoint(1,1);
    addPoint(2,2);
    addPoint(3,3);
    addPoint(4,4);
    addPoint(5,5);
    addPoint(6,6);
    addPoint(7,7);
    addPoint(10,10);
    addPoint(100,0);

    ui->graph->replot();
}


bool GraphWindow::eventFilter(QObject *watched, QEvent *event)
{

}


void GraphWindow::addPoint(double t, double v)
{
    ui->graph->graph(0)->addData(t, v);
    if (t > lastTPoint)
    {
        lastTPoint = t;
        ui->RangeBox->setMaximum(lastTPoint);
        ui->RangeEnd->setMaximum(lastTPoint);
        ui->RangeStart->setMaximum(lastTPoint-1);

        if(ui->RangeEnd->value() == ui->graph->xAxis->range().upper)
            ui->graph->xAxis->setRange(t, ui->graph->xAxis->range().lower + (t - ui->graph->xAxis->range().upper));
    }
}


void GraphWindow::updateRangeData(QCPRange range)
{
    ui->RangeStart->setValue((int)(range.lower));
    ui->RangeEnd->setValue((int)(range.upper));
    ui->RangeBox->setValue((int)(range.upper - range.lower));
    updateRangeSlider(range);
}


void GraphWindow::updateRangeSlider(QCPRange range)
{
    if(ui->RangeBox->value() > lastTPoint)
        return;

    ui->RangeSlider->blockSignals(true);

    double x2 = (lastTPoint - ui->RangeBox->value())/100.0;
    if (x2 > 0)
    {
        int slPos = (int)(range.lower / x2);
        ui->RangeSlider->setValue(slPos);
    }
    else
        ui->RangeBox->setValue(100);

    ui->RangeSlider->blockSignals(false);
}


void GraphWindow::on_BgColorButton_clicked()
{
    QColor newColor = QColorDialog::getColor();
    if (newColor.isValid())
    {
        ui->graph->setBackground(QBrush(newColor));
        ui->BgColorButton->setStyleSheet("#BgColorButton{background-color: " + newColor.toRgb().name() + ";}");
    }
}


void GraphWindow::on_GraphTypeBox_activated(int index)
{

}


void GraphWindow::on_RangeStart_valueChanged(int arg1)
{
    ui->graph->xAxis->setRange(arg1, ui->graph->xAxis->range().upper);
    ui->graph->replot();
}


void GraphWindow::on_RangeEnd_valueChanged(int arg1)
{
    if (arg1 > lastTPoint)
        arg1 = (int)lastTPoint;

    ui->graph->xAxis->setRange(ui->graph->xAxis->range().lower, arg1);

    ui->graph->replot();
}


void GraphWindow::on_RangeBox_valueChanged(int arg1)
{
    double x1 = ui->graph->xAxis->range().upper - (double)arg1;
    double x2 = ui->graph->xAxis->range().upper;
    if (x1 < 0)
    {
        x1 = 0;
        x2 = (double)ui->RangeBox->value();
    }
    ui->graph->xAxis->setRange(x1, x2);
    ui->graph->replot();
}


void GraphWindow::on_ScaleSlider_valueChanged(int value)
{

}


void GraphWindow::on_GridColorButton_clicked()
{

}


void GraphWindow::on_GridTypeBox_activated(const QString &arg1)
{

}


void GraphWindow::on_RangeSlider_valueChanged(int value)
{
    if(ui->RangeBox->value() > lastTPoint)
        return;

    int pos = ((lastTPoint - ui->RangeBox->value())/100.0) * (double)value;
    if (pos<0)
        pos = 0;

    ui->graph->xAxis->setRange(pos, pos + ui->RangeBox->value());
}


void GraphWindow::on_AddGraphButton_clicked()
{
    DeviceManagerWindow* DeviceWindow = new DeviceManagerWindow(this);
    connect(DeviceWindow, SIGNAL(finished()), this, SLOT(updateGraphs()));
    ui->centralwidget->layout()->addWidget(DeviceWindow);
    ui->GraphWidget->hide();
}


void GraphWindow::updateGraphs()
{
    ui->GraphWidget->show();
}
