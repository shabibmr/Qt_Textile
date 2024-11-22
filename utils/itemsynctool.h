#ifndef ITEMSYNCTOOL_H
#define ITEMSYNCTOOL_H

#include <QWidget>
#include <QMessageBox>
#include <QSqlRecord>

#include "customwidgets/Company/companyselectorwidget.h"
#include "inventory/masters/sales_inventory_items/additems.h"

#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "utils/duplicateitemsdialog.h"

#include "customwidgets/Material/components/qtmaterialflatbutton.h"


namespace Ui {
class ItemSyncTool;
}

class ItemSyncTool : public QWidget
{
    Q_OBJECT

public:
    explicit ItemSyncTool(QWidget *parent = nullptr);
    ~ItemSyncTool();

    SalesInventoryItemDatabaseHelper *itemHelper;
    QList<inventoryItemDataModel *> missingItems;
    QMap<int, QList<QPair<QString, QString> > > dupsList;

    QString currentDb = DatabaseValues::dbName;
    QString refDB;

public slots:

    bool selectCurrentDb();
    bool selectRefdb();
    void syncMissingItems();
    void syncItemIdsWithDiffNames();
    void removeDuplicates();

    void syncItemsWithSameName();

    void selectOption1();
    void selectOption2();
    void selectOption3();
    void selectOption4();

    void showMessage(QString text);
    void showItemEditor();
    void showDuplicateDialog();



private:
    void setWidgets();
    Ui::ItemSyncTool *ui;
};

#endif // ITEMSYNCTOOL_H
