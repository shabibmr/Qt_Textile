#include "inventory_sidebar.h"
#include "ui_inventory_sidebar.h"
#include "inventory/masters/sales_inventory_items/additems.h"
#include "counter/MainScreen/restuarantcountermainwindow.h";

Inventory_SideBar::Inventory_SideBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Inventory_SideBar)
{
    ui->setupUi(this);
    qDebug()<<"Creating Inventory_SideBar";
    //    dash = new DefaultDashBoard(this);
    //  ui->widgetLayout->addWidget(dash);

    ui->scrollArea->setStyleSheet("background-color:qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:0.4, stop:0 #424242, stop:1 #424242)");

    ui->verticalLayout_3->setAlignment(Qt::AlignTop);
    sectionMaster = new Section("Masters", 300,new QIcon(":/icons/gm.ico"), this);
    sectionTransaction = new Section("Transactions", 300,new QIcon(":/icons/gm.ico"), this);
    sectionReports = new Section("Reports", 300,new QIcon(":/icons/gm.ico"), this);
    sectionItems = new Section("Items", 300,new QIcon(":/icons/gm.ico"), this);




    ItemMaster = new SideBarButton("Inventory Items");
    ItemGroup = new SideBarButton("Inventory Groups");
    itemSelect = new SideBarButton("Collection");
    StockEntryButton = new SideBarButton("Physical Stock Entry");
    StockJournal = new SideBarButton("Stock Journal");
    MaterialIn = new SideBarButton("Material In");
    MaterialOut = new SideBarButton("Material Out");
    DeliveryNote = new SideBarButton("Delivery Notes");
    ReceiptNote = new SideBarButton("Receipt Notes");
    Property = new SideBarButton("Item Properties");
    PriceList = new SideBarButton("Price List");
    UOMButton = new SideBarButton("U.O.M");
    Godown = new SideBarButton("Godown");
    brandsToolButt = new SideBarButton("Brands");
    InventoryOpeningEntry = new SideBarButton("Opening Stock Entry");
    StockRequestButton = new SideBarButton("Stock Request/Issue");

    TimeButton = new SideBarButton("Time Sales");
    modeButton = new SideBarButton("Mode of Service");
    //    godownwiseStockButton = new SideBarButton(this);

    TransSummary = new SideBarButton("Transaction Summary");


    QMenu *menu = new QMenu();
    QAction *testAction1 = new QAction("Sales", this);
    QAction *testAction2 = new QAction("Purchase", this);
    QAction *testAction3 = new QAction("Stock Adjustments", this);
    QAction *testAction4 = new QAction("Wastage", this);
    QAction *testAction5 = new QAction("Credit Note", this);
    QAction *testAction6 = new QAction("Debit Note", this);
    QAction *testAction7 = new QAction("Quotaiton", this);
    QAction *testAction9 = new QAction("Receipt Note", this);

    testAction1->setProperty("Type",GMVoucherTypes::SalesVoucher);
    testAction2->setProperty("Type",GMVoucherTypes::PurchaseVoucher);
    testAction3->setProperty("Type",GMVoucherTypes::StockEntry);
    testAction5->setProperty("Type",GMVoucherTypes::CreditNote);
    testAction6->setProperty("Type",GMVoucherTypes::DebitNote);
    testAction7->setProperty("Type",GMVoucherTypes::Proforma);
    testAction4->setProperty("Type",GMVoucherTypes::DefectiveVoucher);
    testAction9->setProperty("Type",GMVoucherTypes::ReceiptNote);

    menu->addAction(testAction1);
    menu->addAction(testAction2);
    menu->addAction(testAction3);
    menu->addAction(testAction4);
    menu->addAction(testAction5);
    menu->addAction(testAction6);
    menu->addAction(testAction9);


    QObject::connect(testAction1,SIGNAL(triggered()),this,SLOT(transactionReportClicked()));
    QObject::connect(testAction2,SIGNAL(triggered()),this,SLOT(transactionReportClicked()));
    QObject::connect(testAction3,SIGNAL(triggered()),this,SLOT(transactionReportClicked()));
    QObject::connect(testAction4,SIGNAL(triggered()),this,SLOT(transactionReportClicked()));
    QObject::connect(testAction5,SIGNAL(triggered()),this,SLOT(transactionReportClicked()));
    QObject::connect(testAction6,SIGNAL(triggered()),this,SLOT(transactionReportClicked()));
    QObject::connect(testAction7,SIGNAL(triggered()),this,SLOT(transactionReportClicked()));
    QObject::connect(testAction9,SIGNAL(triggered()),this,SLOT(transactionReportClicked()));

    TransSummary->setMenu(menu);
    TransSummary->setPopupMode(SideBarButton::InstantPopup);

    StockButton = new SideBarButton("Stock Report");
    ProductionRate = new SideBarButton("Production Rate");
    workOrderButton = new SideBarButton("Work Orders");
    WastageButton= new SideBarButton("Wastage Entries");
    MovementAnalysisButton = new SideBarButton("Movement Analysis");
    ItemAnalysisButton = new SideBarButton("Item Analysis");
    GroupwiseSales = new SideBarButton("Groupwise Sales");
    GroupwisePurchase = new SideBarButton("Groupwise Purchase");
    DebitNoteReportButton = new SideBarButton("Debit Notes");
    CreditNoteReportButton = new SideBarButton("Credit Notes");
    WastageReportButton = new SideBarButton("Wastage Report");
    WOReportButton = new SideBarButton("Work Order Report");
    PhysicalStockRegistersButton= new SideBarButton("Physical Stock Register");
    GodownWiseReportButton = new SideBarButton("Godownwise Report");
    SalesBillsPendingButton = new SideBarButton("Sales Bills Pending");

    NegativeStockButton = new SideBarButton("Negative Stock");
    ReorderLevelButton = new SideBarButton("Below Reorder Level");
    ItemeiseSOButton = new SideBarButton("Itemwise Entry");
    ItemeisePOButton  = new SideBarButton("Purchase Entry");
    ItemWiseSales = new SideBarButton("Itemwise Sales");
    ledgerwiseItemsButton = new SideBarButton("Ledgerwise Item Movement");
    dailyCostingButton = new SideBarButton("Daily Costing");

    PurchaseBillsPendingButton = new SideBarButton("Purchase Bills Pending");
    POSButton = new SideBarButton("DEMO POS");
    itemRepB1 = new SideBarButton("ItemwiseOrder Report");
    //    godownwiseStockButton->setText("Godownwise Stock");
    //    godownwiseStockButton->setFont(font);

    auto* masterLayout = new QVBoxLayout();
    auto* transactionLayout = new QVBoxLayout();
    auto* reportsLayout  = new QVBoxLayout();
    auto* itemsLayout  = new QVBoxLayout();

    //    reportsLayout->setAlignment(Qt::AlignLeft);

    masterLayout ->addWidget(ItemMaster);
    masterLayout ->addWidget(ItemGroup);
    masterLayout ->addWidget(UOMButton);
    masterLayout ->addWidget(brandsToolButt);
    masterLayout ->addWidget(Godown);
    masterLayout ->addWidget(Property);    // Property->hide();
    masterLayout ->addWidget(PriceList);
    masterLayout ->addWidget(InventoryOpeningEntry);

    transactionLayout->addWidget(itemSelect); itemSelect->hide();
    transactionLayout ->addWidget(ItemeiseSOButton);//ItemeiseSOButton->hide();
    transactionLayout ->addWidget(ItemeisePOButton);
    transactionLayout ->addWidget(itemRepB1);

    transactionLayout ->addWidget(StockJournal);
    transactionLayout ->addWidget(workOrderButton);
    transactionLayout ->addWidget(StockRequestButton);
    transactionLayout ->addWidget(StockEntryButton);
    transactionLayout ->addWidget(DeliveryNote);
    transactionLayout ->addWidget(ReceiptNote);
    transactionLayout ->addWidget(MaterialIn);
    transactionLayout ->addWidget(MaterialOut);
    transactionLayout ->addWidget(WastageButton);

    transactionLayout ->addWidget(POSButton);

    reportsLayout ->addWidget(TransSummary);
    reportsLayout ->addWidget(StockButton);
    reportsLayout->addWidget(ledgerwiseItemsButton);
//        reportsLayout->addWidget(dailyCostingButton);
    reportsLayout->addWidget(GroupwiseSales);
    reportsLayout->addWidget(GroupwisePurchase);
    reportsLayout->addWidget(DebitNoteReportButton);
    reportsLayout->addWidget(CreditNoteReportButton);
    reportsLayout->addWidget(WastageReportButton);
    reportsLayout->addWidget(WOReportButton);
    reportsLayout->addWidget(MovementAnalysisButton);
    reportsLayout->addWidget(PhysicalStockRegistersButton);
    reportsLayout->addWidget(GodownWiseReportButton);
    reportsLayout->addWidget(NegativeStockButton);
    reportsLayout->addWidget(ReorderLevelButton);
    reportsLayout->addWidget(SalesBillsPendingButton);
    reportsLayout->addWidget(PurchaseBillsPendingButton);
    reportsLayout->addWidget(ProductionRate);
    reportsLayout->addWidget(modeButton);
    reportsLayout->addWidget(TimeButton);

    //    reportsLayout->setSpacing(20);


    itemsLayout->addWidget(ItemWiseSales);
    itemsLayout->addWidget(ledgerwiseItemsButton);
    itemsLayout->addWidget(dailyCostingButton);
    itemsLayout ->addWidget(ItemAnalysisButton);

    //    reportsLayout->addWidget(godownwiseStockButton);

    ui->verticalLayout_3->addWidget(sectionMaster);
    ui->verticalLayout_3->addWidget(getLine());
    ui->verticalLayout_3->addWidget(sectionTransaction);
    ui->verticalLayout_3->addWidget(getLine());
    ui->verticalLayout_3->addWidget(sectionReports);
    ui->verticalLayout_3->addWidget(getLine());
    ui->verticalLayout_3->addWidget(sectionItems);
    ui->verticalLayout_3->addWidget(getLine());

    sectionMaster->setContentLayout(*masterLayout);
    sectionTransaction->setContentLayout(*transactionLayout);
    sectionReports->setContentLayout(*reportsLayout);
    sectionItems->setContentLayout(*itemsLayout);


    QObject::connect(ItemMaster,SIGNAL(clicked(bool)),this,SLOT(ItemMasterClicked()));
    QObject::connect(ItemGroup,SIGNAL(clicked(bool)),this,SLOT(ItemGroupClicked()));
    QObject::connect(itemSelect,SIGNAL(clicked(bool)),this,SLOT(ItemSelectClicked()));
    QObject::connect(StockEntryButton,SIGNAL(clicked(bool)),this,SLOT(StockEntryClicked()));
    QObject::connect(workOrderButton,SIGNAL(clicked(bool)),this,SLOT(WorkOrderClicked()));
    QObject::connect(WastageButton,SIGNAL(clicked(bool)),this,SLOT(WastageClicked()));

    QObject::connect(StockJournal,SIGNAL(clicked(bool)),this,SLOT(StockJournalClicked()));
    QObject::connect(MaterialIn,SIGNAL(clicked(bool)),this,SLOT(MaterialInClicked()));
    QObject::connect(MaterialOut,SIGNAL(clicked(bool)),this,SLOT(MaterialOutClicked()));
    QObject::connect(DeliveryNote,SIGNAL(clicked(bool)),this,SLOT(DeliveryNoteClicked()));
    QObject::connect(ReceiptNote,SIGNAL(clicked(bool)),this,SLOT(ReceiptNoteClicked()));
    QObject::connect(Property,SIGNAL(clicked(bool)),this,SLOT(PropertyClicked()));
    QObject::connect(PriceList,SIGNAL(clicked(bool)),this,SLOT(PriceListClicked()));
    QObject::connect(UOMButton,SIGNAL(clicked(bool)),this,SLOT(UOMClicked()));
    QObject::connect(StockRequestButton,SIGNAL(clicked(bool)),this,SLOT(StockRequestClicked()));
    QObject::connect(Godown,SIGNAL(clicked(bool)),this,SLOT(GodownClicked()));
    QObject::connect(StockButton,SIGNAL(clicked(bool)),this,SLOT(StockClicked()));
    QObject::connect(ProductionRate,SIGNAL(clicked(bool)),this,SLOT(ProductionRateClicked()));
    QObject::connect(MovementAnalysisButton,SIGNAL(clicked(bool)),this,SLOT(MovementAnalysisClicked()));
    QObject::connect(ItemAnalysisButton,SIGNAL(clicked(bool)),this,SLOT(ItemAnalysisClicked()));
    QObject::connect(GroupwiseSales,SIGNAL(clicked(bool)),this,SLOT(GroupwiseClicked()));
    QObject::connect(GroupwisePurchase,SIGNAL(clicked(bool)),this,SLOT(PurchaseReportClicked()));
    QObject::connect(DebitNoteReportButton,SIGNAL(clicked(bool)),this,SLOT(DebitNoteReportClicked()));
    QObject::connect(CreditNoteReportButton,SIGNAL(clicked(bool)),this,SLOT(CreditNoteReportClicked()));
    QObject::connect(WastageReportButton,SIGNAL(clicked(bool)),this,SLOT(WastageReportClicked()));
    QObject::connect(WOReportButton,SIGNAL(clicked(bool)),this,SLOT(WorkOrderReportClicked()));
    QObject::connect(POSButton,SIGNAL(clicked(bool)),this,SLOT(POSClicked()));


    QObject::connect(PhysicalStockRegistersButton,SIGNAL(clicked(bool)),this,SLOT(PhysicalStockRegistersClicked()));
    QObject::connect(GodownWiseReportButton,SIGNAL(clicked(bool)),this,SLOT(GodownWiseReportClicked()));
    QObject::connect(SalesBillsPendingButton,SIGNAL(clicked(bool)),this,SLOT(SalesBillsPendingClicked()));
    QObject::connect(PurchaseBillsPendingButton,SIGNAL(clicked(bool)),this,SLOT(PurchaseBillsPendingClicked()));
    QObject::connect(NegativeStockButton,SIGNAL(clicked(bool)),this,SLOT(NegativeStockClicked()));
    QObject::connect(ReorderLevelButton,SIGNAL(clicked(bool)),this,SLOT(ReorderLevelClicked()));
    QObject::connect(brandsToolButt,SIGNAL(clicked(bool)),this,SLOT(BrandsClicked()));
    QObject::connect(ItemeiseSOButton,SIGNAL(clicked(bool)),this,SLOT(itemwiseSOClicked()));
    QObject::connect(ItemeisePOButton,SIGNAL(clicked(bool)),this,SLOT(itemwisePOClicked()));

    QObject::connect(InventoryOpeningEntry,SIGNAL(clicked(bool)),this,SLOT(OpeingClicked()));

    QObject::connect(ItemWiseSales,SIGNAL(clicked(bool)),this,SLOT(ItemwiseSalesClicked()));
    QObject::connect(itemRepB1,SIGNAL(clicked(bool)),this,SLOT(itemRep1Clicked()));

    QObject::connect(TimeButton,SIGNAL(clicked(bool)),this,SLOT(timeReportClicked()));
    QObject::connect(dailyCostingButton,SIGNAL(clicked(bool)),this,SLOT(DailyCostingClicked()));
    QObject::connect(modeButton,SIGNAL(clicked(bool)),this,SLOT(ModeOfServiceClicked()));
    QObject::connect(ledgerwiseItemsButton,SIGNAL(clicked(bool)),this,SLOT(LedgerWiseItemsClicked()));

    //    QObject::connect(godownwiseStockButton,SIGNAL(clicked(bool)),this,SLOT(GodownStockClicked()));
    //    QObject::connect(godownwiseMovementButton,SIGNAL(clicked(bool)),this,SLOT(ModeOfServiceClicked()));

    resetAllColors();
    //    setUAC();
    dash = new DefaultDashBoard(this);
    ui->widgetLayout->addWidget(dash);

    ItemAnalysisButton->hide();

}

