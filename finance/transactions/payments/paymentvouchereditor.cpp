#include "paymentvouchereditor.h"
//#include "ui_paymentvouchereditor.h"

#include "finance/transactions/BillwiseRecords/addbillwiserecord.h"
#include <QStringList>


PaymentVoucherEditor::PaymentVoucherEditor(VoucherDBAbstract *dbhelper, VoucherEditorBase *parent) :
    VoucherEditorBase (parent)
{

    this->setWindowTitle(windowTitle);
    this->setObjectName("background");
    this->setStyleSheet("QWidget#background{background-color:#e0f7fa}");

    this->dbHelper = dbhelper;

    //    initDbHelpers();
    initVoucher();
    initGlobalObjects();
    //    setWidgets();
    initExtraActions();
}


void PaymentVoucherEditor::initExtraActions()
{

    moreButton->hide();
    exportButton->hide();
    contactButton->hide();

    QObject::connect(saveButton,&QPushButton::clicked,
                     this,&PaymentVoucherEditor::saveVoucher);
}


void PaymentVoucherEditor::initVoucher()
{

    voucher = new GeneralVoucherDataObject();

    editorStatus = EditorStatus::NewVoucher;

    voucher->VoucherDate = QDate::currentDate();

    voucher->VoucherPrefix = LoginValues::voucherPrefix;

    voucher->voucherType =  GMVoucherTypes::getVoucherString(GMVoucherTypes::PaymentVoucher);

    voucher->voucherNumber = dbHelper->getNextVoucherNo(voucher->VoucherPrefix);

    voucher->status = QuotationStatus::Started;


    setTitle(GMVoucherTypes::getVoucherTitle(GMVoucherTypes::PaymentVoucher));
    voucherDateWidget->setVariable(&voucher->VoucherDate);
    voucherNumberWidget->setVariable(&voucher->voucherNumber);
    voucherNarrationWidget->setVariable(&voucher->narration);


}

void PaymentVoucherEditor::initGlobalObjects()
{



    lis<<"#"<<"Ledgers"<<"Amount";
    DataTableWidget = new LedgervoucherTableView(voucher,1);


    QStringList modeOfPayGroups;
    modeOfPayGroups.append(accHelper->getGroupIDByName("Cash-in-Hand"));
    modeOfPayGroups.append(accHelper->getGroupIDByName("Bank Accounts"));
    qDebug()<<"groups"<<modeOfPayGroups;


    mainLedgerWidget->setSrcModel(ledgerHelper->getLedgerQueryString(modeOfPayGroups));
    mainLedgerWidget->setLabel("Mode Of Pay");

    connect(mainLedgerWidget,&GMLedgerLineMaterial::itemSelectedwithItem,
            this,[=](LedgerMasterDataModel led){
        voucher->ledgerObject = led;
        if(led.LedgerID.length()<1)
            return;
        mainLedgerWidget->clearFocus();
        DataTableWidget->setEnabled(true);
        DataTableWidget->clearFocus();
        //        DataTableWidget->setFocusToFirstRow();



    });

    QStringList groups ;
    groups <<"0x5x19"<< "0x2x15";
    DataTableWidget->setItemDelegate(new GMLedgerTransactionDelegate(
                                         ledgerHelper,ledgerHelper->getAllLedgersModel()));

    model = new GMPaymentEntryModel(voucher,lis.size(),this);

    model->setHeaderStrings(lis);
    QObject::connect(model, &GMPaymentEntryModel::setBillwiseRecords, this, &PaymentVoucherEditor::setBillwiseMapping);
    DataTableWidget->setAvlcols(avlcols);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();


    DataTableWidget->hideColumn(3);

    setTableLayout(DataTableWidget);


}

void PaymentVoucherEditor::resetWidgets()
{
    VoucherEditorBase::resetWidgets();
    delete model;


    model = new GMPaymentEntryModel(voucher,lis.size(),this);
    model->setHeaderStrings(lis);
    QObject::connect(model, &GMPaymentEntryModel::setBillwiseRecords, this, &PaymentVoucherEditor::setBillwiseMapping);
    DataTableWidget->setVoucher(voucher);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();

    //    resetWidgets();
}

void PaymentVoucherEditor::saveVoucher()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(voucher->ledgersList.size()<1){
        snackBar->addMessage("No Items Found!!!");
        return;
    }

    if(voucher->ledgerObject.LedgerID.length()<1){
        snackBar->addMessage("Please Select Payment Mode");
        return;
    }
    if((voucher->ledgerObject.LedgerGroupId == accHelper->getGroupIDByName("Bank Accounts"))){
        qDebug()<<"setting cheque entry"<<accHelper->getGroupIDByName("Bank Accounts")<<voucher->ledgerObject.LedgerGroupId;
        if(!chequeEntryCompleted)
            if(!setChequeData())
                return;
    }

    //    else
    VoucherEditorBase::saveVoucher();


    closeEditor();
}

