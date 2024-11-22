#ifndef WORKORDERGENERATOR_H
#define WORKORDERGENERATOR_H

#include <QObject>
#
#include "database/inventory/workorder/workorderdatabasehelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"

class WorkOrderGenerator : public QObject
{
    Q_OBJECT
public:
    explicit WorkOrderGenerator(QObject *parent = nullptr);

    void generateWorkOrders();


    void setVoucher(GeneralVoucherDataObject *value);

    void printKot();

    private:
    WorkOrderDatabaseHelper *dbHelper;
    GeneralVoucherDataObject *voucher;
    CounterPrinterHelper *printer;
    GodownDatabaseHelper *godown;


signals:

};

#endif // WORKORDERGENERATOR_H
