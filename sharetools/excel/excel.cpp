#include "excel.h"
#include "ui_excel.h"

Excel::Excel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Excel)
{
    ui->setupUi(this);
}

Excel::~Excel()
{
    delete ui;
}
