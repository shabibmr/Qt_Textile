#ifndef PRODUCTIONRATEREPORT_H
#define PRODUCTIONRATEREPORT_H

#include <QWidget>
#include <database/Reports/inventoryhelper.h>
#include "customwidgets/test/gmitemsortfilterproxymodel.h"

namespace Ui {
class ProductionRateReport;
}

class ProductionRateReport : public QWidget
{
    Q_OBJECT

public:
    explicit ProductionRateReport(QWidget *parent = nullptr);
    ~ProductionRateReport();
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
    Ui::ProductionRateReport *ui;
};

#endif // PRODUCTIONRATEREPORT_H
