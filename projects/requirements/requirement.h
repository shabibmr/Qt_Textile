#ifndef REQUIREMENT_H
#define REQUIREMENT_H

#include <QWidget>
#include <QPointer>
#include <QKeyEvent>
#include "login/loginvalues.h"

#include "additemadditionalrequirement.h"
#include "login/loginvalues.h"
#include "database/hr/userprofiledatabasehelper.h"
#include <QMessageBox>
#include "datamodels/addressbook/addressbooksmall.h"
#include "database/addressbook/addressbookdatabasehelper.h"
#include "customwidgets/ContactSearch/gmcontactsearch.h"



#include "projects/requirements/additemrequirement.h"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/projects/requirementsdatabasehelper.h"
#include <QStandardItemModel>
#include <QPointer>
#include <QToolButton>
#include "database/finance/TransactionHelper/generalvoucherdatabasehelper.h"
#include "projects/quotations/addquotation.h"

namespace Ui {
class Requirement;
}

class Requirement : public QWidget
{
    Q_OBJECT

public:
    explicit Requirement(QWidget *parent = 0);
    ~Requirement();
    //  widgets
    addItemRequirement *addItemRequirementWidget;
    addItemAdditionalRequirement *addItemAdditionalRequirementWidget;

    addQuotation* addQuotationWidget;

    //  common
    QString projectID="";
    QString vNo="";
    QString vPrefix ="";

    QToolButton *exportButton;
    int SalesManId=0;
    QString Username = "";


    //  databases

    RequirementsDatabaseHelper *dbHelper;
    SalesInventoryItemDatabaseHelper *itemsHelper;
    LedgerMasterDatabaseHelper *ledgerHelper;
    AddressBookDatabaseHelper *addressBookHelper;
    UserProfileDatabaseHelper *userHelper;



    //  item specific

    QList<QSharedPointer<inventoryItemDataModel>> itemsListPtr;

    QList<LedgerMasterDataModel> allLedgers;

    QList<CompoundItemDataObject> ordersList;

    QStringList NamesForCompleter;
    QStringList itemNamesForLineEdit;
    QMap<int,QString> salesmanMap;


    bool openNewWidget = true;

    //  ledger specific
    QList <LedgerMasterDataModel> ordersLedList;
    QList<QSharedPointer<LedgerMasterDataModel>> allLedgersPtr ;
    QList<QSharedPointer<AddressBookSmall>> addresses;

    QPointer<GMContactSearch> contactsWidget;
    AddressBookDataModel CustomerContact;

    bool openNewWidgetLedger = true;
    QPointer<GMLedgerSearch> customerName;

    LedgerMasterDataModel CustomerName;


private slots:
    void on_addItemPushButton_clicked();
    void keyPressEvent(QKeyEvent *e);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_itemTableWidget_doubleClicked(const QModelIndex &index);
    void on_addAdditionalPushButton_clicked();

    void on_additionalTableWidget_doubleClicked(const QModelIndex &index);
    void setCustomerContact(AddressBookDataModel x);

public slots:
    void setTitle(QString Text);
    void setProjectID(QString projID);
    void showVoucher(GeneralVoucherDataObject voucher);

    void importToRequirement(GeneralVoucherDataObject voucher);

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

    void setCustomerName(LedgerMasterDataModel);

    void saveVoucher(GeneralVoucherDataObject*);

    void exportToQuotation();

signals:
    void closing();

private:
    Ui::Requirement *ui;
};

#endif // REQUIREMENT_H
