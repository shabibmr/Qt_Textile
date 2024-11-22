#ifndef TRIALBALANCE_H
#define TRIALBALANCE_H

#include <QtMath>
#include <QWidget>
#include <QStandardItemModel>
#include <QSqlRecord>
#include "database/Reports/ledgerhelper.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "customwidgets/test/ledgerreportdelegate.h"
#include "database/finance/accountmaster/accountgroupmasterdatabasehelper.h"
#include "database/Reports/accountingreportdatabasehelper.h"

namespace Ui {
class TrialBalance;
}

class TrialBalance : public QWidget
{
    Q_OBJECT

public:
    explicit TrialBalance(QWidget *parent = 0);
    ~TrialBalance();

    QSqlQueryModel *model;
    QStandardItemModel *tbModel;
    AccountingReportDatabaseHelper* ledhelp;
    QList<AccountGroupDataModel> accGroups;

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

    void setTable();

private:
    Ui::TrialBalance *ui;
};

#endif // TRIALBALANCE_H
