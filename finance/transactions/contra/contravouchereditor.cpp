#include "contravouchereditor.h"

ContraVoucherEditor::ContraVoucherEditor(VoucherDBAbstract *db, QWidget *parent) :
    VoucherEditorBase(parent)
{

    this->setWindowTitle(windowTitle);
    this->setObjectName("background");
    this->setStyleSheet("QWidget#background{background-color:#e0f7fa}");

    initVoucher();
    initGlobalObjects();
    initExtraActions();
}

void ContraVoucherEditor::initExtraActions()
{
}

void ContraVoucherEditor::initVoucher()
{
    editorStatus = EditorStatus::NewVoucher;

    voucher = new GeneralVoucherDataObject();
    voucher->VoucherDate = QDate::currentDate();
    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::ContraVoucher);
    voucher->voucherNumber = dbHelper->getNextVoucherNo(voucher->VoucherPrefix);
    voucher->status = QuotationStatus::Started;

    setTitle(GMVoucherTypes::getVoucherTitle(GMVoucherTypes::ContraVoucher));
    voucherDateWidget->setVariable(&voucher->VoucherDate);
    voucherNumberWidget->setVariable(&voucher->voucherNumber);
}

void ContraVoucherEditor::initGlobalObjects()
{

    lis<<"#"<<"Ledgers"<<"Amount";
    DataTableWidget = new LedgervoucherTableView(voucher,1);
    DataTableWidget->setItemDelegate(new GMLedgerTransactionDelegate(
                              ledgerHelper,ledgerHelper->getAllLedgersModel()));

    model = new GMPaymentEntryModel(voucher,lis.size(),this);

    model->setHeaderStrings(lis);
    DataTableWidget->setAvlcols(avlcols);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
    DataTableWidget->hideColumn(3);

    setTableLayout(DataTableWidget);
}

void ContraVoucherEditor::resetWidgets()
{
    delete model;

    model = new GMPaymentEntryModel(voucher,lis.size(),this);
    model->setHeaderStrings(lis);
    DataTableWidget->setVoucher(voucher);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();

}

void ContraVoucherEditor::saveVoucher()
{
    if(voucher->ledgersList.size()<1){
        snackBar->addMessage("No Items Found!!!");
        return;
    }

    if(voucher->ledgerObject.LedgerID.length()<1){
        snackBar->addMessage("Please Select Contra Mode");
        return;
    }

    BillwiseRecordDataModel* bill = new BillwiseRecordDataModel;
    bill->VoucherNo = voucher->voucherNumber;
    bill->VoucherDate = voucher->VoucherDate;
    bill->VoucherType = voucher->voucherType;
    bill->VoucherPrefix = voucher->VoucherPrefix;
    bill->LedgerID = voucher->ledgerObject.LedgerID;
    bill->DebitAmount = voucher->grandTotal;
    bill->CreditAmount = 0;
    bill->DueDate = voucher->VoucherDate.addDays(120);

    voucher->ledgerObject.mapList.append(bill);
    voucher->mapList.append(bill);

    if(voucher->status == QuotationStatus::Started){
        voucher->status = QuotationStatus::SalesInvoiceRaised;
        qDebug()<<"Save voucher";
        dbHelper->insertVoucher(voucher);
    }
    else{
        dbHelper->updateVoucher(voucher,voucher->voucherNumber);
    }

    closeEditor();
}

void ContraVoucherEditor::sendEmail()
{


}

void ContraVoucherEditor::showVoucher()
{
    VoucherEditorBase::showVoucher();
//    calcTotals();
    TransactionCalculator::VoucherCalculateLedgersTotal(voucher);

    resetWidgets();
}

void ContraVoucherEditor::printVoucher(QPrinter *printer)
{
    CounterPrinterHelper *printHelper = new CounterPrinterHelper();
    printHelper->printPaymentVoucher(voucher);
}
