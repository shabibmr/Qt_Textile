#include "cartwidget.h"
#include "ui_cartwidget.h"

CartWidget::CartWidget(GeneralVoucherDataObject *voucher1, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CartWidget)
{
    ui->setupUi(this);
    this->setObjectName("background");
    this->setStyleSheet("QWidget#background{background-color:#ffffff}");
    this->setFixedSize(800,700);
    //    this->setWindowModality(Qt::ApplicationModal);

    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                          qApp->primaryScreen()->geometry()));

    this->setWindowTitle("");

    ui->frame->setFrameShape(QFrame::NoFrame);
    CustomShadowEffect *bodyShadow = new CustomShadowEffect();
    bodyShadow->setBlurRadius(30.0);
    bodyShadow->setDistance(6.0);
    bodyShadow->setColor(QColor(darkColor)); //#1a5050
    ui->frame->setAutoFillBackground(true);
    ui->frame->setGraphicsEffect(bodyShadow);

    voucher = voucher1;
    itemHelper = new SalesInventoryItemDatabaseHelper;

    setWidgets();


}

CartWidget::~CartWidget()
{
    delete ui;
}

void CartWidget::displayItems()
{
//    QFont boldFont = QFont("Roboto", 11, QFont::Bold);
//    QFont subTextFont = QFont("Roboto", 9, QFont::Normal);



//    for(int i=0;i<voucher->InventoryItems.size();i++){
//        QListWidgetItem *listItem = new QListWidgetItem;
//        QHBoxLayout *outerLayout = new QHBoxLayout;
//        QVBoxLayout *itemDescLayout = new QVBoxLayout;

//        QImage image(":/images/" + voucher->InventoryItems[i].BaseItem.ItemName + ".PNG");
//        QLabel *itemImage = new QLabel;
//        itemImage->setPixmap(QPixmap::fromImage(image.scaled(100,100)));

//        QLabel *itemName = new QLabel(voucher->InventoryItems[i].BaseItem.ItemName);
//        itemName->setFont(boldFont);
//        itemName->setStyleSheet("QLabel { color :" + darkColor + ";  }");

//        QLabel *price = new QLabel("\u20B9 " + QString::number(voucher->InventoryItems[i].BaseItem.price));
//        price->setFont(subTextFont);
//        price->setStyleSheet("QLabel { color :" + darkColor + ";  }");

//        QLabel *makingCharge = new QLabel("Making Charge: " + QString::number(voucher->InventoryItems[i].BaseItem.makingChargeRate) + "%");
//        makingCharge->setFont(subTextFont);
//        makingCharge->setStyleSheet("QLabel { color :" + darkColor + ";  }");

//        QLabel *stoneCharge = new QLabel("Stone Charge: \u20B9" + QString::number(voucher->InventoryItems[i].BaseItem.stoneCharge ));
//        stoneCharge->setFont(subTextFont);
//        stoneCharge->setStyleSheet("QLabel { color :" + darkColor + ";  }");

//        QLabel *weight = new QLabel("Weight: " + QString::number(voucher->InventoryItems[i].BaseItem.weight) + " gm");
//        weight->setFont(subTextFont);
//        weight->setStyleSheet("QLabel { color :" + darkColor + ";  }");

//        itemDescLayout->addWidget(itemName);
//        itemDescLayout->addWidget(price);
//        itemDescLayout->addWidget(makingCharge);
//        itemDescLayout->addWidget(weight);
//        itemDescLayout->addWidget(stoneCharge);

//        QWidget *widget = new QWidget;
//        widget->setLayout(itemDescLayout);

//        GMMaterialNumberEdit *qtyEdit = new GMMaterialNumberEdit(&voucher->InventoryItems[i].BaseItem.quantity);
//        qtyEdit->setReadOnly(true);
//        qtyEdit->setMaximumWidth(30);
//        qtyEdit->setTextColor(darkColor);


//        QLabel *subTotal = new QLabel("\u20B9" + QString::number(voucher->InventoryItems[i].BaseItem.subTotal));
//        subTotal->setFont(boldFont);

//        QtMaterialIconButton *deleteItem = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "cancel"));
//        deleteItem->setColor(darkColor);
//        connect(deleteItem, &QtMaterialIconButton::clicked, this, [=](bool val){
//            //            if(val)
//            qDebug()<<"remove at "<<i;
//            removeItemFromCart(i);
//        });

//        outerLayout->addWidget(itemImage);
//        outerLayout->addWidget(widget);
//        outerLayout->addSpacing(50);
//        outerLayout->addWidget(qtyEdit);
//        outerLayout->addSpacing(50);
//        outerLayout->addWidget(subTotal);
//        outerLayout->addStretch(1);
//        outerLayout->addWidget(deleteItem);
//        outerLayout->setSizeConstraint(QLayout::SetFixedSize);

//        QWidget *mainWid = new QWidget;
//        mainWid->setLayout(outerLayout);
//        mainWid->setMaximumWidth(500);
//        mainWid->setStyleSheet("QWidget#border{border-color:#ffffff}");

//        listItem->setSizeHint(mainWid->sizeHint());
//        //        listItem->setText("abc");

//        itemsListWidget->addItem(listItem);
//        itemsListWidget->setItemWidget(listItem, mainWid);

//        //        itemsListWidget->insertItem(i,listItem);
//        qDebug()<<"in wid"<<itemsListWidget->item(i);
//    }

//    qDebug()<<"list widget"<<itemsListWidget->count();
}

