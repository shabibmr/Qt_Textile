#include "itemselectorwidget.h"
#include "ui_itemselectorwidget.h"

ItemSelectorWidget::ItemSelectorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemSelectorWidget)
{
    ui->setupUi(this);
    this->setObjectName("background");
    this->setStyleSheet("QWidget#background{background-color:#ffffff}");
    //    this->setMinimumSize(800,700);
    this->setWindowModality(Qt::ApplicationModal);
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                          qApp->primaryScreen()->geometry()));
    qDebug()<<qApp->primaryScreen()->geometry();
    this->setWindowTitle("");

    itemHelper = new SalesInventoryItemDatabaseHelper;
    voucher = new GeneralVoucherDataObject();
    nam = new QNetworkAccessManager(this);

    setWidgets();
    setFilterList();
    //    displayItems();

    //      ui->appBarLayout->addWidget(Widget);
    
}

ItemSelectorWidget::~ItemSelectorWidget()
{
    delete ui;
}

void ItemSelectorWidget::displayItems()
{
    //    itemsList = itemHelper->getInventoryItemsByGroupModel("0K52");
    itemsList = itemHelper->getInventoryItemsQueryModel();


    int cnt=0;

    int width = gridWidget->width()/2 - 20;
    int height = gridWidget->height()/2 - 20;

    for(int j=0;j<6;j++){
        for(int i=0;i<itemsList->rowCount();i++){
            QString url = ":/images/" + itemsList->record(i).value(1).toString() + ".PNG";
            QToolButton* item = new QToolButton();
            //            item->setIcon(QIcon(url));
            item->setIcon(QPixmap::fromImage(readImage(itemsList->record(i).value(1).toString() )));
            item->setIconSize(QSize(width,height));
            item->setAutoRaise(true);
            item->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            item->setText("\u20B9" + itemsList->record(i).value(3).toString() );
            item->setFixedSize(width ,height);

            connect(item, &QToolButton::clicked, this, [=](bool){
                itemClicked(itemsList->record(i).value(0).toString());
            });


            QWidget *Img1widget = new QWidget;
            QHBoxLayout *lay = new QHBoxLayout;
            Img1widget->setLayout(lay);
            Img1widget->setFixedSize(width,height);
            lay->addWidget(item);
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

            itemsGrid->addWidget(Img1widget, cnt/4,cnt%4);

            cnt++;
        }
    }
}

void ItemSelectorWidget::itemClicked(QString itemID)
{
    //    inventoryItemDataModel *itemData = itemHelper->getInventoryItemByIDasPtr(itemID);
    //    itemData->quantity = 1;
    //    itemData->makingChargeRate = 20;
    //    itemData->stoneCharge = 1000;
    //    itemData->weight = 20;
    //    ItemDetailWidget *itemDetail = new ItemDetailWidget(itemData);
    //    itemDetail->setWindowFlag(Qt::FramelessWindowHint);
    //    itemDetail->show();
    //    connect(itemDetail, &ItemDetailWidget::addItemToCart, this, &ItemSelectorWidget::addToCart);
}

