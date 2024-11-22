#include "itemdetailwidget.h"
#include "ui_itemdetailwidget.h"

ItemDetailWidget::ItemDetailWidget(inventoryItemDataModel *item,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemDetailWidget)
{
    ui->setupUi(this);



    this->Item = item;
    this->setObjectName("background");
    this->setStyleSheet("QWidget#background{background-color:#ffffff}");

    this->setMinimumSize(800,700);
    this->setWindowModality(Qt::ApplicationModal);

    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                          qApp->primaryScreen()->geometry()));

    ui->frame_2->setFrameShape(QFrame::NoFrame);
    CustomShadowEffect *bodyShadow = new CustomShadowEffect();
    bodyShadow->setBlurRadius(30.0);
    bodyShadow->setDistance(6.0);
    bodyShadow->setColor(QColor("#1a3966")); //#1a5050
    ui->frame_2->setAutoFillBackground(true);
    ui->frame_2->setGraphicsEffect(bodyShadow);

    setWidgets();
    setImages();
    calc();
    qDebug()<<"Item"<<Item->price<<Item->subTotal;
    setItemDescription();
    setWidgetData();


}

ItemDetailWidget::~ItemDetailWidget()
{
    delete ui;
}

void ItemDetailWidget::setImages()
{

    QImage image(":/images/Necklace.PNG");
    ui->Picture->setPixmap(QPixmap::fromImage(image.scaled(300,300)));

    QToolButton* image1 = new QToolButton;
    image1->setIcon(QIcon(":/images/" + Item->ItemName + ".PNG"));
    image1->setIconSize(QSize(50,50));
    image1->setAutoRaise(true);

    QWidget *Img1widget = new QWidget;
    QHBoxLayout *lay = new QHBoxLayout;
    Img1widget->setLayout(lay);
    lay->addWidget(image1);
    Img1widget->setAttribute(Qt::WA_TranslucentBackground);
    Img1widget->setWindowFlags(Qt::FramelessWindowHint);
    Img1widget->setStyleSheet("background: #ffffff");

//    QGraphicsDropShadowEffect *wndShadow = new QGraphicsDropShadowEffect;
//    wndShadow->setBlurRadius(9.0);
//    wndShadow->setColor(QColor(0, 0, 0, 160));
//    wndShadow->setXOffset(4.0);
//    wndShadow->setYOffset(4.0);

    CustomShadowEffect *bodyShadow = new CustomShadowEffect();
    bodyShadow->setBlurRadius(20.0);
    bodyShadow->setDistance(6.0);
    bodyShadow->setColor(QColor(0, 0, 0, 80));
    Img1widget->setAutoFillBackground(true);
    Img1widget->setGraphicsEffect(bodyShadow);

    QToolButton* image2 = new QToolButton;
    image2->setIcon(QIcon(":/images/" + Item->ItemName + "2.PNG"));
    image2->setIconSize(QSize(50,50));
    //    image2->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    image2->setAutoRaise(true);

    QWidget *Img2widget = new QWidget;
    QHBoxLayout *lay2 = new QHBoxLayout;
    Img2widget->setLayout(lay2);
    lay2->addWidget(image2);
    Img2widget->setAttribute(Qt::WA_TranslucentBackground);
    Img2widget->setWindowFlags(Qt::FramelessWindowHint);

    Img2widget->setStyleSheet("background: #fff9f3");
    QGraphicsDropShadowEffect *wndShadow3 = new QGraphicsDropShadowEffect;
    wndShadow3->setBlurRadius(9.0);
    wndShadow3->setColor(QColor(0, 0, 0, 160));
    wndShadow3->setXOffset(4.0);
    wndShadow3->setYOffset(4.0);

    CustomShadowEffect *bodyShadow3 = new CustomShadowEffect();
    bodyShadow3->setBlurRadius(20.0);
    bodyShadow3->setDistance(6.0);
    bodyShadow3->setColor(QColor(0, 0, 0, 80));
    Img2widget->setAutoFillBackground(true);
    Img2widget->setGraphicsEffect(bodyShadow3);

    connect(image1, &QToolButton::clicked, this, [=](bool){
        QImage image(":/images/Necklace.PNG");
        image1->setAutoRaise(false);
        image2->setAutoRaise(true);
        bodyShadow->setColor(QColor(0, 0, 0, 160));
        bodyShadow3->setColor(QColor(0,0,0,80));
        ui->Picture->setPixmap(QPixmap::fromImage(image.scaled(300,300)));
    });

    connect(image2, &QToolButton::clicked, this, [=](bool){
        QImage image(":/images/Necklace2.PNG");
        image2->setAutoRaise(false);
        image1->setAutoRaise(true);
        bodyShadow3->setColor(QColor(0, 0, 0, 160));
        bodyShadow->setColor(QColor(0,0,0,80));
        ui->Picture->setPixmap(QPixmap::fromImage(image.scaled(300,300)));
    });

    ui->imagesLayout->addSpacerItem(new QSpacerItem(10,10));
    ui->imagesLayout->addStretch(1);
    ui->imagesLayout->addWidget(Img1widget);
    ui->imagesLayout->addWidget(Img2widget);
    ui->imagesLayout->addStretch(1);
    ui->imagesLayout->addSpacerItem(new QSpacerItem(100,100));


}

