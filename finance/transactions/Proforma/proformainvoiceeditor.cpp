#include "ProformaInvoiceEditor.h"
//#include "ui_ProformaInvoiceEditor.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "counter/MainScreen/itemdetail.h"
#include <QScreen>

ProformaInvoiceEditor::ProformaInvoiceEditor(VoucherDBAbstract *dbhelper, InventoryLedgerTransactionsEditor *parent) :
    InventoryLedgerTransactionsEditor (GMVoucherTypes::Proforma, dbhelper,parent)
{

}

void ProformaInvoiceEditor::getVoucherstoImport()
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

void ProformaInvoiceEditor::saveVoucher()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(editorStatus == EditorStatus::NewVoucher)
        voucher->status = QuotationStatus::SalesInvoiceRaised;
    InventoryLedgerTransactionsEditor::saveVoucher();
    if(saveStatus)
        shareOptionsWidget->showPrintOptions();

}

void ProformaInvoiceEditor::printVoucher(QPrinter *printer)
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

void ProformaInvoiceEditor::exportToPdf(QPrinter *printer)
{
    printHelper = new PrinterHelper();
    //    printHelper->testPrint(voucher);
    printHelper->setPrinter(printer);
    printHelper->PrintInventorySalesVoucher(voucher);
}

void ProformaInvoiceEditor::importVoucher(GeneralVoucherDataObject *voucher2)
{
    setVoucher(voucher2);
}