void ItemSelectorWidget::setWidgets()
{

    appBar = new QtMaterialAppBar();
    appBar->setBackgroundColor(QColor(darkColor));
    QLabel *windowLabel = new QLabel("Collection");
    QPalette palette = windowLabel->palette();
    palette.setColor(windowLabel->foregroundRole(), QColor(lightColor));
    windowLabel->setPalette(palette);
    windowLabel->setFont(QFont("Roboto", 18, QFont::Normal));
    filterButton = new QtMaterialIconButton(QtMaterialTheme::icon("content", "filter_list"));
    filterButton->setParent(appBar);
    filterButton->setColor(QColor(lightColor));
    filterButton->setIconSize(QSize(30,30));
    appBar->appBarLayout()->addWidget(filterButton);
    appBar->appBarLayout()->addSpacing(350);
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
    connect(Cart, &QtMaterialFlatButton::pressed, this, &ItemSelectorWidget::openCartWidget);

    Store = new QtMaterialFlatButton;
    Store->setText("Store");
    Store->setForegroundColor(QColor(lightColor));

    appBar->appBarLayout()->addWidget(Store);
    appBar->appBarLayout()->addWidget(Notifications);
    appBar->appBarLayout()->addWidget(Cart);

    ui->appBarLayout->addWidget(appBar);


    filterLayout = new QVBoxLayout;
    filterDrawer = new QtMaterialDrawer(this);
    filterDrawer->setClickOutsideToClose(true);
    filterDrawer->setOverlayMode(true);
    filterDrawer->setDrawerLayout(filterLayout);

    connect(filterButton, &QtMaterialIconButton::clicked, filterDrawer, &QtMaterialDrawer::openDrawer);


    gridWidget= new QWidget();
    itemsGrid = new QGridLayout(gridWidget);
    itemsGrid->setHorizontalSpacing(30);
    itemsGrid->setVerticalSpacing(30);
    //    itemsGrid->addWidget(new QLabel("check"));
    displayItems();
    gridWidget->setLayout(itemsGrid);

    scrollBar = new QtMaterialScrollBar;
    scrollBar->setOrientation(Qt::Vertical);

    scrollArea = new QScrollArea;
    scrollArea->setWidget(gridWidget);

    scrollArea->addScrollBarWidget(scrollBar, Qt::AlignRight);
    //    scrollArea->setVerticalScrollBar(scrollBar);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(scrollArea);

    //    ui->itemsLayout->addSpacing(200);
    ui->itemsLayout->addLayout(vLayout)    ;
    //    ui->itemsLayout->addWidget(scrollArea);
    //    ui->itemsLayout->addWidget(scrollBar);

}

void ItemSelectorWidget::addToCart(inventoryItemDataModel *item)
{
    CompoundItemDataObject compItem;
    compItem.BaseItem = *item;
    voucher->InventoryItems.append(compItem);
    Cart->setText(QString::number(voucher->InventoryItems.size()));
}

void ItemSelectorWidget::openCartWidget()
{

    //    QHBoxLayout *lay = new QHBoxLayout();
    //    QWidget *cartParentWidget = new QWidget();
    //    cartParentWidget->setLayout(lay);
    //    cartParentWidget->setAttribute(Qt::WA_TranslucentBackground);
    //    cartParentWidget->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    //    cartParentWidget->setStyleSheet("background: #fff9f3");
    //    cartParentWidget->setFocus();

    CartWidget *cartWidget = new CartWidget(voucher);
    cartWidget->setWindowFlags(/*Qt::WindowStaysOnTopHint|*/Qt::FramelessWindowHint);
    //    cartWidget->setAttribute(Qt::WA_NoSystemBackground);
    cartWidget->setWindowModality(Qt::WindowModal);
    cartWidget->setFocus(Qt::ActiveWindowFocusReason);

    //    lay->addWidget(cartWidget);

    //    QGraphicsDropShadowEffect *wndShadow = new QGraphicsDropShadowEffect;
    //    wndShadow->setBlurRadius(9.0);
    //    wndShadow->setColor(QColor(0, 0, 0, 160));
    //    wndShadow->setXOffset(4.0);
    //    wndShadow->setYOffset(4.0);

    //    CustomShadowEffect *bodyShadow = new CustomShadowEffect();
    //    bodyShadow->setBlurRadius(20.0);
    //    bodyShadow->setDistance(6.0);
    //    bodyShadow->setColor(QColor(0, 0, 0, 80));
    //    cartParentWidget->setAutoFillBackground(true);
    //    cartParentWidget->setGraphicsEffect(bodyShadow);
    cartWidget->show();

}

