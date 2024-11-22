#ifndef SALESITEMS_H
#define SALESITEMS_H

#include <QWidget>
#include <QSignalMapper>
#include <QScrollArea>
#include <QScrollBar>
#include <QLabel>
#include <QGridLayout>

#include "customwidgets/qtbutton.h"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "datamodels/inventory/sales_inventory_item/inventoryitemdatamodel.h"
#include "salesgroups.h"

namespace Ui {
class SalesItems;
}

class SalesItems : public QWidget
{
    Q_OBJECT

public:
    explicit SalesItems(int colCount=6,int buttonHeight=40,int buttonWidth=130,QWidget *parent = 0);
    ~SalesItems();

    SalesInventoryItemDatabaseHelper *dbHelper;
    QString grpID;

    QList<QSharedPointer<inventoryItemDataModel>> items;
    QPointer<QGridLayout> gLayout;
    void setWidgets();
    void setTable(int lang =0);
    int colCount;

    int buttonWidth;
    int buttonHeight;

public slots:
    void salesItemSelected(QString itemID);

    void groupButtonFavouriteClicked();

    void clearItems();

signals:
    void selectSalesItemID(QString);

private:
    Ui::SalesItems *ui;
};

#endif // SALESITEMS_H
