#ifndef GroupSummary_H
#define GroupSummary_H

#include <QWidget>
#include <QToolButton>
#include "customwidgets/SearchWidgets/Ledger/gmledgersearchlite.h"
#include "database/Reports/ledgerhelper.h"
#include "database/finance/accountmaster/accountgroupmasterdatabasehelper.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "customwidgets/TableViews/reportstableview.h"

namespace Ui {
class GroupSummary;
}

class GroupSummary : public QWidget
{
    Q_OBJECT

public:
    explicit GroupSummary(QWidget *parent = 0);
    ~GroupSummary();

    ReportsTableView *tableView;
    AccountGroupMasterDatabaseHelper* accHelper;
    LedgerHelper* ledHelper;
    QSqlQueryModel* groupsModel;
    AccountGroupDataModel group;

    QSqlQueryModel* model;

private slots:
    void on_pushButton_clicked();
    void setTableView();
    void setAccountDetails(AccountGroupDataModel);

    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

private:
    Ui::GroupSummary *ui;
};

#endif // GroupSummary_H
