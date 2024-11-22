#include "stockrequestform.h"
#include "ui_stockrequestform.h"

StockRequestForm::StockRequestForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StockRequestForm)
{
    ui->setupUi(this);
    ui->voucherDateEdit->setDate(QDate::currentDate());
    ui->addedByName->setText(LoginValues::userName);
    obj = new GeneralVoucherDataObject;

    dbHelper = new StockRequestDatabaseHelper();


    itemsmodel = itemHelper.getInventoryItemsQueryModelForSearch(2);


    QString vno = QString::number(dbHelper->getMaxID()+1);
    ui->voucherNoLineEdit->setText(vno);
    int i=0;
    ui->tableWidget->setColumnCount(5);
    QTableWidgetItem* SLNO = new QTableWidgetItem;
    SLNO->setText("#");
    ui->tableWidget->setHorizontalHeaderItem(i++,SLNO);

    QTableWidgetItem* dateItem = new QTableWidgetItem;
    dateItem->setText("Item");
    ui->tableWidget->setHorizontalHeaderItem(i++,dateItem);

    QTableWidgetItem* LedgerName = new QTableWidgetItem;
    LedgerName->setText("Request");
    ui->tableWidget->setHorizontalHeaderItem(i++,LedgerName);

    QTableWidgetItem* ReqNo = new QTableWidgetItem;
    ReqNo->setText("Issue");
    ui->tableWidget->setHorizontalHeaderItem(i++,ReqNo);

    QTableWidgetItem* dele = new QTableWidgetItem;
    dele->setText("Delete");
    ui->tableWidget->setHorizontalHeaderItem(i++,dele);

    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setColumnWidth(0,25);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->setColumnWidth(4,25);


    setTable();

}

StockRequestForm::~StockRequestForm()
{
    delete ui;
}

void StockRequestForm::setTable()
{
    qDebug()<<"Setting table";
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setRowCount(obj->InventoryItems.size()+1);
    int i=0;
    for(;i<obj->InventoryItems.size();i++){

        QTableWidgetItem* SLNO = new QTableWidgetItem;
        SLNO->setText(QString::number(i+1));
        ui->tableWidget->setItem(i,0,SLNO);

        GMItemSearchLite *search = new GMItemSearchLite(itemsmodel);
        QToolButton *buttonlay = new QToolButton(this);
        QHBoxLayout *lay = new QHBoxLayout(buttonlay);
        lay->setContentsMargins(0,0,0,0);
        lay->addWidget(search);
        search->setText(obj->InventoryItems[i].BaseItem.ItemName);
        ui->tableWidget->setCellWidget(i,1,buttonlay);
        search->setProperty("row",i);

        QObject::connect(search,SIGNAL(SelectedItemID(inventoryItemDataModel)),this,
                         SLOT(ItemSelected(inventoryItemDataModel)));

        QLineEdit* reqQty = new QLineEdit;
        reqQty->setAlignment(Qt::AlignRight);
        reqQty->setText(QString::number(obj->InventoryItems[i].BaseItem.drQty));
        ui->tableWidget->setCellWidget(i,2,reqQty);
        reqQty->setProperty("row",i);
        QObject::connect(reqQty,SIGNAL(editingFinished()),this,
                         SLOT(reqQtyEntered()));

        QLineEdit* issQty = new QLineEdit;
        issQty->setAlignment(Qt::AlignRight);
        issQty->setText(QString::number(obj->InventoryItems[i].BaseItem.crQty));
        ui->tableWidget->setCellWidget(i,3,issQty);
        issQty->setProperty("row",i);
        QObject::connect(issQty,SIGNAL(editingFinished()),this,
                         SLOT(issQtyEntered()));

        QToolButton* dele = new QToolButton(this);
        dele->setIcon(QIcon(":/icons/trash.ico"));
        ui->tableWidget->setCellWidget(i,4,dele);
        dele->setProperty("row",i);
        QObject::connect(dele,SIGNAL(clicked(bool)),this,
                         SLOT(deleteClicked(bool)));
    }
    qDebug()<<"Adding empty line";

    QTableWidgetItem* SLNO = new QTableWidgetItem;
    SLNO->setText(QString::number(i+1));
    ui->tableWidget->setItem(i,0,SLNO);


    GMItemSearchLite *search = new GMItemSearchLite(itemsmodel);
    QToolButton *buttonlay = new QToolButton(this);
    QHBoxLayout *lay = new QHBoxLayout(buttonlay);
    lay->setContentsMargins(0,0,0,0);
    lay->addWidget(search);
    ui->tableWidget->setCellWidget(i,1,buttonlay);
    search->setProperty("row",i);
    QObject::connect(search,SIGNAL(SelectedItemID(inventoryItemDataModel)),this,
                     SLOT(ItemSelected(inventoryItemDataModel)));
    qDebug()<<"Adding empty line 110 ";

    QLineEdit* reqQty = new QLineEdit;
    reqQty->setAlignment(Qt::AlignRight);
    ui->tableWidget->setCellWidget(i,2,reqQty);
    reqQty->setProperty("row",i);
    QObject::connect(reqQty,SIGNAL(editingFinished()),this,
                     SLOT(reqQtyEntered()));
    qDebug()<<"Adding empty line 118";


    QLineEdit* issQty = new QLineEdit;
    issQty->setAlignment(Qt::AlignRight);
    ui->tableWidget->setCellWidget(i,3,issQty);
    issQty->setProperty("row",i);
    QObject::connect(issQty,SIGNAL(editingFinished()),this,
                     SLOT(issQtyEntered()));
    qDebug()<<"Adding empty line";



}

