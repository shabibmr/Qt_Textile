#include "shippingdetailseditor.h"
#include "ui_shippingdetailseditor.h"

ShippingDetailsEditor::ShippingDetailsEditor(GeneralVoucherDataObject *voucher1, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShippingDetailsEditor)
{
    ui->setupUi(this);

    qDebug()<<"shipping";

    this->setObjectName("background");
    this->setStyleSheet("QWidget#background{background-color:#ffffff}");
    this->setMinimumSize(800,700);
    this->setWindowModality(Qt::ApplicationModal);

    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                          qApp->primaryScreen()->geometry()));

//    ui->frame->setFrameShape(QFrame::NoFrame);
//    ui->frame->setStyleSheet("QWidget#background{background-color:#ffffff}");


    CustomShadowEffect *bodyShadow = new CustomShadowEffect();
    bodyShadow->setBlurRadius(30.0);
    bodyShadow->setDistance(6.0);
    bodyShadow->setColor(QColor("#1a3966")); //#1a5050
    ui->frame->setAutoFillBackground(true);
    ui->frame->setGraphicsEffect(bodyShadow);

    voucher = voucher1;
    customer = new AddressBookDataModel;

    setWidgets();
}

ShippingDetailsEditor::~ShippingDetailsEditor()
{
    delete ui;
}

void ShippingDetailsEditor::setWidgets()
{
    appBar = new QtMaterialAppBar();
    appBar->setBackgroundColor(QColor(darkColor));

    QLabel *windowLabel = new QLabel("Enter Shipping Details");
    QPalette palette = windowLabel->palette();
    palette.setColor(windowLabel->foregroundRole(), QColor(lightColor));
    windowLabel->setPalette(palette);
    windowLabel->setFont(QFont("Roboto", 12, QFont::Normal));

    backButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "arrow_back"));
    backButton->setParent(appBar);
    backButton->setColor(QColor(lightColor));
    backButton->setIconSize(QSize(30,30));
    connect(backButton, &QtMaterialIconButton::clicked, this, &ShippingDetailsEditor::closeWindow);

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

    Store = new QtMaterialFlatButton;
    Store->setText("Store");
    Store->setForegroundColor(QColor(lightColor));

    appBar->appBarLayout()->addWidget(Store);
    appBar->appBarLayout()->addWidget(Notifications);
    appBar->appBarLayout()->addWidget(Cart);

    ui->appBarLayout->addWidget(appBar);

    email = new GMMaterialLineEdit(&customer->email);
    email->setLabel("Email");
    email->setLabelColor(QColor(lightColor));
    email->setTextColor(QColor(darkColor));
    email->setMinimumWidth(500);

    firstName = new GMMaterialLineEdit(&customer->ContactName);
    firstName->setLabel("First Name");
    firstName->setLabelColor(QColor(lightColor));
    firstName->setTextColor(QColor(darkColor));
    firstName->setMinimumWidth(500);

    lastName = new GMMaterialLineEdit(&customer->email);
    lastName->setLabel("Last Name");
    lastName->setLabelColor(QColor(lightColor));
    lastName->setTextColor(QColor(darkColor));
    lastName->setMinimumWidth(500);

    address = new GMMaterialLineEdit(&customer->address);
    address->setLabel("Address");
    address->setLabelColor(QColor(lightColor));
    address->setTextColor(QColor(darkColor));
    address->setMinimumWidth(500);

    city = new GMMaterialLineEdit(&customer->city);
    city->setLabel("City");
    city->setLabelColor(QColor(lightColor));
    city->setTextColor(QColor(darkColor));
    city->setMinimumWidth(500);

    country = new GMMaterialLineEdit(&customer->country);
    country->setLabel("Country");
    country->setLabelColor(QColor(lightColor));
    country->setTextColor(QColor(darkColor));
    country->setMinimumWidth(500);

    region = new GMMaterialLineEdit(&customer->location);
    region->setLabel("Region");
    region->setLabelColor(QColor(lightColor));
    region->setTextColor(QColor(darkColor));
    region->setMinimumWidth(500);

    postalCode = new GMMaterialLineEdit(&customer->POBox);
    postalCode->setLabel("Postal Code");
    postalCode->setLabelColor(QColor(lightColor));
    postalCode->setTextColor(QColor(darkColor));
    postalCode->setMinimumWidth(500);

    phone = new GMMaterialLineEdit(&customer->PhoneNumber);
    phone->setLabel("PhoneNumber");
    phone->setLabelColor(QColor(lightColor));
    phone->setTextColor(QColor(darkColor));
    phone->setMinimumWidth(500);

    continueButton = new QtMaterialFlatButton("Continue");
    continueButton->setMinimumWidth(280);
    continueButton->setFont(QFont("Roboto", 11, QFont::Normal));
    continueButton->setBackgroundColor(QColor(lightColor));
    continueButton->setBackgroundMode(Qt::OpaqueMode);
    continueButton->setForegroundColor(QColor("#ffffff"));


    ui->ShippingDetailsLayout->addWidget(email);
    ui->ShippingDetailsLayout->addWidget(firstName);
    ui->ShippingDetailsLayout->addWidget(lastName);
    ui->ShippingDetailsLayout->addWidget(address);
    ui->ShippingDetailsLayout->addWidget(city);
    ui->ShippingDetailsLayout->addWidget(country);
    ui->ShippingDetailsLayout->addWidget(region);
    ui->ShippingDetailsLayout->addWidget(postalCode);
    ui->ShippingDetailsLayout->addWidget(phone);
    ui->ShippingDetailsLayout->addWidget(continueButton);

    ui->ShippingDetailsLayout->setAlignment(continueButton, Qt::AlignCenter);




    QLabel *title = new QLabel("Order Summary");
    title->setFont(boldFont);
    title->setStyleSheet("QLabel { color :" + darkColor + ";  }");

    total = new GMMaterialNumberEdit(&voucher->grandTotal, "\u20B9");
    total->setReadOnly(true);
    total->setLabel("Subtotal");
    total->setTextColor(darkColor);
    total->setFont(boldFont);
    total->setMaximumWidth(100);

    itemsList = new QListWidget;
    itemsList->setFrameShape(QFrame::NoFrame);
    summaryWidget = new QTableWidget;
    summaryWidget->setFrameShape(QFrame::NoFrame);
    summaryWidget->horizontalHeader()->hide();
    summaryWidget->verticalHeader()->hide();
    summaryWidget->setGridStyle(Qt::NoPen);
    summaryWidget->setSelectionMode(QAbstractItemView::NoSelection);
