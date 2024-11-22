#include "inventoryitemdashboard.h"
#include "ui_inventoryitemdashboard.h"

InventoryItemDashboard::InventoryItemDashboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InventoryItemDashboard)
{
    ui->setupUi(this);


    this->setObjectName("dashboard");
    this->setStyleSheet("QWidget#dashboard{background-color:" + bgcolor + "}");


    QPalette pal = palette();
    pal.setColor(QPalette::Background,QColor(bgcolor));
    setAutoFillBackground(true);
    setPalette(pal);

    initDbHelpers();
    setWidgets();
    setAllItemsLayout();
    setFilterLayout();
    setItemLayout();
}

InventoryItemDashboard::~InventoryItemDashboard()
{
    delete ui;
}

void InventoryItemDashboard::initDbHelpers()
{
    dbHelper = new InventoryHelper;
    itemHelper = new SalesInventoryItemDatabaseHelper;
    grpHelper = new SalesInventoryGroupsDatabaseHelper;
    stkHelper = new ClosingStockHelper;
}

void InventoryItemDashboard::setWidgets()
{
    QWidget *mainWidget = new QWidget;
    allItemsLayout = new QVBoxLayout;
    QWidget *allItemsWidget = new QWidget;
    allItemsWidget->setLayout(allItemsLayout);


    filterLayout = new QHBoxLayout;
    itemLayout = new QVBoxLayout;
    QWidget *ItemWidget = new QWidget;
    ItemWidget->setLayout(itemLayout);

    QtMaterialScrollBar *itemScroll = new QtMaterialScrollBar;
    itemScroll->setParent(ItemWidget);
    itemScroll->setOrientation(Qt::Vertical);

    QHBoxLayout *lay = new QHBoxLayout;
    QScrollArea *scroll = new QScrollArea();
    scroll->setLayout(lay);
    lay->addWidget(ItemWidget);
    lay->addWidget(itemScroll);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainWidget->setLayout(mainLayout);

    mainLayout->addWidget(allItemsWidget);
    mainLayout->addLayout(filterLayout);
//    mainLayout->addLayout(lay);
    mainLayout->addWidget(ItemWidget);
    mainLayout->addStretch(1);

    ui->verticalLayout->addWidget(mainWidget);

    mainWidget->setObjectName("dashboard");
    mainWidget->setStyleSheet("QWidget#dashboard{background-color:" + bgcolor + "}");
}

void InventoryItemDashboard::setAllItemsLayout()
{
    QHBoxLayout *row1Layout = new QHBoxLayout;
    allItemsLayout->addLayout(row1Layout);
    allItemsLayout->addStretch(1);

    QTableView *mostSoldTable= new QTableView;
    mostSoldTable->setModel(dbHelper->getMostSoldItems(fromDate, toDate));
    mostSoldTable->setStyleSheet(tableViewSS);
    mostSoldTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mostSoldTable->verticalHeader()->hide();
    mostSoldTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    GMCardWidget *mostSoldCard = new GMCardWidget("Most Sold");
    mostSoldCard->setDataWidget(mostSoldTable);
    //    mostSoldCard->setMaximumSize(250,200);


    QTableView *mostBoughtTable = new QTableView;
    mostBoughtTable->setModel(dbHelper->getMostBoughtItems(fromDate, toDate));
    mostBoughtTable->setStyleSheet(tableViewSS);
    mostBoughtTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mostBoughtTable->verticalHeader()->hide();
    mostBoughtTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    GMCardWidget *mostBoughtCard = new GMCardWidget("Most Purchased");
    mostBoughtCard->setDataWidget(mostBoughtTable);
    //    mostBoughtCard->setMaximumSize(250,200);

    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    QTableView *mostWastedTable= new QTableView;
    mostWastedTable->setModel(dbHelper->getWastageQueryByDateVDATE(fromDateTime, toDateTime, 5));
    mostWastedTable->setStyleSheet(tableViewSS);
    mostWastedTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mostWastedTable->verticalHeader()->hide();
    mostWastedTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    GMCardWidget *mostWastageCard = new GMCardWidget("Most Defective");
    mostWastageCard->setDataWidget(mostWastedTable);
    //    mostWastageCard->setMaximumSize(250,200);

    row1Layout->addWidget(mostSoldCard);
    row1Layout->addWidget(mostBoughtCard);
    row1Layout->addWidget(mostWastageCard);

}

