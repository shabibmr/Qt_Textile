#include "salesordervouchereditor.h"
//#include "ui_salesordervouchereditor.h"
#include "finance/transactions/sales/salesvouchereditor.h"
#include "inventory/transactions/deliverynote/deliverynoteeditor.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "voucherOperations/vouchereditorselector.h"


SalesOrderVoucherEditor::SalesOrderVoucherEditor(SalesOrderDatabaseHelper *db, InventoryLedgerTransactionsEditor *parent) :
    InventoryLedgerTransactionsEditor(GMVoucherTypes::SalesOrder, db, parent)
{
    this->dbHelper =db;

    setDrawerLayout();
    initGlobalObjects();

}

//SalesOrderVoucherEditor::~SalesOrderVoucherEditor()
//{
//    delete ui;
//}

void SalesOrderVoucherEditor::setDrawerLayout()
{


    QPushButton *button = new QPushButton(this);
    button->setText("Convert To Sales Invoice");
    drawer->drawerLayout()->addWidget(button);

    QPushButton *DNbutton = new QPushButton(this);
    DNbutton->setText("Convert To Delivery Note");
    drawer->drawerLayout()->addWidget(DNbutton);

    QObject::connect(exportButton,&QPushButton::clicked,
                     drawer,&QtMaterialDrawer::openDrawer);

    QObject::connect(button,&QPushButton::clicked,
                     this,&SalesOrderVoucherEditor::exportToSalesVoucher);
    QObject::connect(DNbutton,&QPushButton::clicked,
                     this,&SalesOrderVoucherEditor::exportToDeliveryNote);

    //    QObject::connect(moreButton,&QtMaterialIconButton::clicked,
    //                     this, [=](bool Checked){
    //        ledgerTable->setHidden(!ledgerTable->isHidden());
    //    });

}


void SalesOrderVoucherEditor::initGlobalObjects()
{
    deliveryDateWidget = new GMDateTimeEdit(&voucher->DeliveryDate);
    addToExtrasLayout2(new QLabel("Delivery Date"));
    addToExtrasLayout2(deliveryDateWidget);
    extrasLayout2->addStretch(1);

}



void SalesOrderVoucherEditor::resetWidgets()
{
    qDebug()<<Q_FUNC_INFO<<voucher->SalesmanID;
    InventoryLedgerTransactionsEditor::resetWidgets();

    deliveryDateWidget->setVariable(&voucher->DeliveryDate);
}

void SalesOrderVoucherEditor::saveVoucher()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(editorStatus == EditorStatus::NewVoucher){
        if(voucher->DeliveryDate > QDateTime::currentDateTime().addSecs(60*60)){
        voucher->status = QuotationStatus::SalesOrderDeliveryCreated;
        }
        else{
            voucher->status = QuotationStatus::SalesOrderCreated;
        }
    }

    voucher->PaidAmount = voucher->advanceCash;
    bool printKOtfromSOFlag = ConfigurationSettingsDatabaseHelper::getValue(printKOtfromSO,true).toBool();

    if(printKOtfromSOFlag){
        printKOT();
    }

    InventoryLedgerTransactionsEditor::saveVoucher();

    if(saveStatus)
        shareOptionsWidget->showPrintOptions();
    //    closeEditor();
}

//void SalesOrderVoucherEditor::askForPrint()
//{
//    QMessageBox::StandardButton reply;
//    reply = QMessageBox::question(this,"Print Voucher","Would You Like to Print?",
//                                  QMessageBox::Yes|QMessageBox::No);
//    if(reply==QMessageBox::Yes){
//        printVoucher();
//    }
//}

//void SalesOrderVoucherEditor::showContactDetails()
//{
//    AddressDialogWidget->setWindowFlags(Qt::Window);
//    AddressDialogWidget->setAddr(&voucher->Contact);
//    AddressDialogWidget->setFocus();
//    AddressDialogWidget->show();
//    AddressDialogWidget->setFocustoPhone();

