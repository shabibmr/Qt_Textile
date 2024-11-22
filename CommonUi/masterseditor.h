#ifndef MASTERSEDITOR_H
#define MASTERSEDITOR_H

#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "database/inventory/sales_inventory_groups/salesinventorygroupsdatabasehelper.h"
#include "database/inventory/pricelist/pricelistdatabasehelper.h"
#include "database/inventory/godown/godowndatabasehelper.h"
#include "database/inventory/uom/uomdatabasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "database/finance/accountmaster/accountgroupmasterdatabasehelper.h"
#include "database/finance/terms/termsdatabasehelper.h"
#include "database/hr/usergroupdatabasehelper.h"
#include "database/hr/userprofiledatabasehelper.h"
#include "database/Counter/TableManagement/tablesmasterhelper.h"

#include "inventory/masters/sales_inventory_items/additems.h"
#include "inventory/masters/sales_inventory_groups/additemgroups.h"
#include "inventory/masters/godowns/addgodown.h"
#include "inventory/masters/uom/adduom.h"
#include "inventory/masters/uom/addconversion.h"
#include "hr/usergrouppermissionseditor.h"
#include "inventory/masters/brands/addbrands.h"
#include "inventory/pricelist/addpricelist.h"
#include "counter/TableManagement/tablesmastereditor.h"

#include "finance/masters/accountgroups/addaccountgroup.h"
#include "finance/masters/ledgers/addledger.h"
#include "finance/masters/termsconditions/termsconditions.h"
#include "hr/adduserprofile.h"


class MastersEditor : public QWidget
{
    Q_OBJECT

public:
    MastersEditor(DatabaseHelper *db, QWidget *parent = nullptr);

    DatabaseHelper *db;
    SalesInventoryItemDatabaseHelper *itemHelper;
    SalesInventoryGroupsDatabaseHelper *invGroupHelper;
    PricelistDatabaseHelper *priceHelper;
    GodownDatabaseHelper *godownHelper;
    UomDataBaseHelper *uomHelper;
    LedgerMasterDatabaseHelper *ledHelper;
    AccountGroupMasterDatabaseHelper *accHelper;
    UserProfileDatabaseHelper *userHelper;
    UserGroupDatabaseHelper *userGroupHelper;
    TermsDatabaseHelper *termsHelper;
    BrandsDatabaseHelper *brandHelper;

    UiAccessControlDataModel *permissions;
    void openEditor(QString indicator, QString id="");

    void setPermissions(UiAccessControlDataModel *value);

private slots:
    void refreshList();
signals:
    void editorOpen(QWidget *, QString title)    ;
    void refresh();
};

#endif // MASTERSEDITOR_H
