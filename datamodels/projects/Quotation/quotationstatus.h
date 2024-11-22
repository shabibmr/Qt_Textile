#ifndef QUOTATIONSTATUS_H
#define QUOTATIONSTATUS_H

#include "utils/editorstatus.h"

class QuotationStatus
{
public:
    QuotationStatus();

    enum Status{
        StartedEnum                 = 1,
        ManagerApprovedEnum         = 2,
        AwaitingClientApprovalEnum  = 3
    };
    static int Started;
    static int ManagerApproved;
    static int AwaitingClientApproval;
    static int ClientApprovedQuoatation;
    static int AccountsApprovedQuoatation;
    static int SentToDraftsMan;
    static int DraftsStarted;
    static int DraftsCompleted;
    static int DraftsApproved;
    static int SalesOrderCreated;
    static int SalesOrderDeliveryCreated;
    static int WorkOrderRaised;
    static int WorkOrderAssigned;
    static int WorkOrderWithheld;
    static int WorkOrderCompleted;
    static int PurchaseOrderRaised;
    static int DeliveryReady;
    static int DeliveryCompleted;
    static int DeliverySigned;
    static int SalesInvoiceRaised;
    static int QuotationDropped;
    static int QuotationPrepared;
    static int QuotationRevisionStarted;
    static int AccountsApprovedAquotation;
    static int Created;
    static int PurchaseInvoiceRaised;
    static int ReceiptReady;

    static int NotificationUnRead;
    static int NotificationAccepted;
    static int NotificationRejected;
    static int NotificationRead;
    static char* getStatusString(int id);
};

#endif // QUOTATIONSTATUS_H