void InventoryItemDashboard::setFilterLayout()
{
    GMItemSearchLite *itemSearch = new GMItemSearchLite(itemHelper->getInventoryItemsQueryModel());
    itemSearch->setText("");
    itemSearch->setPlaceholderText("Search Item...");
    QWidget *SearchWid = new QWidget(this);
    QVBoxLayout *lay = new QVBoxLayout(SearchWid);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    lay->addWidget(itemSearch);
    SearchWid->setLayout(lay);
    SearchWid->setStyleSheet("GMItemSearchLite {color : " + textColor + "}");
    SearchWid->setWhatsThis("Type Inventory Item");
    SearchWid->setFixedWidth(200);

    QString dateEditSS = "GMDateEdit{background-color:" + bgcolor + ";border: none; color: " + textColor + "} "
                         "QDateEdit::drop-down {border-style: solid; subcontrol-origin:margin; spacing: 5px; width:15px;height:10px; subcontrol-position: right center;}"
                         "QDateEdit::down-arrow {image: url(:/icons/icons/hardware/svg/production/ic_keyboard_arrow_down_24px.svg);height:15px;width:15px; }";

    GMDateEdit *fromDateEdit = new GMDateEdit(&fromDate);
    fromDateEdit->setFixedWidth(80);
    fromDateEdit->setFrame(false);
    fromDateEdit->setStyleSheet(dateEditSS);
    fromDateEdit->setFont(QFont("Calibri", 9, QFont::Normal));

    GMDateEdit *toDateEdit = new GMDateEdit(&toDate);
    toDateEdit->setFixedWidth(80);
    toDateEdit->setFrame(false);
    toDateEdit->setStyleSheet(dateEditSS);
    toDateEdit->setFont(QFont("Calibri", 9, QFont::Normal));

    QLabel *fromDateLabel = new QLabel("From");
    fromDateLabel->setStyleSheet("QLabel{color:" + textColor + "}");

    QVBoxLayout *fromDateLay = new QVBoxLayout;
    fromDateLay->addWidget(fromDateLabel);
    fromDateLay->addWidget(fromDateEdit);

    QWidget *fromDateWidget = new QWidget;
    fromDateWidget->setLayout(fromDateLay);

    QLabel *toDateLabel = new QLabel("To");
    toDateLabel->setStyleSheet("QLabel{color:" + textColor + "}");

    QVBoxLayout *toDateLay = new QVBoxLayout;
    toDateLay->addWidget(toDateLabel);
    toDateLay->addWidget(toDateEdit);

    QWidget *toDateWidget = new QWidget;
    toDateWidget->setLayout(toDateLay);

    QtMaterialIconButton *okButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "refresh"));
    okButton->setParent(this);
    okButton->setColor(QColor(iconColor));
    okButton->setText("Refresh");
    QObject::connect(okButton,&QtMaterialIconButton::clicked,[=]{
                setValues();

    });

    filterLayout->addWidget(fromDateWidget);
    filterLayout->addWidget(toDateWidget);
    filterLayout->addWidget(okButton);
    filterLayout->addWidget(SearchWid);
    filterLayout->addStretch(1);

    QObject::connect(itemSearch,&GMItemSearchLite::SelectedItemID,
                     this,[=](inventoryItemDataModel itemObj){
        item = itemObj;
                setValues();
    });

}

