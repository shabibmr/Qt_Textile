#ifndef INVENTORYGROUP_H
#define INVENTORYGROUP_H

#include <QWidget>
#include <QKeyEvent>
#include <QToolButton>
#include <QSqlRecord>

#include "inventory/masters/sales_inventory_groups/additemgroups.h"
#include "database/inventory/sales_inventory_groups/salesinventorygroupsdatabasehelper.h"
#include "datamodels/inventory/sales_inventory_group/inventoryitemgroupdatamodel.h"

namespace Ui {
class inventorygroup;
}

class inventorygroup : public QWidget
{
    Q_OBJECT

public:
    explicit inventorygroup(QWidget *parent = 0);
    ~inventorygroup();
    AddItemGroups *addGroupWidget;
    SalesInventoryGroupsDatabaseHelper *dbhelper;
    QList<InventoryItemGroupDataModel> groupList;
    QSqlQueryModel* invGroupModel;

    QString x;
private slots:
    void on_createNewPushButton_clicked();
//    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void keyPressEvent(QKeyEvent *e);
     void delButtonClicked(bool v);

     void on_tableView_doubleClicked(const QModelIndex &index);

public slots:
//    void settable();
    void setTableView();
    void searchTextChanged(QString);
private:
    Ui::inventorygroup *ui;


};

#endif // INVENTORYGROUP_H
