#include "salesinventoryopening.h"
#include "ui_salesinventoryopening.h"
#include <QLineEdit>
#include <QMessageBox>

SalesInventoryOpening::SalesInventoryOpening(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SalesInventoryOpening)
{
    ui->setupUi(this);
    dbHelper = new SalesInventoryItemDatabaseHelper;
    itemsList = dbHelper->getALLInventoryObjectsAsPtr();

    ui->lineEdit->hide();

    ui->tableWidget->setColumnCount(6);
    int i=0;

    QTableWidgetItem* ItemName = new QTableWidgetItem;
    ItemName->setText("Item Name");
    ui->tableWidget->setHorizontalHeaderItem(i++,ItemName);

    QTableWidgetItem* ModelNumber = new QTableWidgetItem;
    ModelNumber->setText("Model No");
    ui->tableWidget->setHorizontalHeaderItem(i++,ModelNumber);

    QTableWidgetItem* Brand = new QTableWidgetItem;
    Brand->setText("Brand");
    ui->tableWidget->setHorizontalHeaderItem(i++,Brand);

    QTableWidgetItem* OpeningStkQty = new QTableWidgetItem;
    OpeningStkQty->setText("Opening Qty");
    ui->tableWidget->setHorizontalHeaderItem(i++,OpeningStkQty);

    QTableWidgetItem* OpeningStkRate = new QTableWidgetItem;
    OpeningStkRate->setText("Opening Rate");
    ui->tableWidget->setHorizontalHeaderItem(i++,OpeningStkRate);

    QTableWidgetItem* OpeningStkVal = new QTableWidgetItem;
    OpeningStkVal->setText("Opening Value");
    ui->tableWidget->setHorizontalHeaderItem(i++,OpeningStkVal);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);


    setTable();




}

SalesInventoryOpening::~SalesInventoryOpening()
{
    delete ui;
}

void SalesInventoryOpening::setTable()
{
    int rowCount = itemsList.size();

    ui->tableWidget->setRowCount(rowCount);

    for(int j=0;j<rowCount;j++){

        if((text.length() == 0) || ((text.length()>0)&&
                                    (itemsList[j]->ItemName.contains(text,Qt::CaseInsensitive)||
                                     itemsList[j]->brandName.contains(text,Qt::CaseInsensitive)||
                                     itemsList[j]->ItemCode.contains(text,Qt::CaseInsensitive)
                                     ))){
            int i=0;
            QTableWidgetItem* ItemName = new QTableWidgetItem;
            ItemName->setText(itemsList[j]->ItemName);
            ui->tableWidget->setItem(j,i++,ItemName);

            QTableWidgetItem* ModelNumber = new QTableWidgetItem;
            ModelNumber->setText(itemsList[j]->ItemCode);
            ui->tableWidget->setItem(j,i++,ModelNumber);

            QTableWidgetItem* Brand = new QTableWidgetItem;
            Brand->setText(itemsList[j]->brandName);
            ui->tableWidget->setItem(j,i++,Brand);

            QLineEdit* OpeningStkQty = new QLineEdit();
            OpeningStkQty->setAlignment(Qt::AlignRight);
            OpeningStkQty->setValidator(new QDoubleValidator(0,99999,2,this));
            OpeningStkQty->setProperty("row",j);
            OpeningStkQty->setText(QString::number(itemsList[j]->OpeningStock));
            ui->tableWidget->setCellWidget(j,i++,OpeningStkQty);
            QObject::connect(OpeningStkQty,SIGNAL(returnPressed()),SLOT(itemQtyEntered()));

            QLineEdit* OpeningStkRate = new QLineEdit;
            OpeningStkRate ->setAlignment(Qt::AlignRight);
            OpeningStkRate ->setValidator(new QDoubleValidator(0,99999,2,this));
            OpeningStkRate ->setProperty("row",j);
            OpeningStkRate->setText(QString::number(itemsList[j]->OpeningStockPrice));
            ui->tableWidget->setCellWidget(j,i++,OpeningStkRate);
            qDebug()<<itemsList[j]->OpeningStockPrice;

            QObject::connect(OpeningStkRate,SIGNAL(returnPressed()),SLOT(itemRateEntered()));

            QLineEdit* OpeningStkVal = new QLineEdit;
            OpeningStkVal ->setAlignment(Qt::AlignRight);
            OpeningStkVal ->setValidator(new QDoubleValidator(0,99999,2,this));
            OpeningStkVal ->setProperty("row",j);
            OpeningStkVal->setText(QString::number(itemsList[j]->OpeningStockValue));
            ui->tableWidget->setCellWidget(j,i++,OpeningStkVal);
            QObject::connect(OpeningStkVal,SIGNAL(returnPressed()),SLOT(itemValueEntered()));


        }

    }
}

