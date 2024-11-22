#ifndef PURCHASEORDERS_H
#define PURCHASEORDERS_H

#include <QWidget>
#include <QDate>
#include <QKeyEvent>

#include "purchaseordervouchereditor.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"

namespace Ui {
class purchaseOrders;
}

class purchaseOrders : public QWidget
{
    Q_OBJECT

public:
    explicit purchaseOrders(bool hideCreate=false,QWidget *parent = 0);
    ~purchaseOrders();

    PurchaseOrderVoucherEditor *AddPurchaseOrderWidget;
    QList<GeneralVoucherDataObject> voucherList;
    PurchaseOrderDatabaseHelper *dbhelper;
    QSqlQueryModel *model;
    GMItemSortFilterProxyModel *sort;

public slots:
    void setHeading(QString label);

private slots:
    void on_createNewPushButton_clicked();
    void keyPressEvent(QKeyEvent *e);
    void setTableView();
//    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void on_okPushButton_clicked();
    void delButtonClicked(bool);

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::purchaseOrders *ui;
};

#endif // PURCHASEORDERS_H
