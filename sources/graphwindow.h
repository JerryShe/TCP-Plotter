#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>

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

private:
    Ui::GraphWindow *ui;

    QMdiSubWindow* Window;
};

#endif // GRAPHWINDOW_H
