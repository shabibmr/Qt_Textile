#ifndef ITEMTRASACTIONLIST_H
#define ITEMTRASACTIONLIST_H

#include <QDialog>
#include <QSqlQueryModel>
#include <database/Reports/inventoryhelper.h>
#include "datamodels/inventory/sales_inventory_item/inventoryitemdatamodel.h"
#include "customwidgets/TableViews/reportstableview.h"
#include "customwidgets/Delegates/reportcommondelegate.h"

namespace Ui {
class ItemTrasactionList;
}

class ItemTrasactionList : public QDialog
{
    Q_OBJECT

public:
    explicit ItemTrasactionList(QString itemID,
                                SalesInventoryItemDatabaseHelper *itemHelper,
                                InventoryHelper *invHelper,
                                QWidget *parent = nullptr);
    ~ItemTrasactionList();
    QSqlQueryModel *model;

    QString godownID="";
    QString voucherType="";

    ReportsTableView *tableView;

    inventoryItemDataModel *item;

    InventoryHelper *invHelper;
    SalesInventoryItemDatabaseHelper *itemHelper;



    void setValue();

public slots:
    void setDateFilters(QDate fromDate,QDate toDate=QDate::currentDate());
    void setVoucherTypeFilter(QString voucherType);
    void setGodownFilter(QString godownID);

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::ItemTrasactionList *ui;
};

#endif // ITEMTRASACTIONLIST_H