Inventory_SideBar::~Inventory_SideBar()
{
    delete ui;
}

QFrame *Inventory_SideBar::getLine()
{
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    return line;
}

void Inventory_SideBar::searchText(QString text)
{
    emit searchTextchanged(text);
}

void Inventory_SideBar::resetAllColors()
{
    ItemMaster->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    ItemGroup ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    itemSelect ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    StockEntryButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    StockJournal ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    MaterialIn ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    MaterialOut ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    DeliveryNote ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    ReceiptNote ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    Property ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    PriceList ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    UOMButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    Godown ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    brandsToolButt->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    InventoryOpeningEntry->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    StockRequestButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    workOrderButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    WastageButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    StockButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    MovementAnalysisButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    ItemAnalysisButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    GroupwiseSales ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    GroupwisePurchase ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    DebitNoteReportButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    CreditNoteReportButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    WOReportButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    WastageReportButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    GodownWiseReportButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    SalesBillsPendingButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    PhysicalStockRegistersButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    PurchaseBillsPendingButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    NegativeStockButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    ReorderLevelButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    ItemWiseSales->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    ItemeiseSOButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    ItemeisePOButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    TransSummary->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    ProductionRate->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    dailyCostingButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    TimeButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    modeButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    ledgerwiseItemsButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    POSButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    itemRepB1->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    clearLayout();
}

