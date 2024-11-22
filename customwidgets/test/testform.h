#ifndef TESTFORM_H
#define TESTFORM_H

#include <QWidget>
#include <QSqlQueryModel>
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "gmitemsortfilterproxymodel.h"
#include "ledgerreportdelegate.h"

#include "customwidgets/SearchWidgets/Inventoryitem/gmitemsearchlite.h"

namespace Ui {
class TestForm;
}

class TestForm : public QWidget
{
    Q_OBJECT

public:
    explicit TestForm(QWidget *parent = 0);
    ~TestForm();

    QSqlQueryModel *sqlModel;
    GMItemSortFilterProxyModel* sort;
    int KeyCol=0;

    SalesInventoryItemDatabaseHelper *dbhelper ;
private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::TestForm *ui;
};

#endif // TESTFORM_H
