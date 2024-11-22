#include "journalvouchereditor.h"

//#include "ui_journalvouchereditor.h"


JournalVoucherEditor::JournalVoucherEditor(VoucherDBAbstract *db, VoucherEditorBase *parent) :
    VoucherEditorBase(parent)
{

    this->dbHelper = db;

    initVoucher();
    initGlobalObjects();
    initExtraActions();

}

JournalVoucherEditor::~JournalVoucherEditor()
{
}

void JournalVoucherEditor::initExtraActions()
{
    contactButton->hide();
    moreButton->hide();

    QObject::connect(saveButton,&QPushButton::clicked,
                     this,&JournalVoucherEditor::saveVoucher);

}

void JournalVoucherEditor::initVoucher()
{
    editorStatus = EditorStatus::NewVoucher;

    voucher = new GeneralVoucherDataObject();
    voucher->VoucherDate = QDate::currentDate();
    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::JournalVoucher);
    voucher->voucherNumber = dbHelper->getNextVoucherNo(voucher->VoucherPrefix);
    voucher->status = QuotationStatus::Started;

    setTitle(GMVoucherTypes::getVoucherTitle(GMVoucherTypes::JournalVoucher));
    voucherDateWidget->setVariable(&voucher->VoucherDate);
    voucherNumberWidget->setVariable(&voucher->voucherNumber);
    voucherNarrationWidget->setVariable(&voucher->narration);


}

void JournalVoucherEditor::initGlobalObjects()
{

    lis<<"#"<<"Ledgers"<<"Debit"<<"Credit";
    mainLedgerWidget->hide();
    DataTableWidget = new LedgervoucherTableView(voucher,3);

    DataTableWidget->setItemDelegate(new GMLedgerTransactionDelegate(
                                         ledgerHelper,ledgerHelper->getAllLedgersModel()));

    model = new GMJournalEntryModel(voucher,lis.size(),this);
    model->setHeaderStrings(lis);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();

    setTableLayout(DataTableWidget);

}

void JournalVoucherEditor::resetWidgets()
{
    VoucherEditorBase::resetWidgets();
    delete model;
    model = new GMJournalEntryModel(voucher,lis.size(),this);
    model->setHeaderStrings(lis);
    DataTableWidget->setVoucher(voucher);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
}

void JournalVoucherEditor::saveVoucher()
{
    if(voucher->ledgersList.size()<1){
        snackBar->addMessage("No ledgers Found!!!");
        return;
    }

//    if(voucher->crTotal != voucher->drTotal){
    double crTotal = (double)voucher->crTotal;
    double drTotal = (double)voucher->drTotal;

    qDebug()<<Q_FUNC_INFO<<__LINE__<<crTotal<<drTotal<<qFabs(crTotal - drTotal);

    if(qFabs(crTotal - drTotal) > 0) {
        snackBar->addMessage("Credit and Debit Totals Do Not Match");
        return;
    }

//    qDebug()<<"223"<<Q_FUNC_INFO<<voucher->ledgerObject.LedgerGroupId<<accHelper->getGroupIDByName("Bank Accounts");

    if((chqObj == nullptr) && (voucher->ledgerObject.LedgerGroupId == accHelper->getGroupIDByName("Bank Accounts"))){
        setChequeData();
    }
    else
    {
//        if(editorStatus == EditorStatus::NewVoucher)
            VoucherEditorBase::saveVoucher();
    }


    closeEditor();
}

void JournalVoucherEditor::printVoucher(QPrinter *printer)
{
    printHelper = new PrinterHelper();
    printHelper->setPrinter(printer);
    printHelper->PrintInventoryVoucher(voucher);
}

void JournalVoucherEditor::setChequeData()
{
    qDebug()<<Q_FUNC_INFO;
    if(voucher->ledgerObject.LedgerGroupId == accHelper->getGroupIDByName("Bank Accounts")){
        chqObj = new ChequeDetail();
        if(voucher->ledgersList.size()>2){
            snackBar->addMessage("More than one ledgers entered, items will be removed");
            while(voucher->ledgersList.size()>2)
                voucher->ledgersList.removeLast();
        }
        chqObj->LedgerID = voucher->ledgersList[0].LedgerID;
        chqObj->Amount = voucher->grandTotal;
        chqObj->crAmount = voucher->grandTotal;
        chqObj->VoucherID = voucher->voucherNumber;
        chqObj->VoucherType = voucher->voucherType;
        chqObj->VoucherPrefix = voucher->VoucherPrefix;
        chqObj->InstrumentDate = voucher->VoucherDate;
        voucher->chequeEntry = chqObj;

        chequeWidget = new ChequeWidget(voucher);
        chequeWidget->setWindowFlags(Qt::Window);
        chequeWidget->setAttribute(Qt::WA_DeleteOnClose);
//        chequeWidget->setVoucher(voucher);

        chequeWidget->show();
        QObject::connect(chequeWidget, SIGNAL(closing()), this, SLOT(saveVoucher()));

    }
}

void JournalVoucherEditor::sendEmail()
{

}

void JournalVoucherEditor::showVoucher()
{
    VoucherEditorBase::showVoucher();
    TransactionCalculator::VoucherCalculateLedgersTotal(voucher);
    resetWidgets();
}