void Inventory_SideBar::ItemMasterClicked()
{
    resetAllColors();
    ItemMaster->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    invItemWidget = new inventoryItem(this);
    QObject::connect(this,SIGNAL(searchTextchanged(QString)),invItemWidget,SLOT(searchTextChanged(QString)));
    ui->widgetLayout->addWidget(invItemWidget);
    invItemWidget->setFocus();
}

void Inventory_SideBar::ItemGroupClicked()
{
    resetAllColors();
    ItemGroup->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    invGroupWidget = new inventorygroup(this);
    QObject::connect(this,SIGNAL(searchTextchanged(QString)),invGroupWidget,SLOT(searchTextChanged(QString)));

    ui->widgetLayout->addWidget(invGroupWidget);
    invGroupWidget ->setFocus();
}

void Inventory_SideBar::ItemSelectClicked()
{
    resetAllColors();
    itemSelect->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    itemSel = new ItemSelectorWidget();
    //    itemSel->setWindowFlags(Qt::FramelessWindowHint);
    itemSel->showMaximized();
    //    ui->widgetLayout->addWidget(itemSel);
    qDebug()<<"show";
    itemSel->setFocus();
}

void Inventory_SideBar::StockEntryClicked()
{
    resetAllColors();
    StockEntryButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    StockEntryWidget = new StockEntry(this);
    ui->widgetLayout->addWidget(StockEntryWidget);
    StockEntryWidget->setFocus();
}

