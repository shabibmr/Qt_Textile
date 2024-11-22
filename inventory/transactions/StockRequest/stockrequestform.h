#ifndef STOCKREQUESTFORM_H
#define STOCKREQUESTFORM_H

#include <QWidget>
#include "login/loginvalues.h"
#include "database/inventory/StockRequest/stockrequestdatabasehelper.h"
#include "customwidgets/SearchWidgets/Inventoryitem/gmitemsearchlite.h"
#include "sharetools/excel/excelreporthelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include <QToolButton>
#include "database/hr/userprofiledatabasehelper.h"

namespace Ui {
class StockRequestForm;
}

class StockRequestForm : public QWidget
{
    Q_OBJECT

public:
    explicit StockRequestForm(QWidget *parent = 0);
    ~StockRequestForm();

    GeneralVoucherDataObject *obj;
    SalesInventoryItemDatabaseHelper itemHelper;
    StockRequestDatabaseHelper *dbHelper;

    void setTable();
    QSqlQueryModel *itemsmodel;
    QString vNo="";

    void showVoucher(GeneralVoucherDataObject *obj);

public slots:
    void ItemSelected(inventoryItemDataModel item);
    void deleteClicked(bool);
    void reqQtyEntered();
    void issQtyEntered();
    void appendItem(CompoundItemDataObject,int);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();
signals:
    void closing();

private:
    Ui::StockRequestForm *ui;
};

#endif // STOCKREQUESTFORM_H
