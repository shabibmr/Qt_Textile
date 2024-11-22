#ifndef ITEMSELECTORWIDGET_H
#define ITEMSELECTORWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QPointer>
#include <QListView>
#include <QScreen>
#include <QWindow>
#include <QScrollArea>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QImageReader>


#include "customwidgets/Material/components/qtmaterialraisedbutton.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "customwidgets/Misc/customshadoweffect.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "customwidgets/Material/components/qtmaterialdrawer.h"
#include "customwidgets/Material/components/qtmaterialslider.h"
#include "customwidgets/Material/components/qtmaterialscrollbar.h"
#include "customwidgets/Basic/gmhovertoolbutton.h"


#include "itemdetailwidget.h"
#include "cartwidget.h"

namespace Ui {
class ItemSelectorWidget;
}

class ItemSelectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ItemSelectorWidget(QWidget *parent = nullptr);
    ~ItemSelectorWidget();

    QFont subTextFont = QFont("Calibri", 9, QFont::Normal);
    QFont boldFont = QFont("Calibri", 12, QFont::Bold);

    QString darkColor = "#1a3966";
    QString lightColor = "#cd9d7d";

    QSqlQueryModel *itemsList;
    SalesInventoryItemDatabaseHelper *itemHelper;
    GeneralVoucherDataObject *voucher;

    QtMaterialAppBar *appBar;
    QtMaterialIconButton *backButton;
    QtMaterialIconButton *Notifications;
    QtMaterialFlatButton *Cart;
    QtMaterialFlatButton *Store;
    QtMaterialIconButton *filterButton;

    QtMaterialDrawer *filterDrawer;
    QVBoxLayout *filterLayout;

    QVBoxLayout *collectionFilterLayout;
    QVBoxLayout *priceFilterLayout;
    QHBoxLayout *mcFilterLayout;


    QScrollArea* scrollArea;
    QGridLayout *itemsGrid;
    QWidget *gridWidget;
    QtMaterialScrollBar *scrollBar;


    bool CollectionExpanded;
    bool priceExpanded;
    bool mcExpanded;
    bool scExpanded;
    bool weightExpanded;

    QtMaterialFlatButton *all;
    QtMaterialSlider *priceSlider;
    QtMaterialSlider *mcSlider;


    QNetworkAccessManager *nam;

private slots:
    void displayItems();
    void itemClicked(QString itemID)    ;
    void setWidgets();
    void addToCart(inventoryItemDataModel *item);
    void openCartWidget();
    void setFilterList();
    void closeWindow();
    void showCollectionFilter(bool show);
    void showPriceFilter();
    void showMCFilter();
    void clearLayout(QLayout *lay);
    QImage readImage(QString name);

private:
    Ui::ItemSelectorWidget *ui;
};

#endif // ITEMSELECTORWIDGET_H
