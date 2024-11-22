#ifndef STOCKJOURNALS_H
#define STOCKJOURNALS_H

#include <QWidget>
#include <QDate>
#include <QKeyEvent>
#include "stockjournaleditor.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include "database/inventory/stockjournal/stockjournaldatabasehelper.h"
namespace Ui {
class stockJournals;
}

class stockJournals : public QWidget
{
    Q_OBJECT

public:
    explicit stockJournals(QWidget *parent = 0);
    ~stockJournals();
    StockJournalEditor *AddStockJournalWidget;
    QList<GeneralVoucherDataObject> voucherList;
    StockJournalDatabaseHelper *dbhelper;
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
    Ui::stockJournals *ui;
};

#endif // STOCKJOURNALS_H
