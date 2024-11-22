#include "tabservice.h"
#include "finance/transactions/calculator/transactioncalculator.h"

TabService::TabService(int argc, char **argv): QtService<QCoreApplication>( argc,  argv,"GMTAB")
{
    try
    {
        //Set the service information
        setServiceDescription("GM TAB Service.");
        setServiceFlags(QtServiceBase::CanBeSuspended);
        salesOrderHelper = new SalesOrderDatabaseHelper();
        itemsHelper = new SalesInventoryItemDatabaseHelper();
        printer = new CounterPrinterHelper();
    }
    catch(...)
    {
        qCritical() << "An unknown error in the constructor";
    }
}

TabService::~TabService()
{
    try
    {
        //TO DO - release any memory
    }
    catch(...)
    {
        qCritical() << "An unknown error in the deconstructor";
    }

}

void TabService::start()
{
    try
    {
        QCoreApplication *app = application();
        qDebug() << "Service started!";
        qDebug() << app->applicationDirPath();
    }
    catch(...)
    {
        qCritical() << "An unknown error in the start";
    }
}

void TabService::pause()
{
    try
    {
        qDebug() << "Service paused!";
    }
    catch(...)
    {
        qCritical() << "An unknown error in the pause";
    }
}

void TabService::resume()
{
    try
    {
        qDebug() << "Service resumed!";
    }
    catch(...)
    {
        qCritical() << "An unknown error in the resume";
    }
}

void TabService::stop()
{
    try
    {
        qDebug() << "Service stopped!";
    }
    catch(...)
    {
        qCritical() << "An unknown error in the stop";
    }
}

void TabService::tabSync()
{

}
