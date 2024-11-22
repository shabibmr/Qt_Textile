#ifndef PAYMENT_H
#define PAYMENT_H

#include <QWidget>
#include <QDate>
#include <QKeyEvent>
#include "paymentvouchereditor.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
namespace Ui {
class Payment;
}

class Payment : public QWidget
{
    Q_OBJECT

public:
    explicit Payment(QWidget *parent = 0);
    ~Payment();
    PaymentVoucherEditor *editor;
    QList<GeneralVoucherDataObject> voucherList;
    PaymentVoucherDatabaseHelper *dbhelper;

    GMItemSortFilterProxyModel* sort;

    QSqlQueryModel *model;

public slots:
    void setHeading(QString label);

private slots:
    void on_createNewPushButton_clicked();
    void keyPressEvent(QKeyEvent *e);
    void setTableView();

    void on_okPushButton_clicked();
    void delButtonClicked(bool);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_printPushButton_clicked();

private:
    Ui::Payment *ui;
};

#endif // PAYMENT_H
