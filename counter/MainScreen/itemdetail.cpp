#include "itemdetail.h"
#include "ui_itemdetail.h"
#include <QScreen>


ItemDetail::ItemDetail(GeneralVoucherDataObject *voucher,
                       int row,
                       int transactionType,
                       SalesInventoryItemDatabaseHelper *itemHelper,
                       UomDataBaseHelper *uomHelper,
                       PricelistDatabaseHelper *priceHelper,
                       GodownDatabaseHelper *godownHelper,
                       QString priceListId, QWidget *parent):
    QWidget(parent),
    ui(new Ui::ItemDetail)
{
    ui->setupUi(this);

    this->voucher = voucher;
    this->Item = &voucher->InventoryItems[row];
    this->row = row;
    this->itemHelper = itemHelper;
    this->priceHelper = priceHelper;
    this->uomHelper = uomHelper;
    this->godownHelper = godownHelper;
    this->transactionType = transactionType;
    windowTitle = Item->BaseItem.ItemName;
    this->setWindowTitle("Item Editor");
    this->setWindowFlag(Qt::Popup);
    //    this->setFixedHeight(700);

    setMaximumWidth(this->size().width()*.75);
    setMaximumHeight(this->size().height()*.8);
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                          qApp->primaryScreen()->geometry()));

    this->setStyleSheet("QWidget#editor{background-color: white }");
    QPalette pal = palette();
    pal.setColor(QPalette::Background,QColor("white"));
    setAutoFillBackground(true);
    setPalette(pal);

    this->priceListId = priceListId;
    this->priceListName = priceHelper->getPriceListNameByID(this->priceListId);

//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    setDefaultValues();
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    initGlobalObjects();
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    setBatchTab();
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    setWidgets();
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    setTabs();
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    setFocusOrder();
//    qDebug()<<Q_FUNC_INFO<<__LINE__;

}

ItemDetail::~ItemDetail()
{
    delete ui;
}