//    summaryWidget->setMaximumWidth(100);


    ui->orderSummaryLayout->addStretch(1);
    ui->orderSummaryLayout->addWidget(title);
    ui->orderSummaryLayout->addSpacing(50);
    ui->orderSummaryLayout->addWidget(itemsList);
    ui->orderSummaryLayout->addWidget(summaryWidget);
    ui->orderSummaryLayout->addSpacing(100);
    ui->orderSummaryLayout->addStretch(1);

    ui->orderSummaryLayout->setAlignment(title, Qt::AlignHCenter|Qt::AlignVCenter);
    ui->orderSummaryLayout->setAlignment(total, Qt::AlignCenter);
    ui->orderSummaryLayout->setAlignment(summaryWidget, Qt::AlignCenter);

    displayItems();
    setSummaryTable();



}

void ShippingDetailsEditor::closeWindow()
{
    this->close();

}


void ShippingDetailsEditor::displayItems()
{

    for(int i=0;i<voucher->InventoryItems.size();i++){
        QListWidgetItem *listItem = new QListWidgetItem;
        QHBoxLayout *outerLayout = new QHBoxLayout;
        QVBoxLayout *itemDescLayout = new QVBoxLayout;

        QImage image(":/images/" + voucher->InventoryItems[i].BaseItem.ItemName + ".PNG");
        QLabel *itemImage = new QLabel;
        itemImage->setPixmap(QPixmap::fromImage(image.scaled(50,50)));

        QLabel *itemName = new QLabel(voucher->InventoryItems[i].BaseItem.ItemName);
        itemName->setFont(boldFont);
        itemName->setStyleSheet("QLabel { color :" + darkColor + ";  }");

        QLabel *qty = new QLabel("Qty: " + QString::number(voucher->InventoryItems[i].BaseItem.quantity));
        qty->setFont(subTextFont);
        qty->setStyleSheet("QLabel { color :" + darkColor + ";  }");

        itemDescLayout->addWidget(itemName);
        itemDescLayout->addWidget(qty);

        QWidget *widget = new QWidget;
        widget->setLayout(itemDescLayout);

        QLabel *subTotal = new QLabel("\u20B9" + QString::number(voucher->InventoryItems[i].BaseItem.subTotal));
        subTotal->setFont(boldFont);

        outerLayout->addWidget(itemImage);
        outerLayout->addWidget(widget);
        outerLayout->addSpacing(50);
        outerLayout->addWidget(subTotal);
        outerLayout->addStretch(1);
        outerLayout->setSizeConstraint(QLayout::SetFixedSize);

        QWidget *mainWid = new QWidget;
        mainWid->setLayout(outerLayout);
        mainWid->setMaximumWidth(500);
        mainWid->setStyleSheet("QWidget#border{border-color:#ffffff}");

        listItem->setSizeHint(mainWid->sizeHint());

        itemsList->addItem(listItem);
        itemsList->setItemWidget(listItem, mainWid);

    }

}

