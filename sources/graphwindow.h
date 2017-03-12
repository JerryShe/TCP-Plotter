#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>

#include "qcustomplot.h"

namespace Ui {
class GraphWindow;
}

class GraphWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GraphWindow(QWidget *parent = 0);
    ~GraphWindow();

    void setWindow(QMdiSubWindow* win);
    void addPoint(double t, double v);

private slots:
    void on_RangeStart_valueChanged(int arg1);
    void on_RangeEnd_valueChanged(int arg1);
    void on_RangeBox_valueChanged(int arg1);

    void updateRangeData(QCPRange range);
    void updateRangeSlider(QCPRange range);
    void on_RangeSlider_valueChanged(int value);


    void on_ScaleSlider_valueChanged(int value);

    void on_BgColorButton_clicked();
    void on_GraphTypeBox_activated(int index);

    void on_GridColorButton_clicked();
    void on_GridTypeBox_activated(const QString &arg1);


    void on_AddGraphButton_clicked();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::GraphWindow *ui;
    double lastTPoint;

    QMdiSubWindow* Window;
};

#endif // GRAPHWINDOW_H
