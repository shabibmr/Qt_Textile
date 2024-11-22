#include "fifopopupdialog.h"
#include "ui_fifopopupdialog.h"
#include <QLabel>

FifoPopupDialog::FifoPopupDialog(inventoryItemDataModel *item, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FifoPopupDialog)
{
    ui->setupUi(this);
    this->item=item;

    QLabel* DialogLabel = new QLabel(this);
    DialogLabel->setText("Batches");

    QLabel* itemLabel = new QLabel(this);
    itemLabel->setText("Item : "+item->ItemName);

    ui->labelLay->addWidget(DialogLabel);
    ui->labelLay->addWidget(itemLabel);

    ui->QtyLine->setText(QString::number(item->quantity,'f',item->uomObject.decimalPoints));

    QTableView *table = new QTableView(this);

    QStringList headers;
    headers<<"SlNo"<<"Date"<<"Section"<<"Qty"<<"Price"<<"Margin %"<<"Margin Value"<<"Selling Price";
//    GMFifoModel *model = new GMFifoModel(item,headers);
//    table->setModel(model);

//    ui->tableLay->addWidget(table);

}

FifoPopupDialog::~FifoPopupDialog()
{
    delete ui;
}

void FifoPopupDialog::on_pushButton_clicked()
{
    this->close();
}


void FifoPopupDialog::calculate()
{
    ui->rateLine->setText(QString::number(item->price,'f',2));
}
