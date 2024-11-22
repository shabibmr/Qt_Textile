#include "reportsmainwindow.h"
#include "ui_reportsmainwindow.h"

ReportsMainWindow::ReportsMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReportsMainWindow)
{
    ui->setupUi(this);
}

ReportsMainWindow::~ReportsMainWindow()
{
    delete ui;
}
