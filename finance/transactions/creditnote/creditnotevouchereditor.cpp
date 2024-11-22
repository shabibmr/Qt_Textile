#include "creditnotevouchereditor.h"


CreditNoteVoucherEditor::CreditNoteVoucherEditor(CreditNoteVoucherDatabaseHelper *db, InventoryLedgerTransactionsEditor *parent) :
    InventoryLedgerTransactionsEditor(GMVoucherTypes::CreditNote, db, parent)
{
    this->dbHelper = db;
}
