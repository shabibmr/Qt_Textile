#ifndef BELOWREORDERLEVEL_H
#define BELOWREORDERLEVEL_H

#include <QWidget>
#include <database/Reports/inventoryhelper.h>
#include "inventory/transactions/purchaseorders/purchaseordervouchereditor.h"
#include "counter/PrinterHelper/counterprinterhelper.h"

namespace Ui {
class BelowReorderLevel;
}

class ReOrderLevel : public QWidget
{
    Q_OBJECT

public:
    explicit ReOrderLevel(QWidget *parent = 0);
    ~ReOrderLevel();
    QSqlQueryModel *model;
    InventoryHelper *itemReportHelper;
    SalesInventoryItemDatabaseHelper *itemsHelper;

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

    void setTable();

    void on_exportButton_clicked();

private:
    Ui::BelowReorderLevel *ui;
};

#endif // BELOWREORDERLEVEL_H
