#ifndef CLOSINGENTRY_H
#define CLOSINGENTRY_H

#include <QWidget>
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "database/finance/Closing/dailyclosingdatabasehelper.h"
#include "database/finance/Closing/shiftdatabasehelper.h"


namespace Ui {
class ClosingEntry;
}

class ClosingEntry : public QWidget
{
    Q_OBJECT

public:
    explicit ClosingEntry(QWidget *parent = 0);
    ~ClosingEntry();
    CounterPrinterHelper *printer;

    DailyClosingDatabaseHelper *dbHelper;
    ShiftDatabaseHelper *shiftDatabaseHelper;
    ShiftDataModel *CurrClosing;
    ShiftDataModel lastClosing;

    LedgerMasterDatabaseHelper *ledhelper;
    AccountGroupMasterDatabaseHelper *accHelper;
    QList<ShiftDataModel> shiftsPending;

private slots:

    void on_CancelButton_clicked();

    void on_OkButton_clicked();

    void calcValues();

    void setValues();

private:
    Ui::ClosingEntry *ui;
};

#endif // CLOSINGENTRY_H