void ItemDetailWidget::setItemDescription()
{

//    QFont Font;
//    Font.setBold(true);
//    Font.setPointSize(13);
//    Font.setFamily("Roboto");

//    priceLabel = new GMMaterialNumberEdit(&Item->price,"\u20B9" );
//    qDebug()<<"price"<<Item->price;
//    priceLabel->setLabel("Price");
//    priceLabel->setMaximumWidth(200);
//    priceLabel->setReadOnly(true);
//    priceLabel->setTextColor(QColor("#1a3966"));
//    priceLabel->setFont(Font);
//    ui->descLayout->addWidget(priceLabel);

//    makingChargeEdit = new GMMaterialNumberEdit(&Item->makingChargeRate, "", " %");
//    makingChargeEdit->setLabel("Making Charge");
//    makingChargeEdit->setMaximumWidth(200);
//    makingChargeEdit->setLabelColor(QColor("#cd9d7d"));
//    makingChargeEdit->setTextColor(QColor("#1a3966"));

//    connect(makingChargeEdit, &GMMaterialNumberEdit::editFinished, this, [=](){
//        qDebug()<<"changed mc"<<Item->makingChargeRate;
//        calc();
//        setWidgetData();
//    });

//    stoneChargeEdit = new GMMaterialNumberEdit(&Item->stoneCharge,"\u20B9 " );
//    stoneChargeEdit->setLabel("Stone Charge");
//    stoneChargeEdit->setMaximumWidth(200);
//    stoneChargeEdit->setLabelColor(QColor("#cd9d7d"));
//    stoneChargeEdit->setTextColor(QColor("#1a3966"));
//    connect(stoneChargeEdit, &GMMaterialNumberEdit::editFinished, this, [=](){
//        calc();
//        setWidgetData();
//    });

//    weightEdit = new GMMaterialNumberEdit(&Item->weight, "", " g");
//    weightEdit->setLabel("Weight");
//    weightEdit->setMaximumWidth(200);
//    weightEdit->setLabelColor(QColor("#cd9d7d"));
//    weightEdit->setTextColor(QColor("#1a3966"));
//    connect(weightEdit, &GMMaterialNumberEdit::editFinished, this, [=](){
//        calc();
//        setWidgetData();

//    });

//    qtyEdit = new GMMaterialNumberEdit(&Item->quantity, "", " Nos");
//    qtyEdit->setLabel("Quantity");
//    qtyEdit->setMaximumWidth(200);
//    qtyEdit->setLabelColor(QColor("#cd9d7d"));
//    qtyEdit->setTextColor(QColor("#1a3966"));
//    connect(qtyEdit, &GMMaterialNumberEdit::editFinished, this, [=](){
//        calc();
//        setWidgetData();
//    });

//    discountEdit = new GMMaterialNumberEdit(&Item->discountinAmount ,"\u20B9 " );
//    discountEdit->setLabel("Discount");
//    discountEdit->setMaximumWidth(200);
//    discountEdit->setLabelColor(QColor("#cd9d7d"));
//    discountEdit->setTextColor(QColor("#1a3966"));
//    connect(discountEdit, &GMMaterialNumberEdit::editFinished, this, [=](){
//        calc();
//    });

//    ui->descLayout->addWidget(makingChargeEdit);
//    ui->descLayout->addWidget(weightEdit);
//    ui->descLayout->addWidget(stoneChargeEdit);
//    ui->descLayout->addWidget(qtyEdit);
//    ui->descLayout->addWidget(discountEdit);

//    totalLabel = new GMMaterialNumberEdit(&Item->subTotal,"\u20B9" );
//    qDebug()<<"total"<<Item->subTotal;
//    totalLabel->setLabel("Total");
//    totalLabel->setMinimumWidth(200);
//    totalLabel->setReadOnly(true);
//    totalLabel->setTextColor(QColor("#1a3966"));
//    totalLabel->setFont(Font);
//    ui->descLayout->addWidget(totalLabel);

//    QtMaterialFlatButton *addToCartButton = new QtMaterialFlatButton;
//    addToCartButton->setText(" Add to Cart ");
//    addToCartButton->setFont(QFont("Roboto", 11, QFont::Normal));
//    addToCartButton->setBackgroundColor(QColor("#cd9d7d"));
//    addToCartButton->setBackgroundMode(Qt::OpaqueMode);
//    addToCartButton->setForegroundColor(QColor("#ffffff"));

//    connect(addToCartButton, &QtMaterialFlatButton::clicked,
//            this, &ItemDetailWidget::addToCart );

//    ui->descLayout->addWidget(addToCartButton);
}

