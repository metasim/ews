#ifndef EWSMAINWINDOW_H
#define EWSMAINWINDOW_H

#include <QtGui/QMainWindow>

namespace Ui
{
    class EWSMainWindow;
}

class EWSMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    EWSMainWindow(QWidget *parent = 0);
    ~EWSMainWindow();

private:
    Ui::EWSMainWindow *ui;
};

#endif // EWSMAINWINDOW_H
