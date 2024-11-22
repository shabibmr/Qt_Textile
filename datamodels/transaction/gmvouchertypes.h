#ifndef GMVOUCHERTYPES_H
#define GMVOUCHERTYPES_H

#include <QString>

class GMVoucherTypes
{
public:

    static int SalesVoucher;
    static int SalesOrder;
    static int itemwiseOrder;
    static int PurchaseVoucher;
    static int PurchaseOrder;
    static int itemwisePurchaseOrder;
    static int DeliveryNote;
    static int ReceiptNote;
    static int WorkOrder;
    static int CreditNote;
    static int DebitNote;
    static int PaymentVoucher;
    static int ReceiptVoucher;
    static int JournalVoucher;
    static int ContraVoucher;
    static int StockEntry;
    static int StockJournal;
    static int GodownTransfer;
    static int MaterialIn;
    static int MaterialOut;
    static int Proforma;
    static int IndentRequest;
    static int IndentIssue;
    static int Quotation;
    static int DefectiveVoucher;
    static int TableReservation;

    static int ItemMaster;
    static int LedgerMaster;
    static int AccountGroupMaster;
    static int InventoryGroupMaster;
    static int GodownMaster;
    static int UomMaster;
    static int EmployeeMaster;

    GMVoucherTypes();
    static QString getVoucherString(int id);
    static QString getVoucherMainTablename(int id);
    static QString getVoucherDetailsTablename(int id);
    static int getTransactionType(int id);
    static int getVoucherIdFromString(QString voucherType);

    static QString getVoucherTitle(int id);
    static bool compareVoucherType(QString voucherStr, int voucherType);
};

#endif // GMVOUCHERTYPES_H
