#ifndef SHIFTENTRY_H
#define SHIFTENTRY_H

#include <QWidget>
#include "datamodels/Counter/shiftdatamodel.h"
#include "database/finance/Closing/shiftdatabasehelper.h"
#include "database/Reports/ledgerhelper.h"
#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include <QMessageBox>
#include "database/finance/Closing/dailyclosingdatabasehelper.h"

namespace Ui {
class ShiftEntry;
}

class ShiftEntry : public QWidget
{
    Q_OBJECT

public:
    explicit ShiftEntry(QWidget *parent = 0);
    ~ShiftEntry();

    QString voucherPrefix;
    ShiftDataModel *shift;

    ShiftDatabaseHelper *dbHelper;

    GeneralVoucherDataObject PaymentDetails;

    salesVoucherDatabaseHelper *salesHelper;



    LedgerHelper *ledHelper;

    void setAllNonEditable();

public slots:

    void setDenominationValues(float amount);

private slots:

    void on_printButton_clicked();

    void on_saveButton_clicked();

    void on_CloseButton_clicked();

    void on_CalculateButton_clicked();

    void CalculateValues();

    void setValues();

    void on_cashWithdrawnLineEdit_textChanged(const QString &arg1);

    void on_openingCashLineEdit_returnPressed();

signals:

    void shiftClosed();

private:
    Ui::ShiftEntry *ui;

};

#endif // SHIFTENTRY_H
