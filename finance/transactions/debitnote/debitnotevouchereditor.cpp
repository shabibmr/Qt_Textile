#include "debitnotevouchereditor.h"
//#include "ui_debitnotevouchereditor.h"


DebitNoteVoucherEditor::DebitNoteVoucherEditor(VoucherDBAbstract *dbhelper, InventoryLedgerTransactionsEditor *parent) :
    InventoryLedgerTransactionsEditor (GMVoucherTypes::DebitNote, dbhelper,parent)
{

}

void DebitNoteVoucherEditor::saveVoucher()
{
    if(editorStatus == EditorStatus::NewVoucher)
        voucher->status = QuotationStatus::SalesInvoiceRaised;
    InventoryLedgerTransactionsEditor::saveVoucher();
}


void DebitNoteVoucherEditor::printVoucher(QPrinter *printer)
{
    printHelper = new PrinterHelper();
    printHelper->PrintInventoryVoucher(voucher);
}
