#ifndef SALESITEMSGRID_H
#define SALESITEMSGRID_H

#include <QObject>
#include <QWidget>
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include <QTableView>
#include <customwidgets/Models/possalesitemsmodel.h>

#include <QVBoxLayout>

class SalesItemsGrid : public QWidget
{
    Q_OBJECT
public:
    explicit SalesItemsGrid(SalesInventoryItemDatabaseHelper *dbHelper,
                            QRect rect,
                            int colCount=6,
                            int buttonHeight=40,
                            int buttonWidth=130,
                            QWidget *parent=nullptr);
    ~SalesItemsGrid();

    SalesInventoryItemDatabaseHelper *dbHelper;
    QString grpID;

    int colCount;
    int buttonWidth;
    int buttonHeight;

    QRect rect;

    QTableView *table;
    QSqlQueryModel *model;
    POSSalesItemsModel *tModel;

    QVBoxLayout *lay;

    void setWidgets();


    void setGrpID(const QString &value);

signals:
    void selectSalesItemID(QString);
};

#endif // SALESITEMSGRID_H
