#include "physicalstockregister.h"
#include "ui_physicalstockregister.h"

PhysicalStockRegister::PhysicalStockRegister(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PhysicalStockRegister)
{
    ui->setupUi(this);
}

PhysicalStockRegister::~PhysicalStockRegister()
{
    delete ui;
}

void PhysicalStockRegister::on_OKButton_clicked()
{

}

void PhysicalStockRegister::on_excelButton_clicked()
{

}

void PhysicalStockRegister::on_printButton_clicked()
{

}

void PhysicalStockRegister::on_backButton_clicked()
{

}
