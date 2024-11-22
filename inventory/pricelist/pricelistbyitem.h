#ifndef PRICELISTBYITEM_H
#define PRICELISTBYITEM_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include "database/inventory/pricelist/pricelistdatabasehelper.h"

class PriceListByItem : public QWidget
{
    Q_OBJECT
public:
    explicit PriceListByItem(QString itemID,SalesInventoryItemDatabaseHelper *itemsHelper,QWidget *parent = nullptr);
    ~PriceListByItem();

    QTableWidget *table;

    PricelistDatabaseHelper *priceHelper;

    void setTable();

    void savePrices();

    void setItemID(QString value, QString uomID);

    QSqlQueryModel *model;

signals:

private:

    QString itemID;
    QString uomID;
    float taxRate=0;
    SalesInventoryItemDatabaseHelper *itemsHelper;
};

#endif // PRICELISTBYITEM_H
