#ifndef WORKORDERLIST_H
#define WORKORDERLIST_H

#include <QWidget>

#include "workorderquotes.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include "database/inventory/workorder/workorderdatabasehelper.h"
#include "workordereditor.h"
namespace Ui {
class WorkOrderList;
}

class WorkOrderList : public QWidget
{
    Q_OBJECT

public:
    explicit WorkOrderList(QWidget *parent = 0);
    ~WorkOrderList();
	
    WorkOrderDatabaseHelper *dbHelper;
    QSqlQueryModel *model;
    GMItemSortFilterProxyModel *sort;

    WorkOrderEditor* editor;


private slots:
//    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void setTableView();
    void on_okButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_createNew_clicked();

private:
    Ui::WorkOrderList *ui;
};

#endif // WORKORDERLIST_H