void ItemDetail::initGlobalObjects()
{
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    itemPricesWid = new ItemPriceDetailsWidget(Item, itemHelper);
//    qDebug()<<Q_FUNC_INFO<<__LINE__;

    AmountEdit = new GMMaterialNumberEdit(&Item->BaseItem.grandTotal);
    AmountEdit->setLabel("Amount");
    AmountEdit->setReadOnly(true);

    taxAmount = new GMMaterialNumberEdit(&Item->BaseItem.taxAmount);
    taxAmount->setLabel("Tax Amount");
    taxAmount->setReadOnly(true);

    discAmount = new GMMaterialNumberEdit(&Item->BaseItem.discountinAmount);
    discAmount->setLabel("Discount Amount");
    //    discAmount->setReadOnly(true)    ;
    connect(discAmount, &GMMaterialNumberEdit::editingFinished, [=]{

        if(Item->BaseItem.discountinAmount > 0){
            Item->BaseItem.discountPercentage =
                    Item->BaseItem.discountinAmount * 100
                    / (Item->BaseItem.price *
                       Item->BaseItem.quantity);
        }
        resetWidgets();
    });

//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    QSqlQueryModel *model = uomHelper->getUomModelByItem(Item->BaseItem.ItemID);
    QStringList uomList;
    for(int i=0;i< model->rowCount(); i++){
        uomList.append(model->record(i).value(1).toString());
    }

    uom = new GMMaterialComboBox(&Item->BaseItem.uomObject.UomName, uomList );
    uom->setLabel("UOM");
    connect(uom, &GMMaterialComboBox::selectedItem, this, [=](){
//        qDebug()<<Q_FUNC_INFO<<__LINE__;
        Item->BaseItem.uomObject = uomHelper->getUomObjectByID(uomHelper->getUOMIDByName(Item->BaseItem.uomObject.UomName));
        Item->BaseItem.uomObject.convRate = model->record(uomList.indexOf(Item->BaseItem.uomObject.UomName)).value(2).toFloat();
        emit updateItem();
        resetWidgets();
        itemPricesWid->setValues();

    });

//    qDebug()<<Q_FUNC_INFO<<__LINE__<<transactionType;
    switch (transactionType) {
    case 1:
    {
        if(Item->BaseItem.fromGodownID.length() == 0)
            Item->BaseItem.fromGodownID = godownHelper->getDefaultGodown();
//        qDebug()<<"%%%% FROM LOC"<<Item->BaseItem.fromGodownID;
        godownWidget = new GMGodownLineMaterial(godownHelper,
                                                godownHelper->getGodownForSearchModel(),
                                                &Item->BaseItem.fromGodownID);
        godownWidget->setGodown(&Item->BaseItem.fromGodownID);
        break;
    }
    case 2:
    {
        if(Item->BaseItem.toGodownID.length() == 0)
            Item->BaseItem.toGodownID = godownHelper->getDefaultGodown();
        godownWidget = new GMGodownLineMaterial(godownHelper,
                                                godownHelper->getGodownForSearchModel(),
                                                &Item->BaseItem.toGodownID);
        godownWidget->setGodown(&Item->BaseItem.toGodownID);
        break;
    }
    case 3: //physical stock entry -
    {
        qDebug()<<__LINE__<<Item->BaseItem.crQty<<Item->BaseItem.drQty;
        if(Item->BaseItem.crQty > 0){
            if(Item->BaseItem.toGodownID.length() == 0)
                Item->BaseItem.toGodownID = godownHelper->getDefaultGodown();
            qDebug()<<__LINE__<<Item->BaseItem.toGodownID;
            godownWidget = new GMGodownLineMaterial(godownHelper,
                                                    godownHelper->getGodownForSearchModel(),
                                                    &Item->BaseItem.toGodownID);
            godownWidget->setGodown(&Item->BaseItem.toGodownID);
        }
        else  {
            if(Item->BaseItem.fromGodownID.length() == 0)
                Item->BaseItem.fromGodownID = godownHelper->getDefaultGodown();
            qDebug()<<"%%%% FROM LOC"<<Item->BaseItem.fromGodownID;
            godownWidget = new GMGodownLineMaterial(godownHelper,
                                                    godownHelper->getGodownForSearchModel(),
                                                    &Item->BaseItem.fromGodownID);
            godownWidget->setGodown(&Item->BaseItem.fromGodownID);
        }
        qDebug()<<__LINE__<<Item->BaseItem.fromGodownID<<Item->BaseItem.toGodownID;
        break;
    }
    }

//    qDebug()<<Q_FUNC_INFO<<__LINE__;

    godownWidget->setLabel("Godown");
    godownWidget->setFixedWidth(100);
    godownWidget->hide();

//    qDebug()<<Q_FUNC_INFO<<__LINE__;

    priceEdit = new GMMaterialNumberEdit(&price);
    priceEdit->setLabel("Rate");
    connect(priceEdit,&GMMaterialNumberEdit::editFinished, this, [=](){
        Item->BaseItem.price = price/Item->BaseItem.uomObject.convRate;
        resetWidgets();
    });

//    qDebug()<<Q_FUNC_INFO<<__LINE__;

    quantityEdit = new GMMaterialNumberEdit(&Item->BaseItem.quantity);
    quantityEdit->setLabel("Quantity");
    connect(quantityEdit,&GMMaterialNumberEdit::editFinished, this,  &ItemDetail::resetWidgets);
//    qDebug()<<Q_FUNC_INFO<<__LINE__;

    discPercent = new GMMaterialNumberEdit(&Item->BaseItem.discountPercentage);
    discPercent->setLabel("Discount %");
    connect(discPercent,&GMMaterialNumberEdit::editFinished, this,[=]{
        Item->BaseItem.discountinAmount =
                Item->BaseItem.price *
                Item->BaseItem.quantity *
                Item->BaseItem.discountPercentage /100;
        resetWidgets();
    });

//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    taxRate = new GMMaterialNumberEdit(&Item->BaseItem.taxRate);
    taxRate->setLabel("Tax Rate");
    connect(taxRate,&GMMaterialNumberEdit::editFinished, this,  &ItemDetail::resetWidgets);

    narrationEdit = new GMMaterialLineEdit(&Item->BaseItem.narration);
    narrationEdit->setLabel("Narration");

}

