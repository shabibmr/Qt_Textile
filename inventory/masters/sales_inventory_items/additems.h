#ifndef ADDITEMS_H
#define ADDITEMS_H

#include <QWidget>
#include <QSqlRecord>
#include <QSignalMapper>
#include <QPrinterInfo>

#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "database/inventory/sales_inventory_groups/salesinventorygroupsdatabasehelper.h"
#include "database/inventory/property/propertydatabasehelper.h"
#include "database/inventory/uom/uomdatabasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "database/inventory/brands/brandsdatabasehelper.h"
#include "database/inventory/sales_inventory_items/inventoryitemimagesdatabasehelper.h"
#include "database/inventory/godown/godowndatabasehelper.h"
#include "database/inventory/pricelist/pricelistdatabasehelper.h"
#include "database/inventory/category/categorydatabasehelper.h"
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"
#include "database/inventory/sales_inventory_items/bundledatabasehelper.h"



#include "datamodels/inventory/property/propertydatamodel.h"
#include "datamodels/inventory/uom/uomdatamodel.h"
#include "datamodels/inventory/sales_inventory_item/inventoryitemdatamodel.h"
#include "datamodels/accounts/ledgermasterdatamodel.h"

#include "datamodels/inventory/sales_inventory_item/compounditemdataobject.h"
#include "datamodels/inventory/sales_inventory_item/itemimagedatamodel.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "database/Counter/billofmaterialsingredientsdatabasehelper.h"
#include "inventory/masters/uom/addconversion.h"
#include "inventory/pricelist/pricelistbyitem.h"

#include "Notifications/notificationledgerslist.h"
#include "inventory/masters/item_label_data/itemlabeldata.h"

#include <QToolButton>
#include "customwidgets/gmitemsearch.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"

#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "bundle/itembundlelistdelegate.h"
#include "bundle/itembundlelistmodel.h"
#include "bundle/itembundletableview.h"

namespace Ui {
class AddItems;
}

class AddItems : public QWidget
{
    Q_OBJECT

public:
    explicit AddItems(QWidget *parent = 0);
    ~AddItems();
    QString id;

    QtMaterialSnackbar *snackbar;
    inventoryItemDataModel* invItem;
    CompoundItemDataObject *compItem;
    QList<QSharedPointer<inventoryItemDataModel>> itemsListPtr;

//    QStringList CategoriesList;
    QMap<int, QString> categories;

    SalesInventoryGroupsDatabaseHelper *itemGroupHelper;
    GodownDatabaseHelper *godownHelper;
    SalesInventoryItemDatabaseHelper *dbHelper;
    LedgerMasterDatabaseHelper *ledgerHelper;
    UomDataBaseHelper* uomHelper;
    addConversion* addUOMCompoundWidget;
    PropertyDatabaseHelper* propertyHelper;
    BrandsDatabaseHelper* brandsHelper;
    InventoryItemImagesDatabaseHelper* imagesHelper;
    BillOfMaterialsIngredientsDataBaseHelper *bomHelper;
    CategoryDatabaseHelper *categoryHelper;
    BundleDatabaseHelper *bundleHelper;

    PriceListByItem *priceListWidget;

    GMLedgerLineWidget *defaultLedgerSearch;


    QList<RestaurantIngredientsDataModel> boms;
    GMItemSearch *itemLine ;
    QString tempBomItem;
    inventoryItemDataModel tempBomItemObj;

    QMap<QString,QString> invGroups;
    QMap<QString,QString> sections;
    QList<LedgerMasterDataModel> SalesledgersList;
    QList<LedgerMasterDataModel> PurchaseledgersList;
    QList<LedgerMasterDataModel> VATledgersList;

    QList<UomDataModel> AllUomList;
    QList<UomConversionDataModel> UomConv;
    QList<PropertyDataModel> AllPropertyList;
    QMap<int,QString> brandsMap;
    QSqlQueryModel *priceModel;

    ItemImageDatamodel imageData;
    ItemLabelData *labelWidget;

    ItemBundleListModel *bundleModel;
    ItemBundleTableView *bundleTable;

    void setBOMTable();
    QSqlQueryModel *model;
    UomConversionDataModel *uomConvObj;

    float convalue=1;
    QString uomId;

    void setItem(inventoryItemDataModel *value);
    void disableSave();

    void disableDelete();
private slots:
    void addNewItemBySearch(inventoryItemDataModel);

    void on_addPropertyPushButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_newImagePushButton_clicked();

    void setupItemsTab();

    void setupPropertiesTab();

    void setupUOMTab();

    void setupBOMTab();

    void setupImagesTab();

    void setupBundleTab();


    void setupRestaurantTab();

    void on_customItemCheckBox_clicked();

    void sendNotifications(QStringList);

private:
    Ui::AddItems *ui;

public slots:
    void edit();
    void editProperties(QString itemID);
    void editUOM(QString itemID);
    void editImages(QString itemID);
    void editBOM(QString itemID);
    void editBundle(QString itemID);

    void addPropertyToTable();
    void delPropPressed(int rc,int c);
    void setTitle(QString text);
    void setItemCode(QString text);
    void editRestaurantTab(QString KOTPrinters);
    void setupPriceListTab();


    void deleteUOMConvSlot(UomConversionDataModel *uomObj);

signals:
    void closing();

    void savedItem(CompoundItemDataObject invItem);

    void NotificationAccepted();
    void NotificationRejected(QString reason);

private slots:
    void keyPressEvent(QKeyEvent *e);

    void on_imageResizeButton_clicked();

    void setBOMItem(inventoryItemDataModel invItem);
    void setUOMTable();
    void on_AddBomButton_clicked();
    void deletebom(bool val);
    void changeCHK(bool val);

    void on_addUomPushButton_clicked();
    void on_UomConversionTableView_doubleClicked(const QModelIndex &index);
    void on_codeLineEdit_returnPressed();
    void uomIndexChanged(int index);

    void on_customItemCheckBox_toggled(bool checked);
    void on_SerialLineEdit_returnPressed();
    void on_pluCheck_toggled(bool checked);
    void on_openingBalanceValueLineEdit_returnPressed();
    void on_printBarcodeButton_clicked();
    void on_geeneratePLU_clicked();
    void on_priceLineEdit_returnPressed();
    void on_nettPrice_returnPressed();
    void on_priceLineEdit_editingFinished();


    void on_deletePushButton_clicked();
    void on_nettPrice_editingFinished();
};


#endif // ADDITEMS_H
