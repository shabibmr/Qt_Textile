#ifndef ITEEMWISESALESORDERLIST_H
#define ITEEMWISESALESORDERLIST_H

#include <QWidget>
#include "additemwisesalesorder.h"
#include "customwidgets/Models/gmsqlquerymodel.h"

namespace Ui {
class IteemwiseSalesOrderList;
}

class IteemwiseSalesOrderList : public QWidget
{
    Q_OBJECT

public:

    explicit IteemwiseSalesOrderList(QString voucherItemwiseType,
                                     QString voucherType,
                                     VoucherDBAbstract *vHelper,
                                     QString tableName,
                                     QString mainTable,
                                     QString detailTable,
                                     QWidget *parent = nullptr
            );

    ~IteemwiseSalesOrderList();

    ItemwiseSalesOrderDatabaseHelper *dbHelper;

    QSqlQueryModel *model;

    GMSQLQUeryModel *M;

    VoucherDBAbstract *voucherDbHelper;

    QString title;

    QString voucherType;
    QString voucherItemwise;
    void setTitle(const QString &value);

    void setTableName(QString value);

public slots:
    void setData();

private slots:
    void on_createNewPushButton_clicked();

    void on_okPushButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::IteemwiseSalesOrderList *ui;
};

#endif // ITEEMWISESALESORDERLIST_H
