#ifndef FIFOCALCULATOR_H
#define FIFOCALCULATOR_H

#include "datamodels/transaction/itemfifodatamodel.h"
#include "database/finance/ItemTransaction/itemtransactiondatabasehelper.h"
#include "datamodels/projects/Quotation/quotationstatus.h"
#include <QMessageBox>
#include "customwidgets/Fifo/fifopopupdialog.h"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "database/finance/ItemTransaction/transactionfifodatabasehelper.h"


class FifoCalculator
{
public:
    FifoCalculator();

    QList<ItemFifoDataModel *> fifoList;
//    ItemTransactionDatabaseHelper *transHelper;
    TransactionFifoDatabaseHelper *transHelper;
    SalesInventoryItemDatabaseHelper *shelper;

    void setFifoData(GeneralVoucherDataObject *voucher,inventoryItemDataModel *item, QDate vdate, int index =0);
    void setItemQtyByFifo(GeneralVoucherDataObject *voucher, int row, int godownIndex = 0, bool showPopup = true);
    void setPriceInFifo(GeneralVoucherDataObject *voucher, int row);
    bool compareFifo(ItemFifoDataModel *fifo1, ItemFifoDataModel *fifo2);

};

#endif // FIFOCALCULATOR_H
