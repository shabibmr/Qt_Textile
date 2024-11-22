#ifndef SALESORDERS_H
#define SALESORDERS_H

#include <QWidget>
#include <QDate>
#include <QKeyEvent>

#include "salesordervouchereditor.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"

#include "customwidgets/TableViews/reportstableview.h"
#include "customwidgets/godownselectorwidget.h"

#include "projects/WorkOrder/workordereditor.h"

namespace Ui {
class salesOrders;
}

class salesOrders : public QWidget
{
    Q_OBJECT
public:
    explicit salesOrders(bool hideCreate=false,QWidget *parent = 0);
    ~salesOrders();
    SalesOrderVoucherEditor* AddSalesOrderWidget;
    QList<GeneralVoucherDataObject> voucherList;
    SalesOrderDatabaseHelper *dbhelper;
    QSqlQueryModel *model;
    GMItemSortFilterProxyModel *sort;

    ReportsTableView *tableView;


    QDateTime fromDateTime;
    QDateTime toDateTime;
    QString exportSection;
//    QPointer<GMDateEdit> fromDate;
//    QPointer<GMDateEdit> toDate;


public slots:
    void setHeading(QString label);
    void setTableView();
private slots:

    void on_createNewPushButton_clicked();
    void keyPressEvent(QKeyEvent *e);
    void on_okPushButton_clicked();
    void delButtonClicked(bool);
    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_generateWorkOrder_clicked();

    void exportToWorkOrder(QString section);
    void openWorkorder(GeneralVoucherDataObject *voucher);

private:
    Ui::salesOrders *ui;
};

#endif // SALESORDERS_H
