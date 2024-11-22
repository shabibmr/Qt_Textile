#ifndef STOCKENTRY_H
#define STOCKENTRY_H

#include <QWidget>
#include <QDate>
#include <QKeyEvent>
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include "stockentryeditor.h"
#include "datamodels/Counter/countersettingsdatamodel.h"

namespace Ui {
class StockEntry;
}

class StockEntry : public QWidget
{
    Q_OBJECT

public:
    explicit StockEntry(QWidget *parent = 0);
    ~StockEntry();
    StockEntryEditor *addStockEntryWidget;
    QList<GeneralVoucherDataObject> voucherList;
    StockEntryDatabaseHelper *dbhelper;
    GMItemSortFilterProxyModel* sort;

    QSqlQueryModel *model;

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
    Ui::StockEntry *ui;
};

#endif // STOCKENTRY_H