void Inventory_SideBar::WorkOrderClicked()
{
    resetAllColors();
    workOrderButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    WordOrderListWidget = new WorkOrderList(this);
    ui->widgetLayout->addWidget(WordOrderListWidget);
    WordOrderListWidget->setFocus();
}

void Inventory_SideBar::StockRequestClicked()
{
    resetAllColors();
    StockRequestButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    StockRequestWidget = new StockRequestList(this);
    ui->widgetLayout->addWidget(StockRequestWidget);
    StockRequestWidget->setFocus();
}

void Inventory_SideBar::StockJournalClicked()
{
    resetAllColors();
    StockJournal->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    stockJournalsWidget = new stockJournals(this);
    ui->widgetLayout->addWidget(stockJournalsWidget);
    stockJournalsWidget->setFocus();
}

void Inventory_SideBar::MaterialInClicked()
{
    resetAllColors();
    MaterialIn->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    materialInWidget = new materialInOut(this);
    ui->widgetLayout->addWidget(materialInWidget);
    materialInWidget->setFocus();
}

void Inventory_SideBar::MaterialOutClicked()
{
    resetAllColors();
    MaterialOut->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    materialOutWidget = new materialInOut(this);
    ui->widgetLayout->addWidget(materialOutWidget);
    materialOutWidget->setFocus();
}

