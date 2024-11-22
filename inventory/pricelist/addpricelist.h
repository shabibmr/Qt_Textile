#ifndef ADDPRICELIST_H
#define ADDPRICELIST_H

#include <QWidget>
#include "database/inventory/pricelist/pricelistdatabasehelper.h"
#include "datamodels/inventory/pricelist/pricelistdataobject.h"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "pricelistmodel.h"
#include "pricelisttableview.h"
#include "pricelistdelegate.h"
#include "pricelistsortmodel.h"

namespace Ui {
class addPriceList;
}

class addPriceList : public QWidget
{
    Q_OBJECT

public:
    explicit addPriceList(QWidget *parent = Q_NULLPTR);
    ~addPriceList();
    int id=0;
    PricelistDatabaseHelper *dbHelper;
    SalesInventoryItemDatabaseHelper *itemsHelper;
    UomDataBaseHelper *uomHelper;

    PriceListSortModel *sort;

    QStringList lis;
    PriceListTableView *DataTableWidget;
    QPointer<PriceListModel> model;
    void setWidgets();

    PriceListDataObject *priceList;
    void setId(int value);
    void setItemPrice(QString itemId, float price);
    void savePriceList();
    void disableSave();

private slots:


    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_basedOnDate_toggled(bool checked);

    void on_searchLine_textChanged(const QString &arg1);

public slots:

    void edit(PriceListDataObject* obj);
    void setTitle(QString text);
    void setPriceName(QString name);

private:
    Ui::addPriceList *ui;
signals:
    void closing();
};


#endif // ADDPRICELIST_H
