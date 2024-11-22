#ifndef PURCHASEENTRY_H
#define PURCHASEENTRY_H

#include <QWidget>
#include <QDate>
#include <QKeyEvent>

#include "purchasevouchereditor.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"

namespace Ui {
class PurchaseEntry;
}

class PurchaseEntry : public QWidget
{
    Q_OBJECT

public:
    explicit PurchaseEntry(QWidget *parent = 0);
    ~PurchaseEntry();
    PurchaseVoucherEditor* AddPurchaseEntryWidget;
    QList<GeneralVoucherDataObject> voucherList;
    PurchaseVoucherDatabaseHelper *dbhelper;
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
    Ui::PurchaseEntry *ui;
};

#endif // PURCHASEENTRY_H
