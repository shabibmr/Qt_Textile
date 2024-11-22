#ifndef STOCKREPORT_H
#define STOCKREPORT_H

#include <QWidget>
#include <database/Reports/inventoryhelper.h>
#include "customwidgets/test/gmitemsortfilterproxymodel.h"

namespace Ui {
class StockReport;
}

class StockReport : public QWidget
{
    Q_OBJECT

public:
    explicit StockReport(QWidget *parent = 0);
    ~StockReport();
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
    Ui::StockReport *ui;
};

#endif // STOCKREPORT_H