void CartWidget::setWidgets()
{
    QFont boldFont = QFont("Roboto", 11, QFont::Bold);
    QFont subTextFont = QFont("Roboto", 9, QFont::Normal);

    appBar = new QtMaterialAppBar();
    appBar->setBackgroundColor(QColor(darkColor));

    QLabel *windowLabel = new QLabel("My Cart");
    QPalette palette = windowLabel->palette();
    palette.setColor(windowLabel->foregroundRole(), QColor(lightColor));
    windowLabel->setPalette(palette);
    windowLabel->setFont(QFont("Roboto", 18, QFont::Normal));

    backButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "arrow_back"));
    backButton->setParent(appBar);
    backButton->setColor(QColor(lightColor));
    backButton->setIconSize(QSize(30,30));
    connect(backButton, &QtMaterialIconButton::clicked, this, &CartWidget::closeWindow);

    appBar->appBarLayout()->addWidget(backButton);
    appBar->appBarLayout()->addWidget(windowLabel);
    appBar->appBarLayout()->addStretch(1);

    Notifications = new QtMaterialIconButton(QtMaterialTheme::icon("social", "notifications"));
    Notifications->setColor(QColor(lightColor));
    int iconPadding = 2;
    Cart = new QtMaterialFlatButton(QString::number(voucher->InventoryItems.size()));
    Cart->setIcon(QtMaterialTheme::icon("action", "shopping_cart"));
    Cart->setForegroundColor(QColor(lightColor));
    Cart->setToolTip("Cart");
    Cart->setFontSize(6);
    Cart->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    Cart->setIconSize(QSize(20,20));

    //    Cart = new QtMaterialBadge(QtMaterialTheme::icon("action", "shopping_cart"));
    //    Cart->setTextColor(QColor(255,0,0));
    //    Cart->setText(QString::number(voucher->InventoryItems.size()));
    //    Cart->setBackgroundColor(QColor(lightColor));
    //    Cart->setIcon(QtMaterialTheme::icon("action", "shopping_cart"));

    //    Cart->setOverlayStyle(Material::TintedOverlay);
    Store = new QtMaterialFlatButton;
    Store->setText("Store");
    Store->setForegroundColor(QColor(lightColor));

    appBar->appBarLayout()->addWidget(Store);
    appBar->appBarLayout()->addWidget(Notifications);
    appBar->appBarLayout()->addWidget(Cart);

    ui->appBarLayout->addWidget(appBar);


    itemsListWidget = new QListWidget;
    itemsListWidget->setFrameShape(QFrame::NoFrame);
    itemsListWidget->setMaximumWidth(500);
    ui->cartItemsLayout->addWidget(itemsListWidget);

    if(voucher->InventoryItems.size()>0){
        displayItems();
        showOrderSummary();
    }

    else{
        QLabel *noItems = new QLabel("Cart is empty!");
        noItems->setFont(boldFont);
        noItems->setStyleSheet("QLabel { color :" + darkColor + ";  }");
        ui->cartItemsLayout->addWidget(noItems);
        ui->cartItemsLayout->setAlignment(noItems,Qt::AlignVCenter|Qt::AlignHCenter);
    }



}

