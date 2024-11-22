#include "pricelistbyitem.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include "customwidgets/Validator/myvalidator.h"

PriceListByItem::PriceListByItem(QString itemID, SalesInventoryItemDatabaseHelper *itemsHelper,QWidget *parent) : QWidget(parent)
{
    this->itemID = itemID;
    this->itemsHelper = itemsHelper;

    table = new QTableWidget;
    priceHelper = new PricelistDatabaseHelper();
    table->setColumnCount(3);
    setTable();
    QVBoxLayout *lay =new QVBoxLayout(this);
    lay->addWidget(table);
    setLayout(lay);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    QTableWidgetItem* Item = new QTableWidgetItem;
    Item->setText("Price List Name");
    table->setHorizontalHeaderItem(0,Item);

    QTableWidgetItem* Item2 = new QTableWidgetItem;
    Item2->setText("Base Rate");
    table->setHorizontalHeaderItem(1,Item2);

    QTableWidgetItem* Item3 = new QTableWidgetItem;
    Item3->setText("Net Rate");
    table->setHorizontalHeaderItem(2,Item3);


}


PriceListByItem::~PriceListByItem()
{

}

void PriceListByItem::setTable()
{
    qDebug()<<"Set Table at price lsit";
    //    if(itemID.length()>0)
    QMap<QString,float> mapPrices = priceHelper->getAllPricesOfItemAsMap(itemID);
    //    model = priceHelper->getAllPricesOfItem(itemID);
    QMap<QString,QString> map = priceHelper->getPriceListAsMap();
    QStringList keys = map.keys();
    table->setRowCount(keys.size());

    for(int i =0;i<keys.size();i++){
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(map[keys[i]]);
        QLineEdit *line = new QLineEdit();
        QLineEdit *netRate = new QLineEdit();
        line->setValidator(new MyValidator(0,99999999,2,this));
        netRate->setValidator(new MyValidator(0,99999999,2,this));
        line->setProperty("id",keys[i]);
        line->setAlignment(Qt::AlignRight);
        qDebug()<<"price id "<<keys[i]<<"Price : " <<mapPrices.value(keys[i]);
        if(mapPrices.contains(keys[i]))
            line->setText(QString::number(mapPrices.value(keys[i]),'f',2));
        line->setFrame(QFrame::NoFrame);
        netRate->setFrame(QFrame::NoFrame);
        table->setItem(i,0,item);
        table->setCellWidget(i,1,line);
        table->setCellWidget(i,2,netRate);

        QObject::connect(line,&QLineEdit::editingFinished,this,[=](){
            netRate->blockSignals(true);
            float val = line->text().toFloat();
            float tax = itemsHelper->getTaxRate(itemID);
            val = val + val *tax/100;
            netRate->setText(QString::number(val,'f',2));
            netRate->blockSignals(false);

        });
        QObject::connect(netRate,&QLineEdit::editingFinished,this,[=](){
            line->blockSignals(true);
            float val = netRate->text().toFloat();
            float tax = itemsHelper->getTaxRate(itemID);
            val = val * 100 / (100+tax);
            line->setText(QString::number(val,'f',2));
            line->blockSignals(false);

        });

    }
}

void PriceListByItem::savePrices()
{
    //    priceHelper->startTransaction();

    priceHelper->deletePricesByItem(itemID);
    qDebug()<<"Saving Prices";
    for(int i =0;i<table->rowCount();i++){
        QLineEdit* line = qobject_cast <QLineEdit*> (table->cellWidget(i,1));
        QString val = line->text();
        QString priceID = line->property("id").toString();
        if( val.length() >0 ){
            float price = val.toFloat();
            priceHelper->insertItemPrices(itemID,priceID,uomID, price);
        }
    }
}

void PriceListByItem::setItemID(QString value, QString uomID)
{
    itemID = value;
    this->uomID = uomID;

}
