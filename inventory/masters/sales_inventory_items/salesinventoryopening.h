#ifndef SALESINVENTORYOPENING_H
#define SALESINVENTORYOPENING_H

#include <QWidget>
#include <database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h>


namespace Ui {
class SalesInventoryOpening;
}

class SalesInventoryOpening : public QWidget
{
    Q_OBJECT

public:
    explicit SalesInventoryOpening(QWidget *parent = 0);
    ~SalesInventoryOpening();
    SalesInventoryItemDatabaseHelper *dbHelper;

    QList<QSharedPointer<inventoryItemDataModel >> itemsList;

    QString text;

    void setTable();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void itemQtyEntered();

    void itemRateEntered();

    void itemValueEntered();


   void on_lineEdit_returnPressed();

private:
    Ui::SalesInventoryOpening *ui;
};

#endif // SALESINVENTORYOPENING_H