void PaymentVoucherEditor::printVoucher(QPrinter *printer)
{


//    int val = sender()->property("PrintType").toInt();
//    qDebug()<<"Print Type = "<<val;
//    qDebug()<<"Public Type "<<shareOptionsWidget->printType ;
//    if(val == PrintTypes::Default){
//        printHelper = new PrinterHelper();
//        //    printHelper->testPrint(voucher);
//        printHelper->setPrinter(printer);
//        printHelper->PrintPaymentVoucher(voucher);
//    }
////    else if(val == PrintTypes::DotMatrix){
////        PrintDotMatrix printer;
////        printer.printSalesVoucher(voucher);
////    }
//    else if(val == PrintTypes::Thermal){
        CounterPrinterHelper counter;
        counter.setPrinter(printer);
        counter.printPaymentVoucher(voucher);
//    }
}

bool PaymentVoucherEditor::setChequeData()
{
    qDebug()<<Q_FUNC_INFO;

    if(voucher->ledgerObject.LedgerGroupId == accHelper->getGroupIDByName("Bank Accounts")){
        qDebug()<<"327";

        chqObj = new ChequeDetail;
        qDebug()<<"330";
        if(voucher->ledgersList.size()>1){
            snackBar->addMessage("More than one ledgers not allowed for cheque entry, ledgers will be removed");
            while(voucher->ledgersList.size()>1)
                voucher->ledgersList.removeLast();

            return false;
        }

        qDebug()<<voucher->ledgersList[0].LedgerID<<voucher->grandTotal;
        chqObj->LedgerID = voucher->ledgersList[0].LedgerID;
        chqObj->LedgerName = voucher->ledgersList[0].LedgerName;
        chqObj->Amount = voucher->grandTotal;
        chqObj->drAmount = voucher->grandTotal;
        chqObj->VoucherID = voucher->voucherNumber;
        chqObj->VoucherType = voucher->voucherType;
        chqObj->VoucherPrefix = voucher->VoucherPrefix;
        chqObj->OurBankId = voucher->ledgerObject.LedgerID;
        chqObj->BankName = voucher->ledgerObject.LedgerName;
        chqObj->InstrumentDate = voucher->VoucherDate;
        chqObj->IssuedOn = voucher->VoucherDate;

        voucher->chequeEntry = chqObj;

        bool showCheque = ConfigurationSettingsDatabaseHelper::getValue(showChequeEntry, false).toBool();

        if(showCheque){
            chequeWidget = new ChequeWidget(voucher);
            chequeWidget->setWindowFlags(Qt::Window);
            chequeWidget->setAttribute(Qt::WA_DeleteOnClose);
            chequeWidget->show();
            QObject::connect(chequeWidget, &ChequeWidget::save, this, [=](){
                chequeEntryCompleted = true;
                saveVoucher();
            });
            //            QObject::connect(chequeWidget, SIGNAL(closing()), this, SLOT(saveVoucher()));
            qDebug()<<"352";
            return false;
        }
    }

    qDebug()<<"355";

    return true;
}

void PaymentVoucherEditor::sendEmail()
{

}

void PaymentVoucherEditor::appendLedgersList(LedgerMasterDataModel led)
{
    voucher->ledgersList.append(led);
    TransactionCalculator::VoucherCalculateLedgersTotal(voucher);
    resetWidgets();
}

void PaymentVoucherEditor::setNarration(QString narration)
{
    voucher->narration = narration;
    resetWidgets();
}

void PaymentVoucherEditor::showVoucher()
{
    VoucherEditorBase::showVoucher();
    //    calcTotals();
    TransactionCalculator::VoucherCalculateLedgersTotal(voucher);

    resetWidgets();
}

void PaymentVoucherEditor::setBillwiseMapping(LedgerMasterDataModel* ledger, QDate voucherDate)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    //    AddBillwiseRecord* billwiseWidget = new AddBillwiseRecord() ;
    //    billwiseWidget->setWindowFlags(Qt::Window);
    //    billwiseWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    billwiseWidget->setVoucherData(ledger, voucherDate);
    //    //    billwiseWidget->setLedger(ledger);
    //    billwiseWidget->setBillwiseMapping(voucherDate,true);
    //    if(ConfigurationSettingsDatabaseHelper::getValue(showBillwiseMapping,false).toBool()){
    //        billwiseWidget->setFocus();
    //        billwiseWidget->showMaximized();
    //    }
    bool showBillwiseMappingFlag = ConfigurationSettingsDatabaseHelper::getValue(showBillwiseMapping,false).toBool();
    if(showBillwiseMappingFlag && (
                ledger->LedgerGroupId == accHelper->getGroupIDByName("Sundry Debtors")
                || ledger->LedgerGroupId == accHelper->getGroupIDByName("Sundry Creditors") )){
        BillwiseMappingEditor *billwiseWidget = new BillwiseMappingEditor(ledger, voucher);
        //    billwiseWidget->setWindowFlag(Qt::Popup);
        billwiseWidget->setAttribute(Qt::WA_DeleteOnClose);
        billwiseWidget->show();
    }

}