//    QObject::connect(AddressDialogWidget,&AddressDialog::setAddress,
//                     this, [=](QString Address, QString Phone, QString Name, QString route){
//        voucher->Contact.address= Address;
//        voucher->Contact.PhoneNumber= Phone;
//        voucher->Contact.ContactName= Name;
//        voucher->ledgerObject.ContactPersonNumber = Phone;
//        voucher->ledgerObject.emailAddress = Address;
//        voucher->ledgerObject.ContactPersonName = Name;
//        voucher->Contact.route = route;
//    });

//}



void SalesOrderVoucherEditor::importVoucher(GeneralVoucherDataObject *voucher2)
{

}

void SalesOrderVoucherEditor::importItemsFromVoucher(GeneralVoucherDataObject *voucher2)
{

}

void SalesOrderVoucherEditor::exportToSalesVoucher()
{
    drawer->closeDrawer();
    VoucherEditorSelector *vEditor = new VoucherEditorSelector;
    //    if(voucher->status != QuotationStatus::SalesInvoiceRaised){
    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->SalesmanID;
    vEditor->exportVoucher(voucher, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher));

    QObject::connect(vEditor,&VoucherEditorSelector::saveCompleted,
                     this,&SalesOrderVoucherEditor::SalesVoucherSaved);
    this->hide();
    //    }

    //    this->hide();
}

void SalesOrderVoucherEditor::exportToDeliveryNote()
{
    drawer->closeDrawer();
    //    if(voucher->status < QuotationStatus::DeliveryReady){
    this->hide();
    VoucherEditorSelector *vEditor = new VoucherEditorSelector;
    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->SalesmanID;
    QObject::connect(vEditor,&VoucherEditorSelector::saveCompleted,
                     this,&SalesOrderVoucherEditor::DeliveryNoteSaved);
    vEditor->exportVoucher(voucher, GMVoucherTypes::getVoucherString(GMVoucherTypes::DeliveryNote));
    //    }

}

void SalesOrderVoucherEditor::SalesVoucherSaved()
{
    // Update SalesOrders
    this->show();

    voucher->status = QuotationStatus::SalesInvoiceRaised;


    dbHelper->updateSalesOrderStatus(voucher);
    dbHelper->setAllItemVoucherStatusAsQStatus(voucher,voucher->status);
    QMessageBox box; box.setText("Saved Order"); box.exec();
    closeEditor();

}

void SalesOrderVoucherEditor::DeliveryNoteSaved()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    this->show();
    voucher->status = QuotationStatus::DeliveryReady;

    // Update SalesOrders
    qDebug()<<"Updating SO "<<voucher->voucherNumber;
    qDebug()<<"Updating SO "<<voucher->VoucherPrefix;
    qDebug()<<"Updating SO "<<voucher->VoucherPrefix;

        dbHelper->updateSalesOrderStatus(voucher);
        dbHelper->setAllItemVoucherStatusAsQStatus(voucher,voucher->status);
    QMessageBox box; box.setText("Saved Delivery Note"); box.exec();
    closeEditor();

}

void SalesOrderVoucherEditor::printVoucher(QPrinter *printer)
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

void SalesOrderVoucherEditor::exportToPdf(QPrinter *printer)
{
    printHelper = new PrinterHelper();
    //    printHelper->testPrint(voucher);
    printHelper->setPrinter(printer);
    printHelper->PrintInventorySalesVoucher(voucher);

}

void SalesOrderVoucherEditor::printKOT()
{
    bool askToPrintKOT = ConfigurationSettingsDatabaseHelper::getValue(askForPrint,false).toBool();
    if(askToPrintKOT){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,"Print Kot","Do you Want to print KOT",
                                      QMessageBox::Yes|QMessageBox::No);
        if(reply==QMessageBox::Yes){
            QPointer<CounterPrinterHelper> pHelper = new CounterPrinterHelper;
            pHelper->printKOT(voucher);
        }
        else{
            qDebug()<<"Not Printing";
        }

    }
}

void SalesOrderVoucherEditor::sendEmail()
{

}

void SalesOrderVoucherEditor::showVoucher()
{
    InventoryLedgerTransactionsEditor::showVoucher();
    exportButton->show();

}


