#ifndef NEGATIVESTOCK_H
#define NEGATIVESTOCK_H

#include <QWidget>
#include "database/Reports/inventoryhelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "sharetools/excel/excelreporthelper.h"
#include "customwidgets/Delegates/reportcommondelegate.h"


namespace Ui {
class NegativeStock;
}

class NegativeStock : public QWidget
{
    Q_OBJECT
    InventoryHelper* inHelper;
    QSqlQueryModel* model;

public:
    explicit NegativeStock(QWidget *parent = 0);
    ~NegativeStock();

    void setTable();
private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

private:
    Ui::NegativeStock *ui;
};

#endif // NEGATIVESTOCK_H