void ItemDetail::setWidgets()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    qDebug()<<"3";
    QLabel *windowLabel = new QLabel(windowTitle);
    appBar = new QtMaterialAppBar();
    appBar->setBackgroundColor(QColor());
    appBar->setFixedHeight(50);

    QLabel *groupLabel = new QLabel(Item->BaseItem.GroupName);
    groupLabel->setStyleSheet("QLabel{background-color : #00bcd4 ; color: white; border: none}");
    groupLabel->setFont(QFont("Calibri", 9, QFont::Normal));

    backButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "arrow_back"));
    backButton->setParent(appBar);
    backButton->setColor(QColor(245,0,87));
    backButton->setIconSize(QSize(30,30));

    okButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("content", "send"));
    okButton->setMini(true);
    okButton->setXOffset(10);
    okButton->setYOffset(10);
    okButton->setParent(this);
    okButton->setToolTip("Add changes");

    QObject::connect(backButton,&QPushButton::clicked,
                     this,&ItemDetail::closeEditor);
    QObject::connect(okButton,&QPushButton::clicked,
                     this,[=](){
        emit updateItem();
        qDebug()<<"save";
        this->close();
    });

    appBar->appBarLayout()->addWidget(backButton);
    appBar->appBarLayout()->addWidget(windowLabel);
    appBar->appBarLayout()->addStretch(1);
    appBar->appBarLayout()->addWidget(groupLabel, 1,Qt::AlignBottom);
    QPalette palette = windowLabel->palette();
    palette.setColor(windowLabel->foregroundRole(), Qt::white);
    windowLabel->setPalette(palette);

    windowLabel->setFont(QFont("Calibri", 14, QFont::Normal));

    QVBoxLayout *layout = new QVBoxLayout;
    QWidget *canvas = new QWidget;


    layout->setContentsMargins(0, 0, 0, 0);

    layout = new QVBoxLayout;
    canvas->setLayout(layout);
    canvas->setMaximumHeight(160);
    layout->addWidget(appBar);

    tabs = new QtMaterialTabs();
    tabs->setBackgroundColor(QColor("white"));
    tabs->setTextColor(QColor("#00bcd4"));
    tabs->setFixedHeight(50);
    layout->addWidget(tabs);

    layout->addStretch(1);

    mainWidget = new QWidget(this);
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout(mainWidget);
    mainWidget->setLayout(mainLayout);

    QPointer<QVBoxLayout> dataLayout = new QVBoxLayout;
    QPointer<QHBoxLayout> dataLayout1 = new QHBoxLayout;
    QPointer<QHBoxLayout> dataLayout2 = new QHBoxLayout;
    QPointer<QHBoxLayout> dataLayout3 = new QHBoxLayout;
    QPointer<QHBoxLayout> dataLayout4 = new QHBoxLayout;
    QPointer<QHBoxLayout> dataLayout5 = new QHBoxLayout;
    QPointer<QHBoxLayout> dataLayout6 = new QHBoxLayout;

    QPointer<QHBoxLayout> priceLayout = new QHBoxLayout;
    priceLayout->addWidget(itemPricesWid);

    //    mainLayout->addWidget(canvas);
    mainLayout->addLayout(dataLayout);

    dataLayout1->addWidget(godownWidget);
    dataLayout1->addWidget(quantityEdit);
    dataLayout1->addWidget(uom);
    dataLayout1->addWidget(priceEdit);
    dataLayout3->addWidget(discPercent);
    dataLayout3->addWidget(discAmount);
    dataLayout4->addWidget(taxRate);
    dataLayout4->addWidget(taxAmount);
    dataLayout5->addWidget(AmountEdit);
    dataLayout5->addWidget(narrationEdit);

    dataLayout->addLayout(dataLayout1);
    dataLayout->addLayout(dataLayout2);
    dataLayout->addLayout(dataLayout3);
    dataLayout->addLayout(dataLayout4);
    dataLayout->addLayout(dataLayout5);
    //    dataLayout->addLayout(dataLayout6);
    dataLayout->addLayout(priceLayout);

    mainLayout->addWidget(okButton);

    //    mainWidget->hide();

//    qDebug()<<"239";

    godownAllocWid = new GodownAllocationWidget(voucher, row, this);
    godownAllocWid->hide();
    mainWidget->hide();





    ui->verticalLayout->addWidget(canvas);
    ui->verticalLayout->addWidget(mainWidget);
    ui->verticalLayout->addWidget(godownAllocWid);
    ui->verticalLayout->addWidget(batchWid);

//    qDebug()<<"251"<<Q_FUNC_INFO;

}