void SalesInventoryOpening::on_pushButton_clicked()
{
    //    for(int i=0;i<itemsList.size();i++){

    //        dbHelper->updateOpening(itemsList[i]->ItemID,itemsList[i]->OpeningStock,itemsList[i]->OpeningStockPrice,
    //                                itemsList[i]->OpeningStockValue);
    //    }
    //    this->close();
    QMessageBox box; box.setText("Saved"); box.exec();
}

void SalesInventoryOpening::on_pushButton_2_clicked()
{
    this->close();
}

void SalesInventoryOpening::itemQtyEntered()
{
    int row = sender()->property("row").toInt();

    QLineEdit* rate = qobject_cast<QLineEdit*> (ui->tableWidget->cellWidget(row,4));
    QLineEdit* qty = qobject_cast<QLineEdit*> (ui->tableWidget->cellWidget(row,3));

    itemsList[row]->OpeningStock = qty->text().toFloat();

    rate->setFocus();
    rate->selectAll();
}

void SalesInventoryOpening::itemRateEntered()
{
    int row = sender()->property("row").toInt();

    QLineEdit* rate = qobject_cast<QLineEdit*> (ui->tableWidget->cellWidget(row,4));
    QLineEdit* qty = qobject_cast<QLineEdit*> (ui->tableWidget->cellWidget(row,3));

    QLineEdit* val = qobject_cast<QLineEdit*> (ui->tableWidget->cellWidget(row,5));


    val->blockSignals(true);
    itemsList[row]->OpeningStock = qty->text().toFloat();
    itemsList[row]->OpeningStockPrice = rate->text().toFloat();

    itemsList[row]->OpeningStockValue = itemsList[row]->OpeningStockPrice *itemsList[row]->OpeningStock ;

    val->setText(QString::number(itemsList[row]->OpeningStockValue ,'f',2));

    if(row<itemsList.size()-1){
        QLineEdit* qtyNext = qobject_cast<QLineEdit*> (ui->tableWidget->cellWidget(row+1,3));
        qtyNext->setFocus();
        qtyNext->selectAll();
    }

    dbHelper->updateOpening(itemsList[row]->ItemID,itemsList[row]->OpeningStock,itemsList[row]->OpeningStockPrice,
                            itemsList[row]->OpeningStockValue);
    val->blockSignals(false);
}

void SalesInventoryOpening::itemValueEntered()
{
    int row = sender()->property("row").toInt();

    QLineEdit* rate = qobject_cast<QLineEdit*> (ui->tableWidget->cellWidget(row,4));
    QLineEdit* qty = qobject_cast<QLineEdit*> (ui->tableWidget->cellWidget(row,3));

    QLineEdit* val = qobject_cast<QLineEdit*> (ui->tableWidget->cellWidget(row,5));

    rate->blockSignals(true);
    itemsList[row]->OpeningStock = qty->text().toFloat();
    itemsList[row]->OpeningStockValue = val->text().toFloat();

    itemsList[row]->OpeningStockPrice = itemsList[row]->OpeningStockValue / itemsList[row]->OpeningStock ;

    rate->setText(QString::number(itemsList[row]->OpeningStockValue ,'f',2));

    if(row<itemsList.size()-1){
        QLineEdit* qtyNext = qobject_cast<QLineEdit*> (ui->tableWidget->cellWidget(row+1,3));
        qtyNext->setFocus();
    }

    dbHelper->updateOpening(itemsList[row]->ItemID,itemsList[row]->OpeningStock,itemsList[row]->OpeningStockPrice,
                            itemsList[row]->OpeningStockValue);

    rate->blockSignals(false);
}

void SalesInventoryOpening::on_lineEdit_returnPressed()
{
    text = ui->lineEdit->text();
    setTable();
}
