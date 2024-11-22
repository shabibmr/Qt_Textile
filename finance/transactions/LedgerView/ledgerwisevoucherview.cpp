#include "ledgerwisevoucherview.h"

LedgerwiseVoucherView::LedgerwiseVoucherView(GeneralVoucherDataObject *v,
                                             LedgerMasterDatabaseHelper *ledHelper,
                                             QWidget *parent) : QWidget(parent)
{
    this->voucher = v;
    ledgerHelper = ledHelper;
}

LedgerwiseVoucherView::~LedgerwiseVoucherView()
{

}

void LedgerwiseVoucherView::initGlobalObjects()
{
    lis<<"#"<<"Ledgers"<<"Debit"<<"Credit";
    DataTableWidget = new LedgervoucherTableView(voucher,3);
    DataTableWidget->setItemDelegate(new GMLedgerTransactionDelegate(
                                         ledgerHelper,ledgerHelper->getAllLedgersModel()));
    model = new GMLedgerItemModel(voucher,lis.size(),this);
    model->setHeaderStrings(lis);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
}