void ShippingDetailsEditor::setSummaryTable()
{
    summaryWidget->insertRow(0);
    summaryWidget->setColumnCount(2);

    QTableWidgetItem *subTotal = new QTableWidgetItem;
    subTotal->setText("SubTotal");
    subTotal->setTextColor(QColor(darkColor));
    subTotal->setFont(subTextFont);
    subTotal->setTextAlignment(Qt::AlignTop);

    summaryWidget->setItem(0,0,subTotal);

    QTableWidgetItem *subTotalVal = new QTableWidgetItem;
    subTotalVal->setText("\u20B9" + QString::number(voucher->grandTotal,'f',2));
    subTotalVal->setTextAlignment(Qt::AlignRight);
    subTotalVal->setTextColor(QColor(darkColor));
    subTotalVal->setFont(subTextFont);
    summaryWidget->setItem(0,1,subTotalVal);

    summaryWidget->insertRow(1);

    QTableWidgetItem *storePickup = new QTableWidgetItem;
    storePickup->setText("Store Pickup");
    storePickup->setTextColor(QColor(darkColor));
    storePickup->setFont(subTextFont);
    storePickup->setTextAlignment(Qt::AlignTop);
    summaryWidget->setItem(1,0,storePickup);

    QTableWidgetItem *storePickupVal = new QTableWidgetItem;
    storePickupVal->setText("Free");
    storePickupVal->setTextAlignment(Qt::AlignRight);
    storePickupVal->setTextColor(QColor(darkColor));
    storePickupVal->setFont(subTextFont);
    summaryWidget->setItem(1,1,storePickupVal);

    summaryWidget->insertRow(2);

    QTableWidgetItem *taxes = new QTableWidgetItem;
    taxes->setText("Taxes");
    taxes->setTextColor(QColor(darkColor));
    taxes->setFont(subTextFont);
    taxes->setTextAlignment(Qt::AlignTop);
    summaryWidget->setItem(2,0,taxes);

    QTableWidgetItem *taxesVal = new QTableWidgetItem;
    taxesVal->setText("\u20B9" + QString::number(voucher->taxTotalAmount,'f',2));
    taxesVal->setTextColor(QColor(darkColor));
    taxesVal->setTextAlignment(Qt::AlignRight);
    taxesVal->setFont(subTextFont);
    summaryWidget->setItem(2,1,taxesVal);

    summaryWidget->insertRow(3);

    QTableWidgetItem *Total = new QTableWidgetItem;
    Total->setText("Total");
    Total->setTextColor(QColor(darkColor));
    Total->setFont(boldFont);
    Total->setTextAlignment(Qt::AlignTop);
    summaryWidget->setItem(3,0,Total);

    QTableWidgetItem *TotalVal = new QTableWidgetItem;
    TotalVal->setText("\u20B9" + QString::number(voucher->grossTotal,'f',2));
    TotalVal->setTextAlignment(Qt::AlignRight);
    TotalVal->setTextColor(QColor(darkColor));
    TotalVal->setFont(boldFont);
    summaryWidget->setItem(3,1,TotalVal);

}
