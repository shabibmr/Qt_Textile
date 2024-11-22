#ifndef LEDGERMASTER_H
#define LEDGERMASTER_H

#include <QWidget>
#include "finance/masters/ledgers/addledger.h"
#include <QKeyEvent>
#include "customwidgets/test/gmitemsortfilterproxymodel.h"

namespace Ui {
class ledgerMaster;
}

class ledgerMaster : public QWidget
{
    Q_OBJECT

public:
    explicit ledgerMaster(QWidget *parent = 0);
    ~ledgerMaster();
    addLedger* addLedgerWidget;
    LedgerMasterDatabaseHelper* dbHelper;
    QList<LedgerMasterDataModel> ledgerList;
    QString x;
    QSqlQueryModel* ledModel;
    GMItemSortFilterProxyModel *sort;
private slots:
    void on_createNewPushButton_clicked();
//    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void keyPressEvent(QKeyEvent *e);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_searchLine_textChanged(const QString &arg1);

public slots:
//    void settable();
    void setTableView();
    void searchTextChanged(QString x);


private:
    Ui::ledgerMaster *ui;
};

#endif // LEDGERMASTER_H
