#ifndef CONSUMPTIONREPORT_H
#define CONSUMPTIONREPORT_H

#include <QWidget>
#include <database/Reports/inventoryhelper.h>
#include "customwidgets/test/gmitemsortfilterproxymodel.h"

namespace Ui {
class ConsumptionReport;
}

class ConsumptionReport : public QWidget
{
    Q_OBJECT

public:
    explicit ConsumptionReport(QWidget *parent = nullptr);
    ~ConsumptionReport();

    QSqlQueryModel *model;
    GMItemSortFilterProxyModel* sort;
    InventoryHelper* inventoryReportsHelper;
private slots:
    void setTable();
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();


private:
    Ui::ConsumptionReport *ui;
};

#endif // CONSUMPTIONREPORT_H