void ItemSelectorWidget::setFilterList( )
{

    filterLayout->addSpacing(20);

    QLabel *filterLabel = new QLabel("FILTER BY");
    filterLabel->setFont(boldFont);
    filterLabel->setStyleSheet("QLabel { color :" + darkColor + ";  }");
    filterLayout->addWidget(filterLabel);

    filterLayout->addSpacing(20);

    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("QFrame  { color :" + darkColor + ";  }");
    filterLayout->addWidget(line);

    QtMaterialFlatButton *Collection = new QtMaterialFlatButton(" Collection");
    Collection->setIcon(QtMaterialTheme::icon("content", "add"));
    Collection->setForegroundColor(QColor(darkColor));
    CollectionExpanded = false;
    filterLayout->addWidget(Collection);

    collectionFilterLayout = new QVBoxLayout;
    filterLayout->addLayout(collectionFilterLayout);

    connect(Collection, &QtMaterialFlatButton::clicked, this, [=](bool val){
        if(!CollectionExpanded){
            CollectionExpanded = true;
            Collection->setIcon(QtMaterialTheme::icon("content", "remove"));
            showCollectionFilter(true);
        }
        else{
            CollectionExpanded = false;
            Collection->setIcon(QtMaterialTheme::icon("content", "add"));
            clearLayout(collectionFilterLayout);
        }
    });


    QtMaterialFlatButton *Price = new QtMaterialFlatButton(" Price");
    Price->setIcon(QtMaterialTheme::icon("content", "add"));
    Price->setForegroundColor(QColor(darkColor));
    priceExpanded = false;
    filterLayout->addWidget(Price);

    priceFilterLayout = new QVBoxLayout;
    filterLayout->addLayout(priceFilterLayout);

    connect(Price, &QtMaterialFlatButton::clicked, this, [=](bool val){
        if(!priceExpanded){
            priceExpanded = true;
            Price->setIcon(QtMaterialTheme::icon("content", "remove"));
            showPriceFilter();
        }
        else{
            priceExpanded = false;
            Price->setIcon(QtMaterialTheme::icon("content", "add"));
            clearLayout(priceFilterLayout);
        }
    });


    QtMaterialFlatButton *MakingCharge = new QtMaterialFlatButton(" Making Charge ");
    MakingCharge->setIcon(QtMaterialTheme::icon("content", "add"));
    MakingCharge->setForegroundColor(QColor(darkColor));
    mcExpanded = false;
    filterLayout->addWidget(MakingCharge);

    mcFilterLayout = new QHBoxLayout;
    filterLayout->addLayout(mcFilterLayout);
    connect(MakingCharge, &QtMaterialFlatButton::clicked, this, [=](bool val){
        if(!mcExpanded){
            mcExpanded = true;
            MakingCharge->setIcon(QtMaterialTheme::icon("content", "remove"));
            showMCFilter();
        }
        else{
            mcExpanded = false;
            MakingCharge->setIcon(QtMaterialTheme::icon("content", "add"));
            clearLayout(mcFilterLayout);
        }
    });

    QtMaterialFlatButton *StoneCharge = new QtMaterialFlatButton(" Stone Charge ");
    StoneCharge->setIcon(QtMaterialTheme::icon("content", "add"));
    StoneCharge->setForegroundColor(QColor(darkColor));
    scExpanded = false;
    connect(StoneCharge, &QtMaterialFlatButton::clicked, this, [=](bool val){
        if(!scExpanded){
            scExpanded = true;
            StoneCharge->setIcon(QtMaterialTheme::icon("content", "remove"));
        }
        else{
            scExpanded = false;
            StoneCharge->setIcon(QtMaterialTheme::icon("content", "add"));
        }
    });
    filterLayout->addWidget(StoneCharge);

    filterLayout->addStretch(1);

    filterLayout->setAlignment(filterLabel, Qt::AlignCenter);
    filterLayout->setAlignment(line, Qt::AlignLeft);
    filterLayout->setAlignment(Collection, Qt::AlignLeft);
    filterLayout->setAlignment(MakingCharge, Qt::AlignLeft);
    filterLayout->setAlignment(StoneCharge, Qt::AlignLeft);
    filterLayout->setAlignment(Price, Qt::AlignLeft);
}

