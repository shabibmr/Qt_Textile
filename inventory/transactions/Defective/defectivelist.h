#ifndef DEFECTIVELIST_H
#define DEFECTIVELIST_H

#include <QWidget>

#include "datamodels/Counter/countersettingsdatamodel.h"
#include <QSqlQueryModel>
#include <database/Reports/inventoryhelper.h>
#include <QToolButton>
#include "customwidgets/test/gmitemsortfilterproxymodel.h"

#include "wastageentryeditor.h"

namespace Ui {
class DefectiveList;
}

class DefectiveList : public QWidget
{
    Q_OBJECT

public:
    explicit DefectiveList(bool hideCreate=false,QWidget *parent = nullptr);
    ~DefectiveList();

    WastageEntryEditor *editor;

    QList<GeneralVoucherDataObject> voucherList;
    DefectiveDatabaseHelper *dbHelper;
    QSqlQueryModel *model;
    GMItemSortFilterProxyModel *sort;


private slots:

    void on_okButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_createNew_clicked();

//    void on_tableWidget_doubleClicked(const QModelIndex &index);

public slots:

    void setTableView();

private:
    Ui::DefectiveList *ui;
};

#endif // DEFECTIVELIST_H
