#ifndef ITEMDETAIL_H
#define ITEMDETAIL_H

#include <QWidget>
#include <QLabel>
#include <QPointer>

#include "datamodels/inventory/sales_inventory_item/compounditemdataobject.h"
#include "customwidgets/Validator/myvalidator.h"

#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialtabs.h"
#include "customwidgets/Basic/Material/gmmaterialcombobox.h"
#include "customwidgets/Basic/Material/gmmaterialnumberedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/Material/gmmateriallineedit.h"
#include "database/inventory/pricelist/pricelistdatabasehelper.h"
#include "database/inventory/godown/godowndatabasehelper.h"
#include "godownallocationwidget.h"

#include "itempricedetailswidget.h"
#include "customwidgets/SearchWidgets/Godown/gmgodownlinematerial.h"

namespace Ui {
class ItemDetail;
}

class ItemDetail : public QWidget
{
    Q_OBJECT

public:
    explicit ItemDetail(GeneralVoucherDataObject *voucher,
                        int row,
                        int transactionType,
                        SalesInventoryItemDatabaseHelper *itemHelper,
                        UomDataBaseHelper *uomHelper,
                        PricelistDatabaseHelper *priceHelper = new PricelistDatabaseHelper,
                        GodownDatabaseHelper *godownHelper = new GodownDatabaseHelper,
                        QString priceListId = "",
                        QWidget *parent = nullptr);
    ~ItemDetail();

    CompoundItemDataObject *Item;
    int row;
    GeneralVoucherDataObject *voucher;
    QString windowTitle;

    ItemPriceDetailsWidget *itemPricesWid;

    QtMaterialAppBar *appBar;
    QtMaterialTabs *tabs;
//    GMMaterialLineEdit *group;
    GMMaterialComboBox *uom;
    GMMaterialNumberEdit *priceEdit;
    GMMaterialComboBox *priceListWidget;
    GMMaterialNumberEdit *quantityEdit;
    GMMaterialNumberEdit *discPercent;
    GMMaterialNumberEdit *discAmount;
    GMMaterialNumberEdit *taxRate;
    GMMaterialNumberEdit *taxAmount;
    GMMaterialNumberEdit *AmountEdit;
    GMGodownLineMaterial *godownWidget;
    GMMaterialLineEdit *narrationEdit;

    GMMaterialLineEdit *serialNoEdit;
    GMMaterialLineEdit *batchCodeEdit;
    GMDateEdit *mfgDateEdit;
    GMDateEdit *expiryDateEdit;



    float price =0;
    float quantity =0;
    float amount =0;
    QString selectedGodown;


    QtMaterialIconButton *backButton;
    QtMaterialFloatingActionButton *okButton;
    SalesInventoryItemDatabaseHelper *itemHelper;
    PricelistDatabaseHelper *priceHelper;
    UomDataBaseHelper *uomHelper;
    GodownDatabaseHelper *godownHelper;
    QString priceListId;
    QString priceListName;
    int transactionType;

    QPointer<QWidget> mainWidget;


    GodownAllocationWidget *godownAllocWid;
    QWidget *batchWid;


private slots:
    void initGlobalObjects();
    void setWidgets();
    void setTabs();
    void resetWidgets();
    void closeEditor();
    void setDefaultValues();

    void setFocusOrder();


    void setPrices();
    void setBatchTab();
signals:
    void updateItem();

private:
    Ui::ItemDetail *ui;
};

#endif // ITEMDETAIL_H
