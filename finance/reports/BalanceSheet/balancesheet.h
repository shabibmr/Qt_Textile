#ifndef BALANCESHEET_H
#define BALANCESHEET_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QtMath>
#include <QStandardItemModel>
#include "database/Reports/ledgerhelper.h"
#include "database/Reports/accountingreportdatabasehelper.h"

namespace Ui {
class BalanceSheet;
}

class BalanceSheet : public QWidget
{
    Q_OBJECT

public:
    explicit BalanceSheet(QWidget *parent = 0);
    ~BalanceSheet();

    AccountingReportDatabaseHelper* ledHelper;
    AccountGroupMasterDatabaseHelper* grpHelper;
    QSqlQueryModel* modelAssets;
    QSqlQueryModel* modelLiabilities;



private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

    void setTableView();
//    void setLiabilitiesTableView();

private:
    Ui::BalanceSheet *ui;
};


#endif // BALANCESHEET_H
