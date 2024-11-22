#ifndef RECEIPTENTRY_H
#define RECEIPTENTRY_H

#include <QWidget>
#include <QDate>
#include <QKeyEvent>
#include "receiptvouchereditor.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include "datamodels/Counter/countersettingsdatamodel.h"


namespace Ui {
class ReceiptEntry;
}

class ReceiptEntry : public QWidget
{
    Q_OBJECT

public:
    explicit ReceiptEntry(QWidget *parent = 0);
    ~ReceiptEntry();
    ReceiptVoucherEditor* editor;
    QList<GeneralVoucherDataObject> voucherList;
    ReceiptVoucherDatabaseHelper *dbhelper;

    GMItemSortFilterProxyModel* sort;

    QSqlQueryModel *model;

public slots:
    void setHeading(QString label);

private slots:
    void on_createNew_clicked();
    void keyPressEvent(QKeyEvent *e);
    void setTableView();
    void on_okPushButton_clicked();
    void delButtonClicked(bool);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ReceiptEntry *ui;
};

#endif // RECEIPTENTRY_H
