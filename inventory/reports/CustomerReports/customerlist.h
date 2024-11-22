#ifndef CUSTOMERLIST_H
#define CUSTOMERLIST_H

#include <QWidget>
#include <database/Reports/inventoryhelper.h>
#include <customwidgets/test/gmitemsortfilterproxymodel.h>
#include "pocwisesalessummary.h"

namespace Ui {
class CustomerList;
}

class CustomerList : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerList(QWidget *parent = nullptr);
    ~CustomerList();

    void setTableView();
    InventoryHelper *inventoryReportsHelper;

    GMItemSortFilterProxyModel *sort;

    QSqlQueryModel *model;

    POCwiseSalesSummary *summaryWidget;

private slots:

    void on_OKButton_2_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);



    void on_searchLine_textChanged(const QString &arg1);

    void on_OKButton_clicked();

private:
    Ui::CustomerList *ui;
};

#endif // CUSTOMERLIST_H