void Inventory_SideBar::DeliveryNoteClicked()
{
    resetAllColors();
    DeliveryNote->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    deliveryNotesWidget = new deliveryNotes(false,this);
    ui->widgetLayout->addWidget(deliveryNotesWidget);
    deliveryNotesWidget->setFocus();
}

void Inventory_SideBar::ReceiptNoteClicked()
{
    resetAllColors();
    ReceiptNote->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    ReceiptNotesWidget = new ReceiptNotes(false,this);
    ui->widgetLayout->addWidget(ReceiptNotesWidget);
    ReceiptNotesWidget->setFocus();
}

void Inventory_SideBar::WastageClicked()
{
    resetAllColors();
    WastageButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    DefectiveWidget = new DefectiveList(false,this);
    ui->widgetLayout->addWidget(DefectiveWidget);
    DefectiveWidget->setFocus();
}

void Inventory_SideBar::PropertyClicked()
{
    resetAllColors();
    Property->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    propertyListWidget = new PropertyList(this);
    ui->widgetLayout->addWidget(propertyListWidget);
    propertyListWidget->setFocus();
}

void Inventory_SideBar::PriceListClicked()
{
    resetAllColors();
    PriceList->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    priceListWidget = new priceList(this);
    ui->widgetLayout->addWidget(priceListWidget);
    priceListWidget->setFocus();
}

void Inventory_SideBar::UOMClicked()
{
    resetAllColors();
    UOMButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    uomWidget = new UOM(this);
    ui->widgetLayout->addWidget(uomWidget);
    uomWidget->setFocus();
}

void Inventory_SideBar::GodownClicked()
{
    resetAllColors();
    Godown->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    godownwidget = new godown(this);
    ui->widgetLayout->addWidget(godownwidget);
    godownwidget->setFocus();
}

