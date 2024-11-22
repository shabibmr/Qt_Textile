#ifndef ADDQUOTATION_H
#define ADDQUOTATION_H

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
#include "quotationdetails.h"
#include "finance/masters/ledgers/addledger.h"

#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include <QPointer>
#include <QToolButton>

#include "sharetools/print/printerhelper.h"
#include "sharetools/email/email.h"
#include "sharetools/excel/excel.h"

#include "database/inventory/quotation/quotationdatabasehelper.h"
#include "database/projects/requirementsdatabasehelper.h"

#include "database/projects/oldquote/oldquotationdatabasehelper.h"

//#include "inventory/transactions/salesorders/addsalesorder.h"

#include "datamodels/addressbook/addressbooksmall.h"
#include "database/addressbook/addressbookdatabasehelper.h"

#include "customwidgets/ContactSearch/gmcontactsearch.h"


namespace Ui {
class addQuotation;
}

class addQuotation : public QWidget
{
    Q_OBJECT

public:
    explicit addQuotation(QWidget *parent = 0);
    ~addQuotation();
    //  widgets
    addItemRequirement *addItemRequirementWidget;
    addItemAdditionalRequirement *addItemAdditionalRequirementWidget;

//    AddSalesOrder* AddSalesOrderWidget;
    QuotationDetails* QuotationDetailsWidget;

    addLedger* addLedgerWidget;

    float discPercent =0;
    float discAmount =0;

    //  common
    QString projectID="";
    QString vNo="";
    QString vPrefix ="";
    QToolButton *exportButton;
    QToolButton *AccountsApproval;
    int SalesManId=0;
    int GlobalStatus=0;


    QWidget *SearchWid ;
    QToolButton *managerApprovalButton;
    QToolButton *clientApprovalButton;

    QToolButton *reviseButton;

    QToolButton *printButton;
    QCheckBox *checkbox ;
    QDialog* approvalDialog;
    QLineEdit* LPOref;
    QDateEdit* tentDelivery;
    QLineEdit* Attachment;
    QLineEdit* Remarks;

    QString LPOString="";

    //  databases

    QuotationDatabaseHelper *dbHelper;
    RequirementsDatabaseHelper *requirementsDatabaseHelper;
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

    GeneralVoucherDataObject *voucher;

    GeneralVoucherDataObject objSalesOrder;



    GeneralVoucherDataObject *oldQuote;
    int revisionNumber=0;

    LedgerMasterDataModel CustomerName;

    AddressBookDataModel CustomerContact;


    PrinterHelper* pHelper;
    email* emailHelper;
    Excel* excelHelper;
    QList<DocumentsDataModel> docObj;

    void setFilesTable();
private slots:
    void on_addItemPushButton_clicked();
    void keyPressEvent(QKeyEvent *e);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_itemTableWidget_doubleClicked(const QModelIndex &index);
    void on_addAdditionalPushButton_clicked();

    void on_additionalTableWidget_doubleClicked(const QModelIndex &index);

    void on_voucherDateEdit_editingFinished();

    void on_ReviseButton_clicked();


    void on_discountAmountLineEdit_textChanged(const QString &arg1);

    void on_discountPercentLineEdit_textChanged(const QString &arg1);

    void on_itemTableWidget_clicked(const QModelIndex &index);

    void on_discountPercentLineEdit_editingFinished();

    void on_discountAmountLineEdit_editingFinished();

    void on_itemTableWidget_itemSelectionChanged();

    void on_browseButton_clicked();

    void on_UploadButton_clicked();

public slots:
    void setTitle(QString Text);
    void setProjectID(QString projID);
    void showVoucher(GeneralVoucherDataObject voucher);

    void importToQuotation(GeneralVoucherDataObject voucher);
    void importMultipleVouchers(QList<GeneralVoucherDataObject> vouchers);

    void setItemTable();
    void getItemDetails(inventoryItemDataModel obj);
    void appendItem(CompoundItemDataObject item,int row);
    void delButtonClicked(bool);
    void calcTotal();
    void resetFlag(int);

    void setLedgerTable();
    void getLedgerDetails(LedgerMasterDataModel led);
    void updateLedgerValue();
    void deleteLedgerButtonClicked(bool);
    void appendLedger(LedgerMasterDataModel item,int row);

    void resetLedgerWidgetFlag();

    void clientApprovalButtonClicked();
    void changeClientApprovalStatus(bool);
    void setCustomerName(LedgerMasterDataModel);

    void managerApprovalButtonClicked(bool);
    void accountsApprovalButtonClicked(bool);

    void setCustomerContact(AddressBookDataModel x);

    void saveVoucher(GeneralVoucherDataObject*);
    void exportToSalesOrder();
    void openSalesOrder();
    void getQuoteDetails(QString,QString,QString,QString,QString,QString,QString,
                         bool ,bool,bool,bool );

    void printQuotation();

    void printVoucher(QString validity,QString subject,QString payment,
                      QString warranty,QString extra,QString delivery,QString exclusion,
                      bool printExtra,bool printNow,bool emailNow,bool pdf);

    void reviseQuote(GeneralVoucherDataObject obj);

    void disableSaveButton(bool);

    void reviseButtonClicked(bool);
    void browseClicked(bool);

    void discountAmountEntered();
    void discountPercentEntered();
    void downloadClicked(bool v);

    void insertItem(CompoundItemDataObject item, int row);
signals:
    void closing();

private:
    Ui::addQuotation *ui;
};

#endif // ADDQUOTATION_H
