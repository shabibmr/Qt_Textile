#include "quotationstatus.h"

int QuotationStatus::Started=10;
int QuotationStatus::QuotationPrepared=20;
int QuotationStatus::QuotationRevisionStarted=25;
int QuotationStatus::ManagerApproved=30;
int QuotationStatus::AwaitingClientApproval=40;
int QuotationStatus::ClientApprovedQuoatation=50;
int QuotationStatus::AccountsApprovedQuoatation=55;
int QuotationStatus::SentToDraftsMan=60;
int QuotationStatus::DraftsStarted=70;
int QuotationStatus::DraftsCompleted=80;
int QuotationStatus::DraftsApproved=90;
int QuotationStatus::AccountsApprovedAquotation=100;

int QuotationStatus::SalesOrderCreated=110;
int QuotationStatus::SalesOrderDeliveryCreated=115;

int QuotationStatus::WorkOrderRaised=120;
int QuotationStatus::WorkOrderAssigned=125;
int QuotationStatus::WorkOrderWithheld=127;
int QuotationStatus::WorkOrderCompleted=130;

int QuotationStatus::PurchaseOrderRaised = 140;
int QuotationStatus::DeliveryReady = 150;
int QuotationStatus::DeliveryCompleted = 160;
int QuotationStatus::DeliverySigned = 165;
int QuotationStatus::SalesInvoiceRaised = 170;
int QuotationStatus::PurchaseInvoiceRaised = 180;
int QuotationStatus::ReceiptReady = 190;
int QuotationStatus::QuotationDropped = 300;


int QuotationStatus::Created = 1000;

int QuotationStatus::NotificationUnRead=0;
int QuotationStatus::NotificationAccepted=2000;
int QuotationStatus::NotificationRejected=2010;
int QuotationStatus::NotificationRead=2001;

QuotationStatus::QuotationStatus()
{

}

char* QuotationStatus::getStatusString(int id)
{
    char* a;
    switch (id) {
    case 10:
    {
        a="IN PROGRESS";
        break;
    }
    case 20:
    {
        a="Quotation Prepared";
        break;
    }
    case 25:
    {
        a="Quotation Revised";
        break;
    }
    case 30:
    {
        a="Manager Approved";
        break;
    }
    case 40:
    {
        a="Awaiting Client Approval";
        break;
    }
    case 50:
    {
        a="Client Approved Quotation";
        break;
    }
    case 55 :
    {
        a="Accounts Approved Quotation";
        break;
    }
    case 60:
    {
        a="Sent To Drafts Man";
        break;
    }
    case 70:
    {
        a="Drawing Started";
        break;
    }
    case 80:
    {
        a="Drawing Completed";
        break;
    }
    case 90:
    {
        a="Client Approved Drawing";
        break;
    }
    case 100:
    {
        a="Drafts Approved";
        break;
    }
    case 110:
    {
        a="Current Order";
        break;
    }
    case 115:
    {
        a="Advance Booking";
        break;
    }
    case 120:
    {
        a="Work Order Raised";
        break;
    }
    case 125:
    {
        a= "Work Order Asssigned";
        break;
    }
    case 127:
    {
        a= "Work Order Withheld";
        break;
    }
    case 130:
    {
        a="Work Order Completed";
        break;
    }
    case 140:
    {
        a="Purchase Order Raised";
        break;
    }
    case 150:
    {
        a="Delivery Ready";
        break;
    }
    case 160:
    {
        a="Delivery Completed";
        break;
    }
    case 165:
    {
        a="Delivery Signed";
        break;
    }
    case 170:
    {
        a="Sales Invoice Raised";
        break;
    }
    case 300:
    {
        a="Quotation Dropped";
        break;
    }
    case 1000:
    {
        a= "Created";
        break;
    }

    case 2000:
    {
        a = "Accepted";
        break;
    }

    case 2010:
    {
        a = "Rejected";
        break;
    }
    case 0:
    {
        a = "New";
        break;
    }
    case 2001:
    {
        a = "Ignored";
        break;
    }
    default:
    {
        a = "~~~";
    }
    }
    return a;
}