void InventoryItemDashboard::setItemLayout()
{
    stockCard         = new GMCardWidget("Current Stock");
    lppCard           = new GMCardWidget("Last Purchase Price");
    lspCard           = new GMCardWidget("Last Sold Price");
    godownStockCard   = new GMCardWidget("Godownwise Stock");

    salesMarginCard  = new GMCardWidget("Sales Margin");
    timeBasedSalesCard   = new GMCardWidget("Time Based Sales");
    topCustomersCard = new GMCardWidget("Top Customers");
    rankCard = new GMCardWidget("Rank", "By Sales");
    avgShelfPeriodCard = new GMCardWidget("Average Shelf Period");
    avgPurchIntervalCard = new GMCardWidget("Average Purchase Interval");
    reorderLevelCard = new GMCardWidget("Re-Order Level");
    salesStatisticsCard = new GMCardWidget("Sales Statistics");
    purchaseStatisticsCard = new GMCardWidget("Purchase Statistics");
    salesRetStatisticsCard = new GMCardWidget("Sales Statistics");
    purchaseRetStatisticsCard = new GMCardWidget("Purchase Statistics");
    lastSalesCard = new GMCardWidget("Last Sales");
    lastPurchaseCard = new GMCardWidget("Last Purchase");

    QHBoxLayout *row1Layout = new QHBoxLayout;
    QHBoxLayout *row2Layout = new QHBoxLayout;
    QHBoxLayout *row3Layout = new QHBoxLayout;
    QHBoxLayout *row4Layout = new QHBoxLayout;
    QHBoxLayout *row5Layout = new QHBoxLayout;
    QHBoxLayout *row6Layout = new QHBoxLayout;

    row1Layout->addWidget(rankCard);
    row1Layout->addWidget(stockCard);
    row1Layout->addWidget(lppCard);
    row1Layout->addWidget(lspCard);
    row2Layout->addWidget(avgShelfPeriodCard);
    row2Layout->addWidget(avgPurchIntervalCard);
    row2Layout->addWidget(reorderLevelCard);
    row2Layout->addWidget(salesMarginCard);
    row3Layout->addWidget(lastSalesCard);
    row3Layout->addWidget(lastPurchaseCard);
    row3Layout->addWidget(timeBasedSalesCard);
    row3Layout->addWidget(godownStockCard);
    row3Layout->addWidget(topCustomersCard);
    row4Layout->addWidget(salesStatisticsCard);
    row4Layout->addWidget(purchaseStatisticsCard);
    row4Layout->addWidget(salesRetStatisticsCard);
    row4Layout->addWidget(purchaseRetStatisticsCard);

    stockLabel = new QLabel("");
    lppLabel = new QLabel("");
    lspLabel = new QLabel("");
    salesMarginLabel = new QLabel("");
    rankLabel = new QLabel("");
    avgShelfPeriodLabel = new QLabel("");
    avgPurchIntervalLabel = new QLabel("");
    reorderLevelLabel = new QLabel("");

    timeBasedTable = new QTableView;
    godownWiseStockTable = new QTableView;
    salesStatsTable = new QTableView;
    purchaseStatsTable = new QTableView;
    salesRetStatTable = new QTableView;
    purchaseRetStatTable = new QTableView;
    topCustomersTable = new QTableView;
    lastSalesTable = new QTableView;
    lastPurchaseTable = new QTableView;

    timeBasedTable->setStyleSheet(tableViewSS);
    godownWiseStockTable->setStyleSheet(tableViewSS);
    salesStatsTable->setStyleSheet(tableViewSS);
    purchaseStatsTable->setStyleSheet(tableViewSS);
    salesRetStatTable->setStyleSheet(tableViewSS);
    purchaseRetStatTable->setStyleSheet(tableViewSS);
    topCustomersTable->setStyleSheet(tableViewSS);
    lastSalesTable->setStyleSheet(tableViewSS);
    lastPurchaseTable->setStyleSheet(tableViewSS);

    rankCard->setDataWidget(rankLabel);
    stockCard->setDataWidget(stockLabel);
    lppCard->setDataWidget(lppLabel);
    lspCard->setDataWidget(lspLabel);

    godownStockCard->setDataWidget(godownWiseStockTable);
    salesMarginCard->setDataWidget(salesMarginLabel);
    timeBasedSalesCard->setDataWidget(timeBasedTable);
    topCustomersCard->setDataWidget(topCustomersTable);
    avgShelfPeriodCard->setDataWidget(avgShelfPeriodLabel);
    avgPurchIntervalCard->setDataWidget(avgPurchIntervalLabel);
    reorderLevelCard->setDataWidget(reorderLevelLabel);
    salesStatisticsCard->setDataWidget(salesStatsTable);
    purchaseStatisticsCard->setDataWidget(purchaseStatsTable);
    salesRetStatisticsCard->setDataWidget(salesRetStatTable);
    purchaseRetStatisticsCard->setDataWidget(purchaseRetStatTable);
    lastSalesCard->setDataWidget(lastSalesTable);
    lastPurchaseCard->setDataWidget(lastPurchaseTable);


    itemLayout->addLayout(row1Layout);
    itemLayout->addLayout(row2Layout);
    itemLayout->addLayout(row3Layout);
    itemLayout->addLayout(row4Layout);
    itemLayout->addLayout(row5Layout);
    itemLayout->addLayout(row6Layout);

    qDebug()<<itemLayout->children();

}

