#ifndef CASHBANKBOOK_H
#define CASHBANKBOOK_H

#include <QWidget>

#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgersearchlite.h"
#include "database/Reports/ledgerhelper.h"

namespace Ui {
class CashBankBook;
}

class CashBankBook : public QWidget
{
    Q_OBJECT

public:
    explicit CashBankBook(QWidget *parent = 0);
    ~CashBankBook();


    GMLedgerLineMaterial *wid;
    LedgerMasterDataModel led;
    QSqlQueryModel *model;

    LedgerHelper *dbhelp;

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void setTableView();
private:
    Ui::CashBankBook *ui;
};

#endif // CASHBANKBOOK_H
