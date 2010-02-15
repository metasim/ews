#include "EWSMainWindow.h"
#include "ui_EWSMainWindow.h"

EWSMainWindow::EWSMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::EWSMainWindow)
{
    ui->setupUi(this);
}

EWSMainWindow::~EWSMainWindow()
{
    delete ui;
}