void CartWidget::closeWindow()
{

    this->close();

}

void CartWidget::removeItemFromCart(int index)
{
    voucher->InventoryItems.removeAt(index);
    resetWidgets();
}

void CartWidget::showOrderSummary()
{
    TransactionCalculator::VoucherCalculateSales(voucher);

    QFont boldFont = QFont("Roboto", 11, QFont::Bold);
    QFont subTextFont = QFont("Roboto", 9, QFont::Normal);

    QLabel *title = new QLabel("Order Summary");
    title->setFont(boldFont);
    title->setStyleSheet("QLabel { color :" + darkColor + ";  }");

    total = new GMMaterialNumberEdit(&voucher->grandTotal, "\u20B9");
    total->setReadOnly(true);
    total->setLabel("Subtotal");
    total->setTextColor(darkColor);
    total->setFont(boldFont);
    total->setMaximumWidth(100);

    QtMaterialFlatButton *checkOutButton = new QtMaterialFlatButton;
    checkOutButton->setText(" Checkout ");
    checkOutButton->setIcon(QtMaterialTheme::icon("navigation", "arrow_forward"));
    checkOutButton->setIconPlacement(Material::RightIcon);
    checkOutButton->setFont(QFont("Roboto", 11, QFont::Normal));
    checkOutButton->setBackgroundColor(QColor("#cd9d7d"));
    checkOutButton->setBackgroundMode(Qt::OpaqueMode);
    checkOutButton->setForegroundColor(QColor("#ffffff"));
    checkOutButton->setMaximumWidth(120);

    connect(checkOutButton, &QtMaterialFlatButton::clicked,
            this, &CartWidget::checkOut );



    //    ui->orderSummaryLayout->addStretch(1);
    ui->orderSummaryLayout->addWidget(title);
    ui->orderSummaryLayout->addSpacing(50);
    ui->orderSummaryLayout->addWidget(total);
    ui->orderSummaryLayout->addSpacing(100);
    ui->orderSummaryLayout->addWidget(checkOutButton);
    ui->orderSummaryLayout->addStretch(1);

    ui->orderSummaryLayout->setAlignment(title, Qt::AlignHCenter|Qt::AlignVCenter);
    ui->orderSummaryLayout->setAlignment(total, Qt::AlignCenter);




}

void CartWidget::checkOut()
{

    ShippingDetailsEditor *shippingDetails = new ShippingDetailsEditor(voucher);
    shippingDetails->setWindowFlags(Qt::FramelessWindowHint);

//    QWidget *shippingWidget = new QWidget;
//    shippingWidget->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

//    QVBoxLayout *layout = new QVBoxLayout;
//    shippingWidget->setLayout(layout);
//    layout->addWidget(shippingDetails);
//    shippingDetails->setParent(shippingWidget);

//    CustomShadowEffect *bodyShadow = new CustomShadowEffect();
//    bodyShadow->setBlurRadius(30.0);
//    bodyShadow->setDistance(6.0);
//    bodyShadow->setColor(QColor("#1a3966")); //#1a5050
//    shippingWidget->setAutoFillBackground(true);
//    shippingWidget->setGraphicsEffect(bodyShadow);

    shippingDetails->setFocus();
    shippingDetails->show();


}

void CartWidget::resetWidgets()
{
    itemsListWidget->clear();

    if(voucher->InventoryItems.size()==0){
        QLabel *noItems = new QLabel("Cart is empty!");
        noItems->setFont(QFont("Roboto", 11, QFont::Bold));
        noItems->setStyleSheet("QLabel { color :" + darkColor + ";  }");
        ui->cartItemsLayout->removeWidget(itemsListWidget);
        ui->cartItemsLayout->addWidget(noItems);
    }

    else{
        displayItems();

    }

    TransactionCalculator::VoucherCalculateSales(voucher);
    total->setVariable(&voucher->grandTotal);
}

