#ifndef ITEMDETAILWIDGET_H
#define ITEMDETAILWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include <QGroupBox>
#include <QDesktopWidget>
#include <QPlainTextEdit>
#include <QGraphicsDropShadowEffect>
#include <QScreen>


#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "customwidgets/Basic/Material/gmmateriallineedit.h"
#include "customwidgets/Basic/Material/gmmaterialcombobox.h"
#include "customwidgets/Basic/Material/gmmaterialnumberedit.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialflatbutton.h"
#include "customwidgets/Misc/customshadoweffect.h"
#include "login/loginvalues.h"

namespace Ui {
class ItemDetailWidget;
}

class ItemDetailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ItemDetailWidget(inventoryItemDataModel *item, QWidget *parent = nullptr);
    ~ItemDetailWidget();
    inventoryItemDataModel *Item;

    QString windowTitle;

    void setImages();
    void setItemDescription();
    GMMaterialNumberEdit *makingChargeEdit;
    GMMaterialNumberEdit *weightEdit;
    GMMaterialNumberEdit *stoneChargeEdit;
    GMMaterialNumberEdit *qtyEdit;
    GMMaterialNumberEdit *discountEdit;

    GMMaterialNumberEdit *priceLabel;
    GMMaterialNumberEdit *totalLabel;

    QtMaterialAppBar *appBar;
    QtMaterialIconButton *backButton;

//    QLabel *priceLabel;
//    QLabel *subTotalLabel;


    QString getHtml();


private slots:
    void calc();
    void setWidgets();
    void setWidgetData();
    void closeWindow();
    void addToCart();


private:
    Ui::ItemDetailWidget *ui;

signals:

    void addItemToCart(inventoryItemDataModel *Item) ;
};

#endif // ITEMDETAILWIDGET_H
