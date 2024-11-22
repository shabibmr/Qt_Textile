#ifndef GODOWNALLOCATIONWIDGET_H
#define GODOWNALLOCATIONWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QSqlQueryModel>
#include <QLineEdit>

#include "database/Reports/closingstockhelper.h"
#include "customwidgets/Basic/Material/gmmaterialnumberedit.h"
#include "finance/transactions/calculator/fifocalculator.h"


namespace Ui {
class GodownAllocationWidget;
}

class GodownAllocationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GodownAllocationWidget(GeneralVoucherDataObject *voucher, int itemRow, QWidget *parent = nullptr);
    ~GodownAllocationWidget();

    QTableWidget *godownTable;
    QSqlQueryModel *allGodownModel;
    CompoundItemDataObject *Item;
    int transactionType;
    int itemRow;
    GeneralVoucherDataObject *voucher;

    ClosingStockHelper *dbHelper;
    FifoCalculator *fifoCalc;
    float totalQtySelected = 0;
    GMMaterialNumberEdit *selectedQty;

private slots:
    void setTable();
    void setWidgets();
    void setGodownAllocation();
    float getSetQtyFromFifo(int row);
    void calcTotal();


private:
    Ui::GodownAllocationWidget *ui;
};

#endif // GODOWNALLOCATIONWIDGET_H