void StockRequestForm::showVoucher(GeneralVoucherDataObject *obj1)
{
    obj = obj1;

    qDebug()<<"Voucher NUmber = "<<obj->voucherNumber<<obj->VoucherDate;
    vNo = obj->voucherNumber;
    ui->voucherDateEdit->setDate(obj->VoucherDate);
    ui->narrationTextEdit->setPlainText(obj->narration);
    UserProfileDatabaseHelper userhelper;
    ui->addedByName->setText(userhelper.getEmployeeNameFromID(obj->AddedById));
    ui->reference->setText(obj->reference);
    setTable();

}

void StockRequestForm::ItemSelected(inventoryItemDataModel item)
{
    int row = sender()->property("row").toInt();
    CompoundItemDataObject comp;
    comp.BaseItem = item;
    appendItem(comp,row);
    setTable();

    qobject_cast< QLineEdit*>(ui->tableWidget->cellWidget(row,2))->setFocus();
    qobject_cast< QLineEdit*>(ui->tableWidget->cellWidget(row,2))->selectAll();
    qobject_cast< QLineEdit*>(ui->tableWidget->cellWidget(row,2))->clear();
    qobject_cast< QLineEdit*>(ui->tableWidget->cellWidget(row,3))->clear();
    return;

}

void StockRequestForm::deleteClicked(bool)
{
    int x= sender()->property("row").toInt();

    obj->InventoryItems.removeAt(x);

    setTable();
}

void StockRequestForm::reqQtyEntered()
{
    int x= sender()->property("row").toInt();
    obj->InventoryItems[x].BaseItem.drQty =  qobject_cast< QLineEdit*>(ui->tableWidget->cellWidget(x,2))->text().toFloat();

}

void StockRequestForm::issQtyEntered()
{
    int x= sender()->property("row").toInt();
    obj->InventoryItems[x].BaseItem.crQty =  qobject_cast< QLineEdit*>(ui->tableWidget->cellWidget(x,3))->text().toFloat();

}

void StockRequestForm::appendItem(CompoundItemDataObject item, int row)
{
    qDebug()<<"524 row ="<<row;
    QLineEdit* line = qobject_cast<QLineEdit*> (ui->tableWidget->cellWidget(row,2));
    QLineEdit* line1 = qobject_cast<QLineEdit*> (ui->tableWidget->cellWidget(row,3));

    item.BaseItem.drQty = line->text().toFloat();
    item.BaseItem.crQty = line1->text().toFloat();

    if(obj->InventoryItems.size() == row ){
        obj->InventoryItems.append(item);
        qDebug()<<"item appended";
    }
    else{
        obj->InventoryItems[row]=item;
        qDebug()<<"item Inserted at "<<row;
    }
}

void StockRequestForm::on_buttonBox_accepted()
{
    obj->narration = ui->narrationTextEdit->toPlainText();
    obj->reference = ui->reference->text();
    obj->VoucherDate = ui->voucherDateEdit->date();
    if(vNo.length()>0){
        dbHelper->updateStockRequest(obj,vNo);
    }
    else{
        obj->AddedById = LoginValues::getUserID();
        qDebug()<<"insertin new ID";
        dbHelper->insertStockRequest(obj);
    }
    this->close();
    emit closing();
}

void StockRequestForm::on_buttonBox_rejected()
{
    this->close();
}