void Inventory_SideBar::BrandsClicked()
{
    resetAllColors();
    brandsToolButt->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    BrandsListWidget = new BrandsList(this);
    QObject::connect(this,SIGNAL(searchTextchanged(QString)),BrandsListWidget ,SLOT(searchTextChanged(QString)));

    ui->widgetLayout->addWidget(BrandsListWidget);
    BrandsListWidget->setFocus();
}

void Inventory_SideBar::StockClicked()
{
    resetAllColors();
    StockButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    StockReportWidget = new RestaurantClosingStock(this);
    ui->widgetLayout->addWidget(StockReportWidget);
    StockReportWidget->setFocus();
}

void Inventory_SideBar::ProductionRateClicked()
{
    resetAllColors();
    ProductionRate->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    productionRateWidget = new ProductionRateReport(this);
    ui->widgetLayout->addWidget(productionRateWidget);
    productionRateWidget->setFocus();
}

void Inventory_SideBar::MovementAnalysisClicked()
{
    resetAllColors();
    MovementAnalysisButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    MovementAnalysisWidget = new MovementAnalysis(this);
    ui->widgetLayout->addWidget(MovementAnalysisWidget);
    MovementAnalysisWidget->setFocus();
}

void Inventory_SideBar::ItemAnalysisClicked()
{
    resetAllColors();
    ItemAnalysisButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    ItemAnalysisWidget = new ItemAnalysis(this);
    ui->widgetLayout->addWidget(ItemAnalysisWidget);
    ItemAnalysisWidget->setFocus();
}

void Inventory_SideBar::ItemwiseSalesClicked()
{
    resetAllColors();
    ItemWiseSales->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    itemwiseReport = new ItemwiseReport(this);
    ui->widgetLayout->addWidget(itemwiseReport);
    itemwiseReport->setFocus();
}

void Inventory_SideBar::itemRep1Clicked()
{
    resetAllColors();
    itemRepB1->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    itemRep1 = new ItemwiseSODatewiseReport(this);
    ui->widgetLayout->addWidget(itemRep1);
    itemRep1->setFocus();
}

void Inventory_SideBar::GroupwiseClicked()
{
    resetAllColors();
    qDebug()<<"groupwise clicked";
    GroupwiseSales ->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    GroupwiseItemsWidget = new GroupwiseReport(this);
    qDebug()<<"groupwise called";
    ui->widgetLayout->addWidget(GroupwiseItemsWidget);
    qDebug()<<"widget added";
    GroupwiseItemsWidget->setFocus();
}

void Inventory_SideBar::PurchaseReportClicked()
{
    resetAllColors();
    GroupwisePurchase ->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    PurchaseReportWidget = new PurchaseReport(this);
    ui->widgetLayout->addWidget(PurchaseReportWidget);
    PurchaseReportWidget->setFocus();
}

void Inventory_SideBar::WorkOrderReportClicked()
{
    resetAllColors();
    WOReportButton ->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    WorkOrderReportWidget = new WorkOrderReport(this);
    ui->widgetLayout->addWidget(WorkOrderReportWidget);
    WorkOrderReportWidget->setFocus();
}

void Inventory_SideBar::DebitNoteReportClicked()
{
    resetAllColors();
    DebitNoteReportButton ->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    DebitNoteReportWidget = new DebitNoteReport(this);
    ui->widgetLayout->addWidget(DebitNoteReportWidget);
    DebitNoteReportWidget->setFocus();
}

void Inventory_SideBar::CreditNoteReportClicked()
{
    resetAllColors();
    CreditNoteReportButton ->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    CreditNoteReportWidget = new CreditNoteReport(this);
    ui->widgetLayout->addWidget(CreditNoteReportWidget);
    CreditNoteReportWidget->setFocus();
}

void Inventory_SideBar::WastageReportClicked()
{
    resetAllColors();
    WastageReportButton ->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    WastageReportWidget = new WastageReport(this);
    ui->widgetLayout->addWidget(WastageReportWidget);
    WastageReportWidget->setFocus();
}

void Inventory_SideBar::LedgerWiseItemsClicked()
{
    resetAllColors();
    ledgerwiseItemsButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    ledgerwiseItemsWidget = new LedgerwiseItemMovement(this);
    ui->widgetLayout->addWidget(ledgerwiseItemsWidget);
    ledgerwiseItemsWidget->setFocus();
}

void Inventory_SideBar::DailyCostingClicked()
{
    resetAllColors();
    dailyCostingButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    DailyCostingReport *dailyCostingWidget = new DailyCostingReport(this);
    ui->widgetLayout->addWidget(dailyCostingWidget);
    dailyCostingWidget->setFocus();
}

