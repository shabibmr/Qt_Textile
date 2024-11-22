#ifndef PREVIOUSQUOTATIONWIDGET_H
#define PREVIOUSQUOTATIONWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QPointer>
#include <QKeyEvent>
#include <QDateEdit>
#include <QStandardItemModel>

#include "login/loginvalues.h"
#include "database/hr/userprofiledatabasehelper.h"
#include <QMessageBox>
#include <QFormLayout>

#include "projects/requirements/additemadditionalrequirement.h"
#include "projects/requirements/additemrequirement.h"
#include "projects/quotations/quotationdetails.h"

#include "finance/masters/ledgers/addledger.h"

#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include <QPointer>

#include "sharetools/print/printerhelper.h"
#include "sharetools/email/email.h"
#include "sharetools/excel/excel.h"


#include "database/projects/oldquote/oldquotationdatabasehelper.h"



#include "datamodels/addressbook/addressbooksmall.h"
#include "database/addressbook/addressbookdatabasehelper.h"

#include "customwidgets/ContactSearch/gmcontactsearch.h"

namespace Ui {
class PreviousQuotationWidget;
}

class PreviousQuotationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PreviousQuotationWidget(QWidget *parent = 0);
    ~PreviousQuotationWidget();
	  //  widgets
    addItemRequirement *addItemRequirementWidget;
    addItemAdditionalRequirement *addItemAdditionalRequirementWidget;

    QuotationDetails* QuotationDetailsWidget;

    addLedger* addLedgerWidget;

    //  common
    QString projectID="";
    QString vNo="";
    QString vPrefix ="";
//    QToolButton *exportButton;
    int SalesManId=0;

//    QToolButton *managerApprovalButton;
//    QToolButton *clientApprovalButton;

//    QToolButton *reviseButton;

    QToolButton *printButton;
    QCheckBox *checkbox ;
    QDialog* approvalDialog;
    QLineEdit* LPOref;
    QDateEdit* tentDelivery;

    QString LPOString="";

    //  databases

    OldQuotationDatabaseHelper *dbHelper;
//    RequirementsDatabaseHelper *requirementsDatabaseHelper;
    SalesInventoryItemDatabaseHelper *itemsHelper;
    LedgerMasterDatabaseHelper *ledgerHelper;
    AddressBookDatabaseHelper *addressBookHelper;
    UserProfileDatabaseHelper *userHelper;

    OldQuotationDatabaseHelper *oldQuoteHelper;

    //  item specific

    QList<QSharedPointer<inventoryItemDataModel>> itemsListPtr;

    QList<LedgerMasterDataModel> allLedgers;

    QList<CompoundItemDataObject> ordersList;

    QMap<int,QString> salesmanMap;

    QStringList NamesForCompleter;
    QStringList itemNamesForLineEdit;

    bool openNewWidget = true;
    int userID=0;
    QString Username = "";


    //  ledger specific
    QList <LedgerMasterDataModel> ordersLedList;
    QList<QSharedPointer<LedgerMasterDataModel>> allLedgersPtr ;
    QList<QSharedPointer<AddressBookSmall>> addresses;
    bool openNewWidgetLedger = true;
    //    QPointer<GMLedgerSearch> customerName;

    QPointer<GMContactSearch> contactsWidget;

    GeneralVoucherDataObject objSalesOrder;

    GeneralVoucherDataObject *oldQuote;
    int revisionNumber=0;

    LedgerMasterDataModel CustomerName;

    AddressBookDataModel CustomerContact;


    PrinterHelper* pHelper;
    email* emailHelper;
    Excel* excelHelper;

private slots:
//    void on_addItemPushButton_clicked();
    void keyPressEvent(QKeyEvent *e);
//    void on_buttonBox_accepted();
//    void on_buttonBox_rejected();
    void on_itemTableWidget_doubleClicked(const QModelIndex &index);
    void on_addAdditionalPushButton_clicked();

    void on_additionalTableWidget_doubleClicked(const QModelIndex &index);

    void on_voucherDateEdit_editingFinished();

//    void on_ReviseButton_clicked();

public slots:
//    void setTitle(QString Text);
    void setProjectID(QString projID);
    void showVoucher(GeneralVoucherDataObject voucher);

//    void importToQuotation(GeneralVoucherDataObject voucher);
//    void importMultipleVouchers(QList<GeneralVoucherDataObject> vouchers);

    void setItemTable();
    void getItemDetails(inventoryItemDataModel obj);
    void appendItem(CompoundItemDataObject item,int row);
//    void delButtonClicked(bool);
    void calcTotal();
    void resetFlag(int);

    void setLedgerTable();
    void getLedgerDetails(LedgerMasterDataModel led);
    void updateLedgerValue();
//    void deleteLedgerButtonClicked(bool);
    void appendLedger(LedgerMasterDataModel item,int row);

    void resetLedgerWidgetFlag();

//    void clientApprovalButtonClicked();
//    void changeClientApprovalStatus(bool);
//    void setCustomerName(LedgerMasterDataModel);

    void setCustomerContact(AddressBookDataModel x);

//    void saveVoucher(GeneralVoucherDataObject*);
//    void exportToSalesOrder();

//    void openSalesOrder();


    void getQuoteDetails(QString,QString,QString,QString,QString,QString,QString,
                         bool ,bool,bool );

    void printQuotation();

    void printVoucher(QString validity,QString subject,QString payment,
                      QString warranty,QString extra,QString delivery,QString exclusion,
                      bool printExtra,bool printNow,bool emailNow);

//    void reviseQuote(GeneralVoucherDataObject obj);

//    void disableSaveButton(bool);

//    void reviseButtonClicked(bool);

signals:
    void closing();


private:
    Ui::PreviousQuotationWidget *ui;
};

#endif // PREVIOUSQUOTATIONWIDGET_H
