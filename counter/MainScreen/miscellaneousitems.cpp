#include "miscellaneousitems.h"
#include "ui_miscellaneousitems.h"

MiscellaneousItems::MiscellaneousItems(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MiscellaneousItems)
{
    ui->setupUi(this);

    miscItem = new CompoundItemDataObject();
}

MiscellaneousItems::~MiscellaneousItems()
{
    delete ui;
}

void MiscellaneousItems::on_fruitsButton_clicked()
{
    ui->lineEdit->setText("FRUITS");
    miscItem->BaseItem.ItemID = "0x1";
}

void MiscellaneousItems::on_vegetablesButton_clicked()
{
    ui->lineEdit->setText("VEGETABLES");
    miscItem->BaseItem.ItemID = "0x2";
}

void MiscellaneousItems::on_OKButton_clicked()
{
    itemhelper = new SalesInventoryItemDatabaseHelper();
    if(miscItem->BaseItem.ItemID != "0x1" && miscItem->BaseItem.ItemID != "0x2"){
        miscItem->BaseItem.ItemID = "0x3";
    }
    if(ui->lineEdit->text().size()>0){
        miscItem->BaseItem = itemhelper->getInventoryItemByID(miscItem->BaseItem.ItemID);
        miscItem->BaseItem.ItemName = ui->lineEdit->text();
        miscItem->BaseItem.quantity = 1;

        emit misc(*miscItem);
        this->close();
    }
}

void MiscellaneousItems::on_lineEdit_returnPressed()
{
    on_OKButton_clicked();
}
