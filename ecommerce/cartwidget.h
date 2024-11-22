#ifndef CARTWIDGET_H
#define CARTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QPointer>
#include <QListView>
#include <QListWidget>
#include <QDesktopWidget>
#include <QScreen>

#include "customwidgets/Material/components/qtmaterialraisedbutton.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialbadge.h"
#include "customwidgets/Material/components/qtmaterialflatbutton.h"

#include "customwidgets/Basic/Material/gmmaterialnumberedit.h"
#include "finance/transactions/calculator/transactioncalculator.h"
#include "customwidgets/Misc/customshadoweffect.h"

#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"

#include "shippingdetailseditor.h"


namespace Ui {
class CartWidget;
}

class CartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CartWidget(GeneralVoucherDataObject *voucher1, QWidget *parent = nullptr);
    ~CartWidget();
    GeneralVoucherDataObject *voucher;
    SalesInventoryItemDatabaseHelper *itemHelper;

    QtMaterialAppBar *appBar;
    QtMaterialIconButton *backButton;
    QtMaterialIconButton *Notifications;
    QtMaterialFlatButton *Cart;
//    QtMaterialBadge *Cart;
    QtMaterialFlatButton *Store;

    QListWidget *itemsListWidget;
    GMMaterialNumberEdit *total;


    QString darkColor = "#1a3966";
    QString lightColor = "#cd9d7d";

public slots:
    void displayItems();

private slots:
    void setWidgets();
    void closeWindow();
    void removeItemFromCart(int index);
    void showOrderSummary();
    void checkOut();
    void resetWidgets();

private:
    Ui::CartWidget *ui;
};

#endif // CARTWIDGET_H
