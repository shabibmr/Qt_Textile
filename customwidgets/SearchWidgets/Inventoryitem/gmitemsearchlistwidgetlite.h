#ifndef GMITEMSEARCHLISTWIDGETLITE_H
#define GMITEMSEARCHLISTWIDGETLITE_H

#include <QWidget>
#include <QSqlQueryModel>
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "gmitemsearchproxymodel.h"
#include "itemsearchdelegate.h"
#include <QEvent>
#include <QKeyEvent>
#include <QPointer>
#include <QFocusEvent>
#include <QSqlRecord>

namespace Ui {
class GMItemSearchListWidgetLite;
}

class GMItemSearchListWidgetLite : public QWidget
{
    Q_OBJECT

public:
    explicit GMItemSearchListWidgetLite(QSqlQueryModel *model,QWidget *parent = 0);
    ~GMItemSearchListWidgetLite();

    SalesInventoryItemDatabaseHelper itemHelper;
    GMItemSearchProxyModel *sort;


    QSqlQueryModel *model;

    QStringList itemNames;
    int lLoc =0;

    void setFocusExpl(QString text);
private:
    Ui::GMItemSearchListWidgetLite *ui;
protected:
    bool eventFilter(QObject* obj, QEvent *event);

public slots:
    void filterData(QString);

    void setData(QSqlQueryModel* model);

    void checkData();

private slots:

    void onTablewidgetEnterpressed(QModelIndex i);

    void on_tableWidget_clicked(const QModelIndex &index);

signals:

    void selectedID(QString,QString);

    void selectedItembyBarcode(inventoryItemDataModel);

//    void selectedMultipleItembyBarcode(QList<inventoryItemDataModel>);


    void SOSelectedSig(QString);

};

#endif // GMITEMSEARCHLISTWIDGETLITE_H