void Inventory_SideBar::POSClicked()
{
    resetAllColors();
    POSButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    RestuarantCounterMainWindow* widget = new RestuarantCounterMainWindow();
    widget->showFullScreen();
    QWidget* widt = new QWidget;
    ui->widgetLayout->addWidget(widt);


}

void Inventory_SideBar::PhysicalStockRegistersClicked()
{
    resetAllColors();
    PhysicalStockRegistersButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    PhysicalStockRegisterWidget = new StockEntry(this);
    ui->widgetLayout->addWidget(PhysicalStockRegisterWidget);
    PhysicalStockRegisterWidget->setFocus();
}

void Inventory_SideBar::GodownWiseReportClicked()
{
    resetAllColors();
    GodownWiseReportButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    GodownwiseReportWidget = new GodownwiseReport(this);
    ui->widgetLayout->addWidget(GodownwiseReportWidget);
    GodownwiseReportWidget->setFocus();

    //        godownStockReport = new GodownwiseStockReport(this);
    //        ui->widgetLayout->addWidget(godownStockReport);
    //        godownStockReport->setFocus();
}

void Inventory_SideBar::SalesBillsPendingClicked()
{
    resetAllColors();
    SalesBillsPendingButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    SalesBillsPendingWidget = new SalesBillsPending(this);
    ui->widgetLayout->addWidget(SalesBillsPendingWidget);
    SalesBillsPendingWidget->setFocus();
}

void Inventory_SideBar::PurchaseBillsPendingClicked()
{
    resetAllColors();
    PurchaseBillsPendingButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    PurchaseBillsPendingWidget = new PurchaseBillsPending(this);
    ui->widgetLayout->addWidget(PurchaseBillsPendingWidget);
    PurchaseBillsPendingWidget->setFocus();
}

void Inventory_SideBar::NegativeStockClicked()
{
    resetAllColors();
    NegativeStockButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    NegativeStockWidget = new NegativeStock(this);
    ui->widgetLayout->addWidget(NegativeStockWidget);
    NegativeStockWidget->setFocus();
}

void Inventory_SideBar::ReorderLevelClicked()
{
    resetAllColors();
    ReorderLevelButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    BelowReorderLevelWidget = new ReOrderLevel(this);
    ui->widgetLayout->addWidget(BelowReorderLevelWidget);
    BelowReorderLevelWidget->setFocus();
}

void Inventory_SideBar::itemwiseSOClicked()
{
    resetAllColors();
    ItemeiseSOButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    itemwiseSOListWidget = new IteemwiseSalesOrderList(GMVoucherTypes::getVoucherString(GMVoucherTypes::itemwiseOrder),
                                                       GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesOrder) ,
                                                       new SalesOrderDatabaseHelper(),
                                                       ItemwiseSalesOrderDatabaseHelper::Sales_Order_Itemwise_TableName,
                                                       SalesOrderDatabaseHelper::Sales_Order_Main_TableName,
                                                       SalesOrderDatabaseHelper::Sales_Order_Details_TableName,
                                                       this);
    itemwiseSOListWidget->setTitle("Sales Orders");
    ui->widgetLayout->addWidget(itemwiseSOListWidget);
    itemwiseSOListWidget->setFocus();
}

void Inventory_SideBar::itemwisePOClicked()
{
    resetAllColors();
    ItemeisePOButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    itemwiseSOListWidget = new IteemwiseSalesOrderList(GMVoucherTypes::getVoucherString(GMVoucherTypes::itemwisePurchaseOrder),
                                                       GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseOrder),
                                                       new PurchaseOrderDatabaseHelper(),
                                                       ItemwiseSalesOrderDatabaseHelper::Purchase_Order_Itemwise_TableName,
                                                       PurchaseOrderDatabaseHelper::Purchase_Order_Main_TableName,
                                                       PurchaseOrderDatabaseHelper::Purchase_Order_Details_TableName,
                                                       this);


    itemwiseSOListWidget->setTitle("Purchase Orders");
    ui->widgetLayout->addWidget(itemwiseSOListWidget);
    itemwiseSOListWidget->setFocus();
}

void Inventory_SideBar::ModeOfServiceClicked()
{
    resetAllColors();
    modeButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    serviceSalesReport = new ModeOfServiceWiseReport(this);
    ui->widgetLayout->addWidget(serviceSalesReport);
    serviceSalesReport->setFocus();
}

