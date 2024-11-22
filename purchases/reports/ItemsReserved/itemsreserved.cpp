#include "itemsreserved.h"
#include "ui_itemsreserved.h"

ItemsReserved::ItemsReserved(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemsReserved)
{
    ui->setupUi(this);
}

ItemsReserved::~ItemsReserved()
{
    delete ui;
}

void ItemsReserved::on_OKButton_clicked()
{

}

void ItemsReserved::on_excelButton_clicked()
{

}

void ItemsReserved::on_printButton_clicked()
{

}

void ItemsReserved::on_backButton_clicked()
{

}