void ItemDetail::setTabs()
{
    if(transactionType < 3)
        tabs->addTab("Details");


    tabs->addTab("Godown Allocation");

    if(Item->BaseItem.isSerailNumbered || Item->BaseItem.isBatchProcessed)
        tabs->addTab("Batch Information");

    tabs->setCurrentTab(0);

    if(transactionType == 3)
        godownAllocWid->show();
    else {
        mainWidget->show();
    }

    connect(tabs, &QtMaterialTabs::currentChanged, this, [=](int index){
        qDebug()<<"tab selected"<<index;

        if(transactionType < 3){
            switch (index) {
            case 0:
            {
                mainWidget->show();
                godownAllocWid->hide();
                batchWid->hide();
                break;
            }
            case 1:
            {
                mainWidget->hide();
                godownAllocWid->show();
                batchWid->hide();
                break;
            }
            case 2:
            {
                mainWidget->hide();
                godownAllocWid->hide();
                batchWid->show();
                break;
            }
            }
        }
        else {
            mainWidget->hide();
            godownAllocWid->show();
            batchWid->hide();

        }

    });

}

void ItemDetail::resetWidgets()
{
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    emit updateItem();

    priceEdit->blockSignals(true);
    quantityEdit->blockSignals(true);
    discPercent->blockSignals(true);
    discAmount->blockSignals(true);
    taxAmount->blockSignals(true);
    AmountEdit->blockSignals(true);
    price = Item->BaseItem.price * Item->BaseItem.uomObject.convRate;
    priceEdit->setVariable(&price);

    Item->BaseItem.quantity = Item->BaseItem.quantity / Item->BaseItem.uomObject.convRate;
    quantityEdit->setVariable(&Item->BaseItem.quantity);
    discPercent->setVariable(&Item->BaseItem.discountPercentage);
    discAmount->setVariable(&Item->BaseItem.discountinAmount);
    taxAmount->setVariable(&Item->BaseItem.taxAmount);
    AmountEdit->setVariable(&Item->BaseItem.grandTotal);
    priceEdit->blockSignals(false);
    quantityEdit->blockSignals(false);
    discPercent->blockSignals(false);
    discAmount->blockSignals(false);
    taxAmount->blockSignals(false);
    AmountEdit->blockSignals(false);

}


void ItemDetail::closeEditor()
{
    this->close();
}

void ItemDetail::setDefaultValues()
{
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    price = Item->BaseItem.price;
    quantity = Item->BaseItem.quantity;

}

void ItemDetail::setFocusOrder()
{
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    setTabOrder(quantityEdit,uom);
    setTabOrder(uom,discPercent);
    setTabOrder(discPercent,okButton);
    quantityEdit->setFocus();
}




void ItemDetail::setPrices()
{
    priceListId = QString::number(priceHelper->getPriceListIDByName(priceListName));
    Item->BaseItem.PriceLevel = priceListId;
    float price = priceHelper->getPriceByItemIDAndUOM(Item->BaseItem.ItemID,
                                                      Item->BaseItem.PriceLevel,
                                                      Item->BaseItem.uomObject._id);
    if(price != -1){
        Item->BaseItem.price = price;
        Item->BaseItem.uomObject.convRate = 1;
    }
    else
        Item->BaseItem.price = Item->BaseItem.price_1;

    resetWidgets();



}

void ItemDetail::setBatchTab()
{
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    batchWid = new QWidget;

    QVBoxLayout *vLayout = new QVBoxLayout;
    batchWid->setLayout(vLayout);

    mfgDateEdit = new GMDateEdit(&Item->BaseItem.manufactureDate);
    expiryDateEdit = new GMDateEdit(&Item->BaseItem.expiry);

    serialNoEdit = new GMMaterialLineEdit(&Item->BaseItem.SerailNumber);
    serialNoEdit->setLabel("Serial No");

    batchCodeEdit = new GMMaterialLineEdit(&Item->BaseItem.ItemCode);
    batchCodeEdit->setLabel("Batch Code");
    if(Item->BaseItem.isSerailNumbered){
        vLayout->addWidget(serialNoEdit);
        vLayout->addWidget(mfgDateEdit);
        vLayout->addWidget(expiryDateEdit);
    }
    else if(Item->BaseItem.isBatchProcessed){
        vLayout->addWidget(batchCodeEdit);
        vLayout->addWidget(mfgDateEdit);
        vLayout->addWidget(expiryDateEdit);
    }



}