void InventoryItemDashboard::setValues()
{
    QSqlQueryModel *rankModel = dbHelper->getRankBySalesValue(item.ItemID, fromDate, toDate);
    rankLabel->setText(rankModel->record(0).value(0).toString());

    stockLabel->setText(QString::number(itemHelper->getClsStkOfItemTillDate(item.ItemID, toDate), 'f',3));
    lppLabel->setText(QString::number(itemHelper->getLastPurchPrice(item.ItemID, toDate)));
    lspLabel->setText(QString::number(itemHelper->getLastSalesPrice(item.ItemID, toDate)));

    godownWiseStockTable->setModel(stkHelper->getGodownwiseStockOfItem(item.ItemID, toDate));
    lastSalesTable->setModel(dbHelper->getItemCrDrBetDate(item.ItemID, fromDate, toDate,
                                                          GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher)));
    lastPurchaseTable->setModel(dbHelper->getItemCrDrBetDate(item.ItemID, fromDate, toDate,
                                                             GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher)));
    topCustomersTable->setModel(dbHelper->getTopCustomerByItem(item.ItemID, fromDate, toDate));

    salesStatsTable->setModel(dbHelper->getItemTransValueByFrequency(GMVoucherTypes::SalesVoucher, fromDate, toDate, item.ItemID));
    purchaseStatsTable->setModel(dbHelper->getItemTransValueByFrequency(GMVoucherTypes::PurchaseVoucher, fromDate, toDate, item.ItemID));
    salesRetStatTable->setModel(dbHelper->getItemTransValueByFrequency(GMVoucherTypes::CreditNote, fromDate, toDate, item.ItemID));
    purchaseRetStatTable->setModel(dbHelper->getItemTransValueByFrequency(GMVoucherTypes::DebitNote, fromDate, toDate, item.ItemID));

}


// 1. Last 5 Sales/Purchase with sell p/cost p --getLastTransactionsByItem
// 2. Sales Margin
// 3. Most bought by Customers -- getTopCustomerByItem
// 4. Price Variation --
// 5. Stock Godownwise
// 6. Stock Adj Entries
// 7. By SalesMan
// 8. By Godown sales
// 9. Rank By Sales Value
//10. Purchase Interval
//11. Stock Out Probability
//12. Average shelf period
//13. Average Sales.Ret/Purch.Ret <- Weekly/Monthly/Annual
