#include "salesvouchereditor.h"
//#include "ui_salesvouchereditor.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "counter/MainScreen/itemdetail.h"
#include <QScreen>

SalesVoucherEditor::SalesVoucherEditor(VoucherDBAbstract *dbhelper, InventoryLedgerTransactionsEditor *parent) :
    InventoryLedgerTransactionsEditor (GMVoucherTypes::SalesVoucher, dbhelper,parent)
{

}

void SalesVoucherEditor::getVoucherstoImport()
{
    if(voucher->ledgerObject.LedgerID.length()<1){
        snackBar->addMessage("Please Set Ledger");
        return;
    }

    ShowVouchersWidget *selectVoucherWidget = new ShowVouchersWidget();
    selectVoucherWidget->setVoucherTypes(GMVoucherTypes::SalesOrder, GMVoucherTypes::SalesVoucher);
    selectVoucherWidget->setLedger(voucher->ledgerObject.LedgerID);
    selectVoucherWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
    //    selectVoucherWidget->setWindowState(Qt::WindowMaximized);
    selectVoucherWidget->resize(500,200);
    selectVoucherWidget->show();
    QObject::connect(selectVoucherWidget,SIGNAL(importVoucher(GeneralVoucherDataObject *)),
                     this, SLOT(importItemsFromVoucher(GeneralVoucherDataObject *)));

}

void SalesVoucherEditor::saveVoucher()
{
//    QTime dieTime= QTime::currentTime().addSecs(1);
//    while (QTime::currentTime() < dieTime)
//        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    saveButton->setDisabled(true);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(editorStatus == EditorStatus::NewVoucher)
        voucher->status = QuotationStatus::SalesInvoiceRaised;
    InventoryLedgerTransactionsEditor::saveVoucher();
    if(saveStatus)
        shareOptionsWidget->showPrintOptions();

}

void SalesVoucherEditor::printVoucher(QPrinter *printer)
{
    int val = sender()->property("PrintType").toInt();
    qDebug()<<"Print Type = "<<val;
    qDebug()<<"Public Type "<<shareOptionsWidget->printType ;
    if(val == PrintTypes::Default){
        printHelper = new PrinterHelper();
        //    printHelper->testPrint(voucher);
        printHelper->setPrinter(printer);
        printHelper->PrintInventorySalesVoucher(voucher);
    }
    else if(val == PrintTypes::DotMatrix){
        PrintDotMatrix printer;
        printer.printSalesVoucher(voucher);
    }
    else if(val == PrintTypes::Thermal){
        CounterPrinterHelper counter;
        counter.setPrinter(printer);
        counter.printSalesVoucher(voucher);
    }
}

void SalesVoucherEditor::exportToPdf(QPrinter *printer)
{
    printHelper = new PrinterHelper();
    //    printHelper->testPrint(voucher);
    printHelper->setPrinter(printer);
    printHelper->PrintInventorySalesVoucher(voucher);
}

void SalesVoucherEditor::importVoucher(GeneralVoucherDataObject *voucher2)
{
    setVoucher(voucher2);
}


