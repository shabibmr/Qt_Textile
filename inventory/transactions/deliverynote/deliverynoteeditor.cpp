#include "deliverynoteeditor.h"
#include "voucherOperations/vouchereditorselector.h"

DeliveryNoteEditor::DeliveryNoteEditor(DeliveryNoteDatabaseHelper *db, InventoryLedgerTransactionsEditor *parent) :
     InventoryLedgerTransactionsEditor(GMVoucherTypes::DeliveryNote, db, parent)
{

    this->dbHelper =db;

    bool useSectionFlag = ConfigurationSettingsDatabaseHelper::getValue(useDefaultItemSection,false).toBool();

    initExtraActions();
    setDrawerLayout();

}

DeliveryNoteEditor::~DeliveryNoteEditor()
{
}

void DeliveryNoteEditor::initExtraActions()
{

//    QObject::connect(exportButton,&QtMaterialIconButton::clicked,
//                     this,&DeliveryNoteEditor::exportVoucher);
    selectVoucherWidget = new ShowVouchersWidget();
}

void DeliveryNoteEditor::setDrawerLayout()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    QPushButton *button = new QPushButton(this);
    button->setText("Convert To Sales Invoice");
    drawer->drawerLayout()->addWidget(button);

    QObject::connect(exportButton,&QPushButton::clicked,
                     drawer,&QtMaterialDrawer::openDrawer);

    QObject::connect(button,&QPushButton::clicked,
                     this,&DeliveryNoteEditor::exportVoucher);

}

void DeliveryNoteEditor::saveVoucher()
{
    if(editorStatus == EditorStatus::NewVoucher)
        voucher->status = QuotationStatus::DeliveryReady;

    InventoryLedgerTransactionsEditor::saveVoucher();

}

void DeliveryNoteEditor::importVoucher(GeneralVoucherDataObject *voucher2)
{

}

//void DeliveryNoteEditor::importItemsFromVoucher(GeneralVoucherDataObject *voucher2)
//{

////    if(useSectionFlag){
//        for(int i=0; i<voucher2->InventoryItems.size(); i++){
//            qDebug()<<Q_FUNC_INFO<<"section"<<voucher2->InventoryItems[i].BaseItem.section<<voucher2->InventoryItems[i].BaseItem.ItemName;
//            voucher2->InventoryItems[i].BaseItem.fromGodownID = voucher2->InventoryItems[i].BaseItem.section;
//        }
////    }
//    voucher->InventoryItems.append(voucher2->InventoryItems);
//    qDebug()<<voucher2->InventoryItems.size();
//    resetWidgets();


//}

void DeliveryNoteEditor::exportVoucher()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(voucher->status ==  QuotationStatus::Started){
        snackBar->addMessage("Delivery note not Saved!!!");
        return;
    }

    if(voucher->status != QuotationStatus::SalesInvoiceRaised){

        qDebug()<<"export dn to invoice";

        //    if(SalesVoucherEditorWidget != nullptr)
        //        delete SalesVoucherEditorWidget;

        qDebug()<<"11";

        VoucherEditorSelector *vEditor = new VoucherEditorSelector;
        vEditor->exportVoucher(voucher, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher));

        QObject::connect(vEditor,&VoucherEditorSelector::saveCompleted,
                         this,&DeliveryNoteEditor::SalesVoucherSaved);
        this->hide();
    }
    else{
        snackBar->addMessage("Sales Invoice is generated Already");
    }
}

void DeliveryNoteEditor::SalesVoucherSaved()
{
    // Update SalesOrders

    voucher->status = QuotationStatus::SalesInvoiceRaised;
    dbHelper->updateVoucher(voucher,voucher->voucherNumber);

    QMessageBox box; box.setText("Saved Order"); box.exec();
    this->close();

}

void DeliveryNoteEditor::sendEmail()
{

}

void DeliveryNoteEditor::getVoucherstoImport()
{
    if(voucher->ledgerObject.LedgerID.length()<1){
        snackBar->addMessage("Please Set Ledger");
        return;
    }

    //    if(selectVoucherWidget != nullptr)
    delete selectVoucherWidget;

    selectVoucherWidget = new ShowVouchersWidget();
    selectVoucherWidget->setVoucherTypes(GMVoucherTypes::SalesOrder, GMVoucherTypes::DeliveryNote);
    //    selectVoucherWidget->show();
    selectVoucherWidget->setLedger(voucher->ledgerObject.LedgerID);
    selectVoucherWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
    //    selectVoucherWidget->setWindowState(Qt::WindowMaximized);
    selectVoucherWidget->resize(500,200);
    QObject::connect(selectVoucherWidget,SIGNAL(importVoucher(GeneralVoucherDataObject *)),
                     this, SLOT(importItemsFromVoucher(GeneralVoucherDataObject *)));
    //    QObject::connect(selectVoucherWidget,SIGNAL(noVoucherToExport()),
    //                     this, SLOT(noVoucherToImport()));

    connect(selectVoucherWidget, &ShowVouchersWidget::noVoucherToExport, this, [=]{
        snackBar->addMessage("No vouchers to import!!!");
    });
}

void DeliveryNoteEditor::noVoucherToImport()
{
    qDebug()<<"No voucher to export";
    snackBar->addMessage("No vouchers to import!!!");
    return;
}


void DeliveryNoteEditor::printVoucher(QPrinter *printer)
{
    int val = sender()->property("PrintType").toInt();
    qDebug()<<"Print Type = "<<val;
    qDebug()<<"Public Type "<<shareOptionsWidget->printType ;
    if(val == PrintTypes::Default){
        printHelper = new PrinterHelper();
        //    printHelper->testPrint(voucher);
        printHelper->setPrinter(printer);
        qDebug()<<"Printing Delivery";
        printHelper->PrintDispatchSlip(voucher);
    }
    else if(val == PrintTypes::DotMatrix){
//        PrintDotMatrix printer;
//        printer.printSalesVoucher(voucher);
    }
    else if(val == PrintTypes::Thermal){
        CounterPrinterHelper counter;
        counter.setPrinter(printer);
        counter.printKOT(voucher,true);
    }
}
