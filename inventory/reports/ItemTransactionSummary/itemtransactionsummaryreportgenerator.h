#ifndef ITEMTRANSACTIONSUMMARYREPORTGENERATOR_H
#define ITEMTRANSACTIONSUMMARYREPORTGENERATOR_H

#include <QWidget>
#include "database/Reports/inventoryhelper.h"

namespace Ui {
class ItemTransactionSummaryReportGenerator;
}

class ItemTransactionSummaryReportGenerator : public QWidget
{
    Q_OBJECT

public:
    explicit ItemTransactionSummaryReportGenerator(QWidget *parent = nullptr);
    ~ItemTransactionSummaryReportGenerator();

    void setReportName(const QString &value);

    void setVoucherType(int value);

public slots:
    void setTableView();

    void sendEmail();
    void printReport();
    void exportToExcel();

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_OKButton_clicked();

private:

    QString ReportName;
    QSqlQueryModel *model;
    InventoryHelper *invHelper;
    int voucherType;

    Ui::ItemTransactionSummaryReportGenerator *ui;
};

#endif // ITEMTRANSACTIONSUMMARYREPORTGENERATOR_H
