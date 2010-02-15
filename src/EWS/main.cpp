#include <QtGui/QApplication>
#include "EWSMainWindow.h"

/**
  *
  */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EWSMainWindow w;
    w.show();
    return a.exec();
}
