#include "importvoucherlist.h"
#include "ui_importvoucherlist.h"

ImportVoucherList::ImportVoucherList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImportVoucherList)
{
    ui->setupUi(this);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setColumnWidth(1,25);
    ui->tableWidget->horizontalHeader()->hide();
}

ImportVoucherList::~ImportVoucherList()
{
    delete ui;
}

void ImportVoucherList::setVouchers(QList<QString> vNumbers)
{
    ui->tableWidget->setRowCount(vNumbers.size());

    for(int i=0;i<vNumbers.size();i++){
        QTableWidgetItem *vno = new QTableWidgetItem;
        vno->setText(vNumbers[i]);
        vno->setFlags(vno->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(i,0,vno);

        QCheckBox *box = new QCheckBox();
        ui->tableWidget->setCellWidget(i,1,box);
    }
}

void ImportVoucherList::on_buttonBox_accepted()
{
    QStringList Vnumbers;
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        if(qobject_cast<QCheckBox*> (ui->tableWidget->cellWidget(i,1))->isChecked()){
            Vnumbers.append(ui->tableWidget->item(i,0)->text());
        }
    }
    emit itemsList(Vnumbers);
    this->close();
}

void ImportVoucherList::on_buttonBox_rejected()
{
    this->close();
}