void Inventory_SideBar::timeReportClicked()
{
    qDebug()<<"time report clicked";
    resetAllColors();
    TimeButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    timeSalesReport = new TimeBasedSales(this);
    ui->widgetLayout->addWidget(timeSalesReport);
    timeSalesReport->setFocus();
}

void Inventory_SideBar::GodownStockClicked()
{
    resetAllColors();
    //    godownwiseStockButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    godownStockReport = new GodownwiseStockReport(this);
    qDebug()<<"1G";
    ui->widgetLayout->addWidget(godownStockReport);
    godownStockReport->setFocus();
}

void Inventory_SideBar::GodownwiseClicked()
{
    resetAllColors();
    //    godownwiseStockButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    godownStockReport = new GodownwiseStockReport(this);
    qDebug()<<"1G";
    ui->widgetLayout->addWidget(godownStockReport);
    godownStockReport->setFocus();
}

void Inventory_SideBar::GodownMovementClicked()
{
    GodownwiseReport *rep = new GodownwiseReport(this);
    ui->widgetLayout->addWidget(rep);
}

void Inventory_SideBar::transactionReportClicked()
{
    resetAllColors();
    int type = sender()->property("Type").toInt();
    TransSummary->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    summaryWidget = new ItemTransactionSummaryReportGenerator(this);
    summaryWidget->setVoucherType(type);
    summaryWidget->setTableView();
    summaryWidget->setReportName(GMVoucherTypes::getVoucherString(type));
    ui->widgetLayout->addWidget(summaryWidget);
    summaryWidget->setFocus();
}

void Inventory_SideBar::OpeingClicked()
{
    resetAllColors();
    SalesInventoryOpeningWidget = new SalesInventoryOpening(this);
    ui->widgetLayout->addWidget(SalesInventoryOpeningWidget);
    SalesInventoryOpeningWidget->setFocus();
}

void Inventory_SideBar::clearLayout()
{


    if (ui->widgetLayout != NULL )
    {
        qDebug()<<"clearing Inventory Layout";

        QLayoutItem* item;
        while (( item = ui->widgetLayout->takeAt(0)) != NULL )
        {
            item->widget()->deleteLater();
            delete item;
        }

        qDebug()<<"Layout Cleared.";
    }

    //    while (QLayoutItem* item = layout->takeAt(0))
    //       {
    //           if (deleteWidgets)
    //           {
    //               if (QWidget* widget = item->widget())
    //                   widget->deleteLater();
    //           }
    //           if (QLayout* childLayout = item->layout())
    //               clearLayout(childLayout, deleteWidgets);
    //           delete item;
    //       }



}

void Inventory_SideBar::setUAC()
{
    ItemMaster->setVisible(LoginValues::model.Sales_Inventory_Items_View);
    ItemGroup -> setVisible(LoginValues::model.Sales_Inventory_Groups_View);
    StockEntryButton -> setVisible(LoginValues::model.Stock_Entries_View);
    StockJournal -> setVisible(LoginValues::model.Stock_Journals_View);
    MaterialIn -> setVisible(LoginValues::model.Material_In_Out_View);
    MaterialOut -> setVisible(LoginValues::model.Material_In_Out_View);
    DeliveryNote -> setVisible(LoginValues::model.Delivery_Notes_View);
    ReceiptNote -> setVisible(LoginValues::model.Receipt_Notes_View);
    Property -> setVisible(LoginValues::model.Properties_View);
    PriceList -> setVisible(LoginValues::model.Price_Lists_View);
    UOMButton -> setVisible(LoginValues::model.UOM_View);
    Godown -> setVisible(LoginValues::model.Godowns_View);
    StockButton -> setVisible(LoginValues::model.Stock_Entries_View);
    MovementAnalysisButton -> setVisible(LoginValues::model.Stock_Entries_View);
    ItemAnalysisButton -> setVisible(LoginValues::model.Stock_Entries_View);
    PhysicalStockRegistersButton-> setVisible(LoginValues::model.Stock_Entries_View);
    GodownWiseReportButton -> setVisible(LoginValues::model.Stock_Entries_View);
    SalesBillsPendingButton -> setVisible(LoginValues::model.Stock_Entries_View);
    PurchaseBillsPendingButton -> setVisible(LoginValues::model.Stock_Entries_View);
    NegativeStockButton -> setVisible(LoginValues::model.Stock_Entries_View);
    ReorderLevelButton -> setVisible(LoginValues::model.Stock_Entries_View);
    brandsToolButt -> setVisible(LoginValues::model.Brands_View);

}
