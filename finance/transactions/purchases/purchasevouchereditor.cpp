#include "purchasevouchereditor.h"
//#include "ui_purchasevouchereditor.h"

#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"
#include "counter/MainScreen/itemdetail.h"


PurchaseVoucherEditor::PurchaseVoucherEditor(VoucherDBAbstract *db, InventoryLedgerTransactionsEditor *parent) :
    InventoryLedgerTransactionsEditor(GMVoucherTypes::PurchaseVoucher, db, parent)
{

    setTabs();

}

PurchaseVoucherEditor::~PurchaseVoucherEditor()
{
}

void PurchaseVoucherEditor::setTabs()
{
//    this->setTabOrder(mainLedgerWidget,SalesmanSearchBox);
//    this->setTabOrder(SalesmanSearchBox,DataTableWidget);

//    mainLedgerWidget->setFocus();
//    qDebug()<<"tab Order Set";

}


void PurchaseVoucherEditor::saveVoucher()
{
    if(ConfigurationSettingsDatabaseHelper::getValue(promptItemChecklist,false).toBool()){
        if(!itemsChecked){
            showItemChecklistDialog();
            return;
        }

    }

    if(editorStatus == EditorStatus::NewVoucher)
        voucher->status = QuotationStatus::PurchaseInvoiceRaised;
    InventoryLedgerTransactionsEditor::saveVoucher();

}

//void PurchaseVoucherEditor::askForPrint()
//{
//    QMessageBox::StandardButton reply;
//    reply = QMessageBox::question(this,"Print Voucher","Would You Like to Print?",
//                                  QMessageBox::Yes|QMessageBox::No);
//    if(reply==QMessageBox::Yes){
//        printVoucher();
//    }
//}

void PurchaseVoucherEditor::printVoucher()
{
    printHelper = new PrinterHelper();
    printHelper->PrintInventoryPurchaseVoucher(voucher);
}

void PurchaseVoucherEditor::setBillwiseMapping()
{
    qDebug()<<Q_FUNC_INFO<<voucher->ledgerObject.mapList.size()<<voucher->mapList.size();
    BillwiseRecordDataModel* bill = new BillwiseRecordDataModel;
    bill->VoucherNo = voucher->voucherNumber;
    bill->VoucherDate = voucher->VoucherDate;
    bill->VoucherType = voucher->voucherType;
    bill->VoucherPrefix = voucher->VoucherPrefix;
    bill->LedgerID = voucher->ledgerObject.LedgerID;
    bill->DebitAmount = 0;
    bill->CreditAmount = voucher->grandTotal;
    bill->DueDate = voucher->VoucherDate.addDays(voucher->ledgerObject.Credit_Period);

    voucher->ledgerObject.mapList.append(bill);
    voucher->mapList.append(bill);
}


void PurchaseVoucherEditor::sendEmail()
{

}


void PurchaseVoucherEditor::showItemChecklistDialog()
{

    ItemCheckListWidget *checkList = new ItemCheckListWidget(voucher);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    checkList->show();

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    connect(checkList, &ItemCheckListWidget::checkCompleted, this, [=](bool val){
        itemsChecked = val;
        saveVoucher();
    });

}

