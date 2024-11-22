#include "masterseditor.h"
#include "database/inventory/measurements/measurementsdatabasehelper.h"

#include <inventory/masters/measurement/measurementsmastereditor.h>
MastersEditor::MastersEditor(DatabaseHelper *db, QWidget *parent) : QWidget(parent)
{
    this->db = db;
    itemHelper = new SalesInventoryItemDatabaseHelper(db);
    invGroupHelper = new SalesInventoryGroupsDatabaseHelper(db);
    priceHelper = new PricelistDatabaseHelper(db);
    godownHelper = new GodownDatabaseHelper(db);
    uomHelper = new UomDataBaseHelper(db);
    ledHelper = new LedgerMasterDatabaseHelper(db);
    accHelper = new AccountGroupMasterDatabaseHelper(db);
    userHelper = new UserProfileDatabaseHelper(db);
    userGroupHelper = new UserGroupDatabaseHelper(db);
    termsHelper = new TermsDatabaseHelper(db);
    brandHelper= new BrandsDatabaseHelper(db);

}

void MastersEditor::setPermissions(UiAccessControlDataModel *value)
{
    permissions = value;
}

void MastersEditor::openEditor(QString indicator, QString id)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<indicator<<id;
    if(indicator == "I")
    {
        AddItems *additemWidget = new AddItems();
        QObject::connect(additemWidget,SIGNAL(closing()),this,SLOT(refreshList()));
        additemWidget->setWindowFlags(Qt::Window);
        additemWidget->setAttribute(Qt::WA_DeleteOnClose);
        if(id != ""){
            inventoryItemDataModel *item = itemHelper->getInventoryItemByIDasPtr(id);
            additemWidget->setItem(item);
            additemWidget->edit();
            if(!permissions->allowUpdate)
                additemWidget->disableSave();
            if(!permissions->allowDelete)
                additemWidget->disableDelete();

            emit editorOpen(additemWidget, item->ItemName);
        }
        else {
            emit editorOpen(additemWidget, "Create Item");
        }

    }
    else if (indicator == "G")
    {
        AddItemGroups *addGroupWidget = new AddItemGroups(this);
        QObject::connect(addGroupWidget,SIGNAL(closing()),this,SLOT(refreshList()));
        addGroupWidget->setWindowFlags(Qt::Window);
        if(id != ""){
            addGroupWidget->setTitle("Update Group");
            InventoryItemGroupDataModel *invGroup = invGroupHelper->getInvGroupPtrByID(id);
            addGroupWidget->edit(invGroup);
            if(!permissions->allowUpdate)
                addGroupWidget->disableSave();
            emit editorOpen(addGroupWidget, invGroup->GroupName);
        }
        else{
            emit editorOpen(addGroupWidget, "Create Inventory Group");

        }

    }
    else if (indicator == "Pl") {
        addPriceList *addPriceListWidget = new addPriceList(this);
        QObject::connect(addPriceListWidget,SIGNAL(closing()),this,SLOT(refreshList()));
        addPriceListWidget->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
        if(id != ""){
            PriceListDataObject *priceObj = priceHelper->getPriceListByIDasPtr(id.toInt());
            addPriceListWidget->edit(priceObj);
            addPriceListWidget->setPriceName(priceObj->priceListName);
            if(!permissions->allowUpdate)
                addPriceListWidget->disableSave();
            addPriceListWidget->setId(id.toInt());
            emit editorOpen(addPriceListWidget,  priceObj->priceListName);
        }
        else {

            emit editorOpen(addPriceListWidget, "Create Price List");
        }
    }
    else if (indicator == "Gd") {
        addGodown *addGodownObject = new addGodown(this);
        QObject::connect(addGodownObject,SIGNAL(closing()),this,SLOT(refreshList()));
        addGodownObject ->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
        if(id != ""){
            GodownDataObject *godown = godownHelper->getGodownById(id);
            addGodownObject ->setTitle("Update Godown");
            addGodownObject ->setGodown(godown);
            addGodownObject->edit();
            if(!permissions->allowUpdate)
                addGodownObject->disableSave();
            emit editorOpen(addGodownObject, godown->GodownName);
        }
        else {

            emit editorOpen(addGodownObject, "Create Godown" );
        }
    }
    else if (indicator == "U") {
        addUOM *addUOMWidget = new addUOM(this);
        QObject::connect(addUOMWidget,SIGNAL(closing()),this,SLOT(refreshList()));
        addUOMWidget->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
        if(id != ""){
            addUOMWidget->setTitle("Update UOM");
            UomDataModel *uomObj = uomHelper->getUomPtrByID(id);
            addUOMWidget->editUOM((uomObj));
            if(!permissions->allowUpdate)
                addUOMWidget->disableSave();
            emit editorOpen(addUOMWidget, uomObj->UomName);
        }
        else{
            emit editorOpen(addUOMWidget, "Create UOM");
        }
    }
    else if (indicator == "UC") {

        addConversion *addUOMCompoundWidget = new addConversion(this);
        QObject::connect(addUOMCompoundWidget,SIGNAL(closing()),this,SLOT(refreshList()));
        addUOMCompoundWidget ->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
        if(id != ""){
            addUOMCompoundWidget->setTitle("Update UOM Conversion");
            if(!permissions->allowUpdate)
                addUOMCompoundWidget->disableSave();
            addUOMCompoundWidget->setData(uomHelper->getUOMConversionById(id));
            emit editorOpen(addUOMCompoundWidget, "Edit UOM Conversion");
        }
        else
            emit editorOpen(addUOMCompoundWidget, "Edit UOM Conversion");
    }
    else if (indicator == "L") {
        addLedger *addLedgerWidget = new addLedger(this);
        QObject::connect(addLedgerWidget,SIGNAL(closing()),this,SLOT(refreshList()));
        addLedgerWidget->setWindowFlags(Qt::Window);
        if(id != ""){
            LedgerMasterDataModel *led = ledHelper->getLedgerPtrById(id);
            addLedgerWidget->setTitle("Update Ledger");
            addLedgerWidget->setLedger(led);
            addLedgerWidget->edit();
            if(!permissions->allowUpdate)
                addLedgerWidget->disableSave();
            emit editorOpen(addLedgerWidget, led->LedgerName);
        }
        else{
            emit editorOpen(addLedgerWidget,"Create Ledger");
        }
    }
    else if (indicator == "A") {
        addAccountGroup *newAccountGroup = new addAccountGroup(this);
        QObject::connect(newAccountGroup,SIGNAL(closing()),this,SLOT(refreshList()));
        newAccountGroup->setWindowFlags(Qt::Window);
        if(id != ""){
            AccountGroupDataModel *accGroup = accHelper->getGroupPtrById(id);
            newAccountGroup->setTitle("Update Account Group");
            newAccountGroup->editAccountGroup(accGroup);
            if(!permissions->allowUpdate)
                newAccountGroup->disableSave();
            emit editorOpen(newAccountGroup, accGroup->groupName);
        }
        else{
            emit editorOpen(newAccountGroup, "Create Account Group");
        }
    }

    else if (indicator == "E") {
        AddUserProfile *AddUserProfileWidget = new AddUserProfile(this);
        QObject::connect(AddUserProfileWidget,SIGNAL(closing()),this,SLOT(refreshList()));
        AddUserProfileWidget->setWindowFlags(Qt::Window);
        if(id != ""){
            UserProfileDataModel user = userHelper->getEmployeeByID(id.toInt());
            AddUserProfileWidget->showVoucher(user);
            if(!permissions->allowUpdate)
                AddUserProfileWidget->disableSave();
            emit editorOpen(AddUserProfileWidget, user.Name);
        }
        else {

            emit editorOpen(AddUserProfileWidget, "Create Employee");
        }

    }
    else if (indicator == "UG") {
        UserGroupDataModel *userGroup;
        if(id != ""){
            userGroup = userGroupHelper->getUserGroupById(id.toInt());
        }
        else
            userGroup = new UserGroupDataModel;

        UserGroupPermissionsEditor *AddUserGroupWidget = new UserGroupPermissionsEditor(userGroup);
        QObject::connect(AddUserGroupWidget,SIGNAL(closing()),this,SLOT(refreshList()));
        AddUserGroupWidget->setWindowFlags(Qt::Window | Qt::MaximizeUsingFullscreenGeometryHint);

        if(id != ""){
            if(!permissions->allowUpdate)
                AddUserGroupWidget->disableSave();
            emit editorOpen(AddUserGroupWidget, userGroup->Name);
        }
        else {
            AddUserGroupWidget->initUserGroup();
            emit editorOpen(AddUserGroupWidget, "Create User Group");
        }
    }

    else if (indicator == "TC") {

        TermsConditions *TermsConditionsWidget = new TermsConditions(this);
        QObject::connect(TermsConditionsWidget,SIGNAL(closing()),this,SLOT(refreshList()));
        TermsConditionsWidget->setWindowFlags(Qt::Window);
        if(id != ""){
            TermsConditionsWidget->editTerms(id.toInt());
            if(!permissions->allowUpdate)
                TermsConditionsWidget->disableSave();
        }
        emit editorOpen(TermsConditionsWidget, "Terms & Conditions");

    }

    else if (indicator == "B") {

        AddBrands *brandsWidget = new AddBrands(this);
        QObject::connect(brandsWidget,SIGNAL(closing()),this,SLOT(refreshList()));
        brandsWidget->setWindowFlags(Qt::Window);
        if(id != ""){
            brandsWidget->setBrand(brandHelper->getBrandById(id.toInt()));
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            brandsWidget->edit();
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            if(!permissions->allowUpdate)
                brandsWidget->disableSave();
        }
        emit editorOpen(brandsWidget, "Brands");
        qDebug()<<Q_FUNC_INFO<<__LINE__;

    }
    else if (indicator == "Tab") {

        TablesMasterHelper *dbHelper = new TablesMasterHelper();
        TableMasterDataModel *table = dbHelper->getTableById(id);
        TablesMasterEditor *editor= new TablesMasterEditor(table,dbHelper,this);

        QObject::connect(editor,SIGNAL(closing()),this,SLOT(refreshList()));
        editor->setWindowFlags(Qt::Window);
        if(id != ""){
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            if(!permissions->allowUpdate)
                editor->disableSave();
        }
        emit editorOpen(editor, "Tables Master");
        qDebug()<<Q_FUNC_INFO<<__LINE__;

    }
    else if (indicator == "MS") {

        MeasurementsDatabaseHelper *dbHelper = new MeasurementsDatabaseHelper();
        MeasurementMaster *obj;
        if(id != ""){
            obj = dbHelper->getMeasurementMasterByID(id);
        }
        else{
            obj = new MeasurementMaster();
        }
        MeasurementsMasterEditor *editor= new MeasurementsMasterEditor(obj,dbHelper,this);

        QObject::connect(editor,SIGNAL(closing()),this,SLOT(refreshList()));
        editor->setWindowFlags(Qt::Window);
        if(id != ""){
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            // if(!permissions->allowUpdate)
            //     editor->disableSave();
        }
        emit editorOpen(editor, "Measurements");
        qDebug()<<Q_FUNC_INFO<<__LINE__;

    }

}

void MastersEditor::refreshList()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    emit refresh();
}
