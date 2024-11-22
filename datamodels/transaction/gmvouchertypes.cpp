#include "gmvouchertypes.h"
#include <QDebug>


int GMVoucherTypes::SalesVoucher=100;
int GMVoucherTypes::SalesOrder=110;
int GMVoucherTypes::itemwiseOrder=111;

int GMVoucherTypes::PurchaseVoucher=120;
int GMVoucherTypes::PurchaseOrder=130;
int GMVoucherTypes::itemwisePurchaseOrder=131;
int GMVoucherTypes::DeliveryNote=140;
int GMVoucherTypes::ReceiptNote=150;
int GMVoucherTypes::WorkOrder=160;
int GMVoucherTypes::CreditNote=170;
int GMVoucherTypes::DebitNote=180;
int GMVoucherTypes::PaymentVoucher=190;
int GMVoucherTypes::ReceiptVoucher=200;
int GMVoucherTypes::JournalVoucher=210;
int GMVoucherTypes::ContraVoucher=220;
int GMVoucherTypes::StockEntry=230;
int GMVoucherTypes::StockJournal=240;
int GMVoucherTypes::GodownTransfer=250;
int GMVoucherTypes::MaterialIn=260;
int GMVoucherTypes::MaterialOut=270;
int GMVoucherTypes::Proforma=280;
int GMVoucherTypes::IndentRequest=290;
int GMVoucherTypes::IndentIssue=300;
int GMVoucherTypes::Quotation=310;
int GMVoucherTypes::DefectiveVoucher=320;
int GMVoucherTypes::TableReservation=330;

int GMVoucherTypes::ItemMaster=410;
int GMVoucherTypes::LedgerMaster=420;
int GMVoucherTypes::AccountGroupMaster=430;
int GMVoucherTypes::InventoryGroupMaster=440;
int GMVoucherTypes::GodownMaster=450;
int GMVoucherTypes::UomMaster=460;
int GMVoucherTypes::EmployeeMaster=470;

GMVoucherTypes::GMVoucherTypes()
{

}


QString GMVoucherTypes::getVoucherString(int id)
{
    QString a;
    switch (id) {
    case 100:
    {
        a="SalesVoucher";
        break;
    }
    case 110:
    {
        a="SalesOrder";
        break;
    }
    case 111:
    {
        a="ItemwiseOrder";
        break;
    }
    case 120:
    {
        a="PurchaseVoucher";
        break;
    }
    case 130:
    {
        a="PurchaseOrder";
        break;
    }
    case 131:
    {
        a="ItemwisePurchaseOrder";
        break;
    }
    case 140:
    {
        a="DeliveryNote";
        break;
    }
    case 150:
    {
        a="ReceiptNote";
        break;
    }
    case 160:
    {
        a="Work Order";
        break;
    }
    case 170:
    {
        a="CreditNote";
        break;
    }
    case 180:
    {
        a="DebitNote";
        break;
    }
    case 190:
    {
        a="PaymentVoucher";
        break;
    }
    case 200:
    {
        a="ReceiptVoucher";
        break;
    }
    case 210:
    {
        a="JournalVoucher";
        break;
    }
    case 220:
    {
        a="ContraVoucher";
        break;
    }
    case 230:
    {
        a="Stock";
        break;
    }
    case 240:
    {
        a="StockJournal";
        break;
    }
    case 250:
    {
        a="GodownTransfer";
        break;
    }
    case 260:
    {
        a="MaterialIn";
        break;
    }
    case 270:
    {
        a="MaterialOut";
        break;
    }
    case 280:
    {
        a="Proforma";
        break;
    }
    case 290:
    {
        a="Stock Request";
        break;
    }
    case 300:
    {
        a="IndentIssue";
        break;
    }
    case 310:
    {
        a="Quotation";
        break;
    }
    case 320:
    {
        a="WastageEntry";
        break;
    }
    case 330:
    {
        a="TableReservation";
        break;
    }

    case 410:{
        a="ItemMaster";
        break;
    }
    case 420:{
        a="LedgerMaster";
        break;
    }
    case 430:{
        a="AccountGroupMaster";
        break;
    }
    case 440:{
        a="InventoryGroupMaster";
        break;
    }
    case 450:{
        a="GodownMaster";
        break;
    }
    case 460:{
        a="UomMaster";
        break;
    }
    case 470:{
        a="EmployeeMaster";
        break;
    }

    }
    return  a;
}


