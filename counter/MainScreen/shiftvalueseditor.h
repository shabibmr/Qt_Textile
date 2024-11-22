#ifndef SHIFTVALUESEDITOR_H
#define SHIFTVALUESEDITOR_H

#include <QWidget>
#include <QHeaderView>
#include <QTableView>
#include <QTableWidget>
#include <QSqlQueryModel>
#include <QLabel>
#include "customwidgets/Models/gmshiftsalesmodel.h"
#include "customwidgets/TableViews/shiftsalestableview.h"
#include "customwidgets/Delegates/Shift/gmshiftdelegate.h"
#include "customwidgets/TableViews/reportstableview.h"


#include "database/finance/Closing/restaurantshiftdatabasehelper.h"
#include "counter/MainScreen/modeofpay.h"


namespace Ui {
class ShiftValuesEditor;
}

class ShiftValuesEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ShiftValuesEditor(ShiftDataModel *shift, QWidget *parent = nullptr);
    ~ShiftValuesEditor();

    ShiftDataModel *shift;
    QTableWidget *shiftTable;
    QTableWidget *expTable;
    ReportsTableView *vouchersView;

//    QSqlQueryModel *allLedgersModel;
    RestaurantShiftDatabaseHelper *dbHelper;
    QSqlQueryModel *model;

    float diffTotal=0;
    float actualTotal =0;
    float systemTotal = 0;




    void setShift(ShiftDataModel *value);

public slots:
//    void initExtraActions();
    void setWidgets();
    void setTable();
    void setExpenseTable();
    void calculateTotals();
    void showVouchersByLedger(QModelIndex index);
    void showModeOfPay(QString vNo, QString vPrefix);


private:
    Ui::ShiftValuesEditor *ui;
};

#endif // SHIFTVALUESEDITOR_H