QString ItemDetailWidget::getHtml()
{

    QString html;

    html += "<html><body>";
    //    html += "<table width =\"50%\"><tr><td>";
    html += "<table width =\"100%\"><tr>";

}

void ItemDetailWidget::calc()
{
//    qDebug()<<"calc total";
//    Item->price = Item->weight * LoginValues::goldRate;
//    Item->makingChargeAmount = Item->price * Item->makingChargeRate /100;
//    Item->subTotal = Item->quantity * (Item->price + Item->makingChargeAmount + Item->stoneCharge - Item->discountinAmount);
//    qDebug()<<Item->price<<Item->subTotal;
}

void ItemDetailWidget::setWidgets()
{
    appBar = new QtMaterialAppBar();
    appBar->setBackgroundColor(QColor("#1a3966"));

    QLabel *windowLabel = new QLabel(Item->ItemName);
    QPalette palette = windowLabel->palette();
    palette.setColor(windowLabel->foregroundRole(), QColor("#cd9d7d"));
    windowLabel->setPalette(palette);
    windowLabel->setFont(QFont("Roboto", 18, QFont::Normal));

    backButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "arrow_back"));
    backButton->setParent(appBar);
    backButton->setColor(QColor("#cd9d7d"));
    backButton->setIconSize(QSize(30,30));
    connect(backButton, &QtMaterialIconButton::clicked, this, &ItemDetailWidget::closeWindow);

    QPlainTextEdit *descText = new QPlainTextEdit("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
    descText->setFont(QFont("Roboto", 8, QFont::Normal));
    descText->setWordWrapMode(QTextOption::WordWrap);
    descText->setReadOnly(true);
    descText->setStyleSheet("color: #1a3966");
    descText->setFrameShape(QFrame::NoFrame);
    descText->setMaximumHeight(100);

    appBar->appBarLayout()->addWidget(backButton);
    appBar->appBarLayout()->addWidget(windowLabel);
    appBar->appBarLayout()->addStretch(1);

    ui->appBarLayout->addWidget(appBar);
    ui->appBarLayout->addWidget(descText);

}

void ItemDetailWidget::setWidgetData()
{
    priceLabel->setVariable(&Item->price);
    totalLabel->setVariable(&Item->subTotal);
}

void ItemDetailWidget::closeWindow()
{
    this->close();
}

void ItemDetailWidget::addToCart()
{
    emit addItemToCart(Item);
    closeWindow();
}



