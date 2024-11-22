#ifndef PROFITANDLOSSREPORT_H
#define PROFITANDLOSSREPORT_H

#include <QWidget>
#include <QStandardItemModel>
#include <QInputDialog>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "database/Reports/ledgerhelper.h"
#include "database/Reports/inventoryhelper.h"
#include "database/Reports/accountingreportdatabasehelper.h"

namespace Ui {
class ProfitAndLossReport;
}

class ProfitAndLossReport : public QWidget
{
    Q_OBJECT

public:
    AccountingReportDatabaseHelper *ledHelper;
    AccountGroupMasterDatabaseHelper *grpHelper;
    explicit ProfitAndLossReport(QWidget *parent = 0);
    ~ProfitAndLossReport();

    QSqlQueryModel* modelIncome;
    QSqlQueryModel* modelExpense;

    int lrow =0;
    int rrow =0;

    float ltotal=0;
    float rtotal=0;

    float gp=0;
    float np=0;

    float openStkValue=0;
    float clsStkValue=0;


private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

//    void setTableView();

    void setTable();

    void addLTableEntry(QString item, QString value);
    void addRTableEntry(QString item, QString value);

    void on_leftTableWidget_doubleClicked(const QModelIndex &index);

    void on_rightTableWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::ProfitAndLossReport *ui;
};

#endif // PROFITANDLOSSREPORT_H
