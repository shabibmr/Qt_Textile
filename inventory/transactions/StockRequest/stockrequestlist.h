#ifndef STOCKREQUESTLIST_H
#define STOCKREQUESTLIST_H

#include <QWidget>
#include <datamodels/Counter/countersettingsdatamodel.h>
#include <inventory/transactions/StockRequest/stockrequestform.h>
#include <QSqlRecord>
#include "stockrequesteditor.h"

namespace Ui {
class StockRequestList;
}

class StockRequestList : public QWidget
{
    Q_OBJECT

public:
    explicit StockRequestList(QWidget *parent = 0);
    ~StockRequestList();

    StockRequestDatabaseHelper dbHelper;
    QSqlQueryModel *model;
    GeneralVoucherDataObject *obj;
    StockRequestEditor *editor;

private slots:
    void on_createNew_clicked();

    void on_OKButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void  setTable();


private:
    Ui::StockRequestList *ui;
};

#endif // STOCKREQUESTLIST_H