QString GMVoucherTypes::getVoucherTitle(int id)
{
    QString a;
    switch (id) {
    case 100:
    {
        a="Sales Voucher";
        break;
    }
    case 110:
    {
        a="Sales Order";
        break;
    }
    case 111:
    {
        a="Itemwise Order";
        break;
    }
    case 120:
    {
        a="Purchase Voucher";
        break;
    }
    case 130:
    {
        a="Purchase Order";
        break;
    }
    case 131:
    {
        a="Itemwise Purchase Order";
        break;
    }
    case 140:
    {
        a="Delivery Note";
        break;
    }
    case 150:
    {
        a="Receipt Note";
        break;
    }
    case 160:
    {
        a="Work Order";
        break;
    }
    case 170:
    {
        a="Credit Note";
        break;
    }
    case 180:
    {
        a="Debit Note";
        break;
    }
    case 190:
    {
        a="Payment Voucher";
        break;
    }
    case 200:
    {
        a="Receipt Voucher";
        break;
    }
    case 210:
    {
        a="Journal Voucher";
        break;
    }
    case 220:
    {
        a="Contra Voucher";
        break;
    }
    case 230:
    {
        a="Stock Entry";
        break;
    }
    case 240:
    {
        a="Stock Journal";
        break;
    }
    case 250:
    {
        a="Godown Transfer";
        break;
    }
    case 260:
    {
        a="Material In";
        break;
    }
    case 270:
    {
        a="Material Out";
        break;
    }
    case 280:
    {
        a="Proforma";
        break;
    }
    case 290:
    {
        a="Stock Request";
        break;
    }
    case 300:
    {
        a="Indent Issue";
        break;
    }
    case 310:
    {
        a="Quotation";
        break;
    }
    case 320:
    {
        a="Wastage Entry";
        break;
    }
    case 330:
    {
        a="Table Reservation";
        break;
    }

    case 410:{
        a="Item Master";
        break;
    }
    case 420:{
        a="Ledger Master";
        break;
    }
    case 430:{
        a="Account Group Master";
        break;
    }
    case 440:{
        a="Inventory Group Master";
        break;
    }
    case 450:{
        a="Godown Master";
        break;
    }
    case 460:{
        a="Uom Master";
        break;
    }
    case 470:{
        a="Employee Master";
        break;
    }

    }
    return  a;
}

bool GMVoucherTypes::compareVoucherType(QString voucherStr, int voucherType)
{
//    qDebug()<<Q_FUNC_INFO<<__LINE__<<getVoucherString(voucherType);
    return (!QString::compare(voucherStr,
                         getVoucherString(voucherType),
                         Qt::CaseInsensitive));

}

QString GMVoucherTypes::getVoucherMainTablename(int id)
{
    QString a;
    switch (id) {
    case 100:
    {
        a="SALES_INVOICE_MAIN";
        break;
    }
    case 110:
    {
        a="SALES_ORDER_MAIN";
        break;
    }
    case 111:
    {
        a="SALES_ORDER_ITEMWISE";
        break;
    }
    case 120:
    {
        a="PURCHASE_INVOICE_MAIN";
        break;
    }
    case 130:
    {
        a="PURCHASE_ORDER_MAIN";
        break;
    }
    case 140:
    {
        a="DELIVERY_NOTE_MAIN";
        break;
    }
    case 150:
    {
        a="RECEIPT_NOTE_MAIN";
        break;
    }
    case 160:
    {
        a="WORK_ORDER_MAIN";
        break;
    }
    case 170:
    {
        a="CREDIT_NOTE_MAIN";
        break;
    }
    case 180:
    {
        a="DEBIT_NOTE_MAIN";
        break;
    }
    case 190:
    {
        a="PAYMENT_VOUCHER_MAIN";
        break;
    }
    case 200:
    {
        a="RECEIPT_VOUCHER_MAIN";
        break;
    }
    case 210:
    {
        a="JOURNAL_VOUCHER_MAIN";
        break;
    }
    case 220:
    {
        a="CONTRA_VOUCHER_MAIN";
        break;
    }
    case 230:
    {
        a="STOCK_MAIN";
        break;
    }
    case 240:
    {
        a="STOCK_JOURNAL_MAIN";
        break;
    }
    case 250:
    {
        a="GODOWN_TRANSFER_MAIN";
        break;
    }
    case 260:
    {
        a="MATERIAL_IN_MAIN";
        break;
    }
    case 270:
    {
        a="MATERIAL_OUT_MAIN";
        break;
    }
    case 280:
    {
        a="Proforma";
        break;
    }
    case 290:
    {
        a="IndentRequest";
        break;
    }
    case 300:
    {
        a="IndentIssue";
        break;
    }
    case 310:
    {
        a="QUOTATION_MAIN";
        break;
    }
    case 320:
    {
        a="WASTAGE_ENTRY_MAIN";
        break;
    }


    }
    return  a;
}

