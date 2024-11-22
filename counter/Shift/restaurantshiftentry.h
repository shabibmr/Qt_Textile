#ifndef RESTAURANTSHIFTENTRY_H
#define RESTAURANTSHIFTENTRY_H

#include <QWidget>
#include "datamodels/Counter/shiftdatamodel.h"
#include "database/finance/Closing/restaurantshiftdatabasehelper.h"
#include "database/Reports/ledgerhelper.h"
#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "database/finance/salesvoucher/salesvoucherdeleteddatabasehelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include <QMessageBox>
#include "database/finance/Closing/dailyclosingdatabasehelper.h"
#include <sharetools/email/emailthread.h>
#include "database/Reports/inventoryhelper.h"
#include <QSqlRecord>
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"
#include "customwidgets/TableViews/reportstableview.h"
#include "customwidgets/Models/gmshiftmodel.h"

namespace Ui {
class RestaurantShiftEntry;
}

class RestaurantShiftEntry : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantShiftEntry(QWidget *parent = 0);
    ~RestaurantShiftEntry();

    QString voucherPrefix;

    ShiftDataModel *shift;
    float shiftOpening;

    RestaurantShiftDatabaseHelper *dbHelper;

    GeneralVoucherDataObject PaymentDetails;

    salesVoucherDatabaseHelper *salesHelper;

    InventoryHelper *invHelper;

    LedgerHelper *ledHelper;

//    void setAllNonEditable();
    CounterPrinterHelper *printerHelper;

    QtMaterialIconButton *shareButton;
    ShareTools *shareOptionsWidget;

    void setShift(ShiftDataModel *value);

    ReportsTableView *salesTable;
    ReportsTableView *purchaseTable;
    ReportsTableView *receiptsTable;
    ReportsTableView *paymentsTable;
    ReportsTableView *debitNoteTable;
    ReportsTableView *creditNoteTable;

    QTableView *table1;
    QTableView *table2;

    GMShiftModel *model1;
    GMShiftModel *model2;


public slots:

    void setDenominationValues(DenominationsDataModel *obj);

    void sendMail();

private slots:

    void on_printButton_clicked();

    void on_saveButton_clicked();

    void on_CloseButton_clicked();

    void on_CalculateButton_clicked();

    void CalculateValues();

    void setValues();

//    void on_cashWithdrawnLineEdit_textChanged(const QString &arg1);

//    void on_openingCashLineEdit_returnPressed();

    void on_clearButton_clicked();
    void showShareOptions();

    void printReport(QPrinter *printer);

    void on_pdfButton_clicked();

signals:

    void shiftClosed();

private:
    Ui::RestaurantShiftEntry *ui;

};

#endif // RESTAURANTSHIFTENTRY_H
