#ifndef GROUPWISEOUTSTANDING_H
#define GROUPWISEOUTSTANDING_H

#include <QWidget>
#include <QToolButton>
#include "customwidgets/SearchWidgets/Ledger/gmledgersearchlite.h"
#include "database/Reports/ledgerhelper.h"
#include "database/finance/accountmaster/accountgroupmasterdatabasehelper.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/excel/excelreporthelper.h"

#include "sharetools/sharetools.h"
#include "counter/PrinterHelper/counterprinterhelper.h"

namespace Ui {
class GroupwiseOutstanding;
}

class GroupwiseOutstanding : public QWidget
{
    Q_OBJECT

public:
    explicit GroupwiseOutstanding(QWidget *parent = 0);
    ~GroupwiseOutstanding();

    AccountGroupMasterDatabaseHelper* accHelper;
    LedgerHelper* ledHelper;
    QSqlQueryModel* groupsModel;
    AccountGroupDataModel group;

    QSqlQueryModel* model;
    QtMaterialIconButton *shareButton;

private slots:
    void on_pushButton_clicked();
    void setTableView();
    void setAccountDetails(AccountGroupDataModel);

    void showShareOptions();
    void printReport(QPrinter *printer);
    void on_excelButton_clicked();
private:
    Ui::GroupwiseOutstanding *ui;
};

#endif // GROUPWISEOUTSTANDING_H
