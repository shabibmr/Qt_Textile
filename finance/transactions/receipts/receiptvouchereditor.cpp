#include "receiptvouchereditor.h"
//#include "ui_receiptvouchereditor.h"

#include "finance/transactions/BillwiseRecords/addbillwiserecord.h"


ReceiptVoucherEditor::ReceiptVoucherEditor(VoucherDBAbstract *db, VoucherEditorBase *parent) :
    VoucherEditorBase(parent)
{

    //    this->setWindowTitle(windowTitle);
    //    this->setObjectName("background");
    //    this->setStyleSheet("QWidget#background{background-color:#e0f7fa}");
    bool showCheque = ConfigurationSettingsDatabaseHelper::getValue(showChequeEntry, false).toBool();
    this->dbHelper = db;

    //    initDbHelpers();
    initVoucher();
    initGlobalObjects();

    initExtraActions();

    this->setTabOrder(mainLedgerWidget,DataTableWidget);
}

ReceiptVoucherEditor::~ReceiptVoucherEditor()
{
}

void ReceiptVoucherEditor::initExtraActions()
{

    moreButton->hide();
    exportButton->hide();
    contactButton->hide();

    QObject::connect(saveButton,&QPushButton::clicked,
                     this,&ReceiptVoucherEditor::saveVoucher);

}


void ReceiptVoucherEditor::initVoucher()
{
    editorStatus = EditorStatus::NewVoucher;
    voucher = new GeneralVoucherDataObject();
    voucher->VoucherDate = QDate::currentDate();
    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::ReceiptVoucher);
    voucher->voucherNumber = dbHelper->getNextVoucherNo(voucher->VoucherPrefix);
    voucher->status = QuotationStatus::Started;

    setTitle(GMVoucherTypes::getVoucherTitle(GMVoucherTypes::ReceiptVoucher));
    voucherDateWidget->setVariable(&voucher->VoucherDate);
    voucherNumberWidget->setVariable(&voucher->voucherNumber);
    voucherNarrationWidget->setVariable(&voucher->narration);

}

void ReceiptVoucherEditor::initGlobalObjects()
{

    lis<<"#"<<"Ledgers"<<"Amount";


    DataTableWidget = new LedgervoucherTableView(voucher,2);

    QStringList modeOfPayGroups;
    modeOfPayGroups.append(accHelper->getGroupIDByName("Cash-in-Hand"));
    modeOfPayGroups.append(accHelper->getGroupIDByName("Bank Accounts"));


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
    });


    QStringList groups ;
    groups <<"0x5x19"<< "0x2x15";
    DataTableWidget->setItemDelegate(new GMLedgerTransactionDelegate(
                                         ledgerHelper,ledgerHelper->getAllLedgersModel()));

    model = new GMReceiptEntryModel(voucher,lis.size(),this);
    model->setHeaderStrings(lis);

    QObject::connect(model, &GMReceiptEntryModel::setBillwiseRecords, this, &ReceiptVoucherEditor::setBillwiseMapping);

    DataTableWidget->setAvlcols(avlcols);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
    DataTableWidget->hideColumn(3);

    setTableLayout(DataTableWidget);

}

void ReceiptVoucherEditor::resetWidgets()
{
    VoucherEditorBase::resetWidgets();

    delete model;

    model = new GMReceiptEntryModel(voucher,lis.size(),this);
    model->setHeaderStrings(lis);
    DataTableWidget->setVoucher(voucher);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
    QObject::connect(model, &GMReceiptEntryModel::setBillwiseRecords, this, &ReceiptVoucherEditor::setBillwiseMapping);
    //    QObject::connect(model, SIGNAL(updateValues()), this, SLOT(setChequeData()));
}

void ReceiptVoucherEditor::saveVoucher()
{
    if(voucher->ledgersList.size()<1){
        snackBar->addMessage("Nothig to Save!!!");
        return;
    }

    if(voucher->ledgerObject.LedgerID.length()<1){
        snackBar->addMessage("Please Set Mode of Payment");
        return;
    }
    if((voucher->ledgerObject.LedgerGroupId == accHelper->getGroupIDByName("Bank Accounts"))){
        if(!chequeEntryCompleted){
            if(!setChequeData())
                return;
        }
    }

    if(editorStatus == EditorStatus::NewVoucher)
        voucher->status = QuotationStatus::SalesInvoiceRaised;

    //    else
    VoucherEditorBase::saveVoucher();
    closeEditor();


}

void ReceiptVoucherEditor::printVoucher(QPrinter *printer)
{
    CounterPrinterHelper *printHelper = new CounterPrinterHelper();

    printHelper->printReceiptVoucher(voucher);
}


bool ReceiptVoucherEditor::setChequeData()
{
    if(voucher->ledgerObject.LedgerGroupId == accHelper->getGroupIDByName("Bank Accounts")){
        chqObj = new ChequeDetail();
        if(voucher->ledgersList.size()>1){
            snackBar->addMessage("More than one ledgers not allowed for cheque entry, ledgers will be removed");
            while(voucher->ledgersList.size()>1)
                voucher->ledgersList.removeLast();

            return false;
        }
        chqObj->LedgerID = voucher->ledgersList[0].LedgerID;
        chqObj->LedgerName = voucher->ledgersList[0].LedgerName;
        chqObj->Amount = voucher->grandTotal;
        chqObj->crAmount = voucher->grandTotal;
        chqObj->VoucherID = voucher->voucherNumber;
        chqObj->VoucherType = voucher->voucherType;
        chqObj->VoucherPrefix = voucher->VoucherPrefix;
        chqObj->InstrumentDate = voucher->VoucherDate;
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

    return true;
}

void ReceiptVoucherEditor::sendEmail()
{

}

void ReceiptVoucherEditor::showVoucher()
{
    VoucherEditorBase::showVoucher();
    TransactionCalculator::VoucherCalculateLedgersTotal(voucher);
    resetWidgets();
}


void ReceiptVoucherEditor::setBillwiseMapping(LedgerMasterDataModel* ledger, QDate voucherDate)
{
    //    AddBillwiseRecord* billwiseWidget = new AddBillwiseRecord() ;
    //    billwiseWidget->setWindowFlags(Qt::Window);
    //    billwiseWidget->setAttribute(Qt::WA_DeleteOnClose);
    //        billwiseWidget->setVoucherData(ledger, voucherDate);
    ////    billwiseWidget->setLedger(ledger);
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