void ItemSelectorWidget::closeWindow()
{
    this->close();
}

void ItemSelectorWidget::showCollectionFilter(bool show)
{

    all = new QtMaterialFlatButton("ALL");
    all->setForegroundColor(QColor(darkColor));
    all->setFont(subTextFont);
    collectionFilterLayout->addWidget(all);
    collectionFilterLayout->setAlignment(all, Qt::AlignHCenter);

}

void ItemSelectorWidget::showPriceFilter()
{

    //    priceSlider = new QtMaterialSlider(this);
    QSlider *priceSlider = new QSlider;
    priceSlider->setMinimum(2000);
    priceSlider->setMaximum(100000);
    priceSlider->setMaximumWidth(200);
    priceSlider->setTracking(true);
    priceSlider->setStyleSheet("QSlider::groove:horizontal { border: 1px solid "+darkColor+"; background: white; height: 10px; border-radius: 4px; }  QSlider::sub-page:horizontal { background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,     stop: 0 #fff, stop: 1  #fff); background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,     stop: 0  #fff, stop: 1  #fff); border: 1px solid  "+darkColor+"; height: 10px; border-radius: 4px; }  QSlider::add-page:horizontal { background: #fff; border: 1px solid #777; height: 10px; border-radius: 4px; }  QSlider::handle:horizontal { background: qlineargradient(x1:0, y1:0, x2:1, y2:1,     stop:0 "+darkColor+", stop:1 "+darkColor+"); border: 1px solid "+darkColor+"; width: 13px; margin-top: -2px; margin-bottom: -2px; border-radius: 4px; }  QSlider::handle:horizontal:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:1,     stop:0  "+lightColor+", stop:1  "+lightColor+"); border: 1px solid  "+lightColor+"; border-radius: 4px; }  QSlider::sub-page:horizontal:disabled { background: #bbb; border-color: #999; }  QSlider::add-page:horizontal:disabled { background: #eee; border-color: #999; }  QSlider::handle:horizontal:disabled { background: #eee; border: 1px solid #aaa; border-radius: 4px; }");/*
//    priceSlider->setThumbColor(QColor(darkColor));
//    priceSlider->setTrackColor(QColor(lightColor));*/
    priceSlider->setOrientation(Qt::Horizontal);
    //    priceSlider->setTickInterval(1000);
    priceSlider->setPageStep(1000);
    priceSlider->setValue(5000);
    priceFilterLayout->addWidget(priceSlider);
    priceFilterLayout->setAlignment(priceSlider, Qt::AlignHCenter);
    priceFilterLayout->addWidget(priceSlider);

}

void ItemSelectorWidget::showMCFilter()
{
    mcSlider = new QtMaterialSlider;
    mcSlider->setOrientation(Qt::Horizontal);
    mcSlider->setMinimum(0);
    mcSlider->setMaximum(30);
    mcFilterLayout->addWidget(mcSlider);

}

void ItemSelectorWidget::clearLayout(QLayout *lay)
{
    QLayoutItem* item;
    while ( ( item = lay->takeAt( 0 ) ) != NULL )
    {
        delete item->widget();
        delete item;
    }

}

QImage ItemSelectorWidget::readImage(QString name)
{

    QUrl url("http://localhost:8099/Images/"+name+"1.png");
    QNetworkReply* reply = nam->get(QNetworkRequest(url));

    QEventLoop eventloop;
    connect(reply, &QNetworkReply::finished, &eventloop, &QEventLoop::quit );
    eventloop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        qDebug()<<"no error"<<reply;
        //        QImageReader imageReader(reply);
        //        imageReader.setAutoDetectImageFormat (false);
        //        QImage pic = imageReader.read();

        QByteArray      imageData   = reply->readAll();
        QImage          pic;
        pic.loadFromData(imageData);
        return pic;

    }
    else {
        QImage(":/images/download.png");
    }

}