QString GMVoucherTypes::getVoucherDetailsTablename(int id)
{
    QString a;
    switch (id) {
    case 100:
    {
        a="SALES_INVOICE_DETAILS";
        break;
    }
    case 110:
    {
        a="SALES_ORDER_DETAILS";
        break;
    }
    case 111:
    {
        a="SALES_ORDER_ITEMWISE";
        break;
    }
    case 120:
    {
        a="PURCHASE_INVOICE_DETAILS";
        break;
    }
    case 130:
    {
        a="PURCHASE_ORDER_DETAILS";
        break;
    }
    case 140:
    {
        a="DELIVERY_NOTE_DETAILS";
        break;
    }
    case 150:
    {
        a="RECEIPT_NOTE_DETAILS";
        break;
    }
    case 160:
    {
        a="WORK_ORDER_DETAIL";
        break;
    }
    case 170:
    {
        a="CREDIT_NOTE_DETAILS";
        break;
    }
    case 180:
    {
        a="DEBIT_NOTE_DETAILS";
        break;
    }
    case 190:
    {
        a="PAYMENT_VOUCHER_DETAILS";
        break;
    }
    case 200:
    {
        a="RECEIPT_VOUCHER_DETAILS";
        break;
    }
    case 210:
    {
        a="JOURNAL_VOUCHER_DETAILS";
        break;
    }
    case 220:
    {
        a="CONTRA_VOUCHER_DETAILS";
        break;
    }
    case 230:
    {
        a="STOCK_DETAILS";
        break;
    }
    case 240:
    {
        a="STOCK_JOURNAL_DETAILS";
        break;
    }
    case 250:
    {
        a="GODOWN_TRANSFER_DETAILS";
        break;
    }
    case 260:
    {
        a="MATERIAL_IN_DETAILS";
        break;
    }
    case 270:
    {
        a="MATERIAL_OUT_DETAILS";
        break;
    }
    case 280:
    {
        a="Proforma_details";
        break;
    }
    case 290:
    {
        a="Stock_Request_Details";
        break;
    }
    case 300:
    {
        a="Stock_Request_Details";
        break;
    }
    case 310:
    {
        a="QUOTATION_DETAILS";
        break;
    }
    case 320:
    {
        a="WASTAGE_ENTRY_DETAILS";
        break;
    }


    }
    return  a;

}

int GMVoucherTypes::getTransactionType(int id)
{
    qDebug()<<Q_FUNC_INFO<<id;
    int a = 0;
    switch (id) {
    case 100:case 110:case 111:case 140:case 180:case 320:case 280:
    {
        a=1;
        break;
    }

    case 120:case 130:case 150:case 170:
    {
        a=2;
        break;
    }

    case 240:
    {
        a=3;
        break;
    }
    case 230:
    {
        a=4;
        break;
    }

    }

    return a;
}

int GMVoucherTypes::getVoucherIdFromString(QString voucherType)
{
    qDebug()<<Q_FUNC_INFO<<voucherType;
    if(!QString::compare(voucherType, GMVoucherTypes::getVoucherString(SalesVoucher), Qt::CaseInsensitive))
        return SalesVoucher;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(PurchaseVoucher), Qt::CaseInsensitive))
            return PurchaseVoucher;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(SalesOrder), Qt::CaseInsensitive))
            return SalesOrder;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(PurchaseOrder), Qt::CaseInsensitive))
            return PurchaseOrder;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(itemwiseOrder), Qt::CaseInsensitive))
            return itemwiseOrder;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(DeliveryNote), Qt::CaseInsensitive))
            return DeliveryNote;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(ReceiptNote), Qt::CaseInsensitive))
            return ReceiptNote;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(WorkOrder), Qt::CaseInsensitive))
            return WorkOrder;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(CreditNote), Qt::CaseInsensitive))
            return CreditNote;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(DebitNote), Qt::CaseInsensitive))
            return DebitNote;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(PaymentVoucher), Qt::CaseInsensitive))
            return PaymentVoucher;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(ReceiptVoucher), Qt::CaseInsensitive))
            return ReceiptVoucher;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(JournalVoucher), Qt::CaseInsensitive))
            return JournalVoucher;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(ContraVoucher), Qt::CaseInsensitive))
            return ContraVoucher;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(StockEntry), Qt::CaseInsensitive)){
        qDebug()<<"voucher id is"<<StockEntry;
        return StockEntry;
    }
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(StockJournal), Qt::CaseInsensitive))
            return StockJournal;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(GodownTransfer), Qt::CaseInsensitive))
            return GodownTransfer;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(MaterialIn), Qt::CaseInsensitive))
            return MaterialIn;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(MaterialOut), Qt::CaseInsensitive))
            return MaterialOut;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(Proforma), Qt::CaseInsensitive))
            return Proforma;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(IndentIssue), Qt::CaseInsensitive))
            return IndentIssue;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(IndentRequest), Qt::CaseInsensitive))
            return IndentRequest;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(Quotation), Qt::CaseInsensitive))
            return Quotation;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(DefectiveVoucher), Qt::CaseInsensitive))
            return DefectiveVoucher;
    else if (!QString::compare(voucherType, GMVoucherTypes::getVoucherString(TableReservation), Qt::CaseInsensitive))
        return TableReservation;
}
