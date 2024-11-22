#ifndef JOURNAL_H
#define JOURNAL_H

#include <QWidget>
#include <QKeyEvent>
#include <QDate>

#include "journalvouchereditor.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include "datamodels/Counter/countersettingsdatamodel.h"

namespace Ui {
class journal;
}

class journal : public QWidget
{
    Q_OBJECT

public:
    explicit journal(QWidget *parent = 0);
    ~journal();
    JournalVoucherEditor* addJournalEntryWidget;
    QList<GeneralVoucherDataObject> voucherList;
    JournalVoucherDatabaseHelper *dbhelper;
    QSqlQueryModel *model;
    GMItemSortFilterProxyModel *sort;

public slots:
    void setHeading(QString label);
private slots:
    void on_createNew_clicked();
    void keyPressEvent(QKeyEvent *e);
    void setTableView();
//    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void on_okPushButton_clicked();
    void delButtonClicked(bool);

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::journal *ui;
};

#endif // JOURNAL_H
