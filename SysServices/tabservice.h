#ifndef TABSERVICE_H
#define TABSERVICE_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include "qtservice.h"
#include <QTimer>
#include <database/inventory/salesorder/salesorderdatabasehelper.h>
#include <database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h>
#include "counter/PrinterHelper/counterprinterhelper.h"

class TabService : public QtService<QCoreApplication>,QObject
{


public:
    /**
     * @brief The constructor
     * @param argc
     * @param argv
     */
    TabService(int argc, char **argv);

    /**
     * @brief The deconstructor
     */
    ~TabService();

    /**
     * @brief Start the service
     */
    void start();

    /**
     * @brief Pause the service
     */
    void pause();

    /**
     * @brief Resume the service
     */
    void resume();

    /**
     * @brief Stop the service
     */
    void stop();

public slots:
    void tabSync();
private:
    //Add in any pointers
    QTimer *datasync;
    SalesOrderDatabaseHelper* salesOrderHelper;
    SalesInventoryItemDatabaseHelper *itemsHelper;
    CounterPrinterHelper *printer;

};

#endif // TABSERVICE_H
