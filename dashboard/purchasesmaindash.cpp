#include "purchasesmaindash.h"
#include "ui_purchasesmaindash.h"

#include <QHeaderView>
#include <QScrollArea>
#include <QScrollBar>
PurchasesMainDash::PurchasesMainDash(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PurchasesMainDash)
{
    ui->setupUi(this);

    this->setObjectName("dashboard");
    this->setStyleSheet("QWidget#dashboard{background-color:" + bgcolor + "}");

    QPalette pal = palette();
    pal.setColor(QPalette::Background,QColor(bgcolor));
    setAutoFillBackground(true);
    setPalette(pal);

    ledHelper = new LedgerHelper;
    invHelper = new InventoryHelper;
    setWidgets();
    setFilterLayout();
}

PurchasesMainDash::~PurchasesMainDash()
{
    delete ui;
}

void PurchasesMainDash::setWidgets()
{
    QWidget *mainWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainWidget->setLayout(mainLayout);
    mainWidget->setObjectName("dashboard");
    mainWidget->setStyleSheet("QWidget#dashboard{background-color:" + bgcolor + "}");
//    mainWidget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);


    ui->verticalLayout->addWidget(mainWidget);

    filterLayout = new QHBoxLayout;
    QHBoxLayout *row1Layout = new QHBoxLayout;
    QHBoxLayout *row2Layout = new QHBoxLayout;
    QHBoxLayout *row3Layout = new QHBoxLayout;
    QHBoxLayout *row4Layout = new QHBoxLayout;

    mainLayout->addLayout(filterLayout);
    mainLayout->addLayout(row1Layout);
    mainLayout->addLayout(row2Layout);
    mainLayout->addLayout(row3Layout);
    mainLayout->addLayout(row4Layout);


    pendingPurchaseCard = new GMCardWidget("Pending Purchase Orders");
    deliveryExpectedCard = new GMCardWidget("Expected Delivery");
    negativeStockCard = new GMCardWidget("Negative Stock List");
    paymentPendingCard = new GMCardWidget("Pending Payment");
    topVendorsCard = new GMCardWidget("Top Vendors");
    purchaseStatsCard = new GMCardWidget("Purchase Statistics");
    totalPurchaseValueCard = new GMCardWidget("Total Purchase Value");
    lastPurchasesCard = new GMCardWidget("Last Purchases");
    mostBoughtItems = new GMCardWidget("Most Purchased Items");

    row1Layout->addWidget(totalPurchaseValueCard);
    row1Layout->addWidget(lastPurchasesCard);
    row1Layout->addWidget(pendingPurchaseCard);
    row1Layout->addWidget(deliveryExpectedCard);
    row2Layout->addWidget(topVendorsCard);
    row2Layout->addWidget(negativeStockCard);
    row2Layout->addWidget(mostBoughtItems);
    row2Layout->addWidget(paymentPendingCard);

    totalPurchaseValueTable = new QLabel;
    pendingPurchaseTable = new QTableView;
    deliveryExpectedTable = new QTableView;
    negativeStockTable = new QTableView;
    paymentPendingTable = new QTableView;
    topVendorsTable = new QTableView;
    purchaseStatsTable = new QTableView;
    lastPurchasesTable = new QTableView;
    mostBoughtItemsTable = new QTableView;

    totalPurchaseValueTable = new QLabel;
    pendingPurchaseTable->setStyleSheet(tableViewSS);
    deliveryExpectedTable->setStyleSheet(tableViewSS);
    negativeStockTable->setStyleSheet(tableViewSS);
    paymentPendingTable->setStyleSheet(tableViewSS);
    topVendorsTable->setStyleSheet(tableViewSS);
    purchaseStatsTable->setStyleSheet(tableViewSS);
    lastPurchasesTable->setStyleSheet(tableViewSS);
    mostBoughtItemsTable->setStyleSheet(tableViewSS);

    pendingPurchaseTable->verticalHeader()->hide();
    deliveryExpectedTable->setStyleSheet(tableViewSS);
    negativeStockTable->setStyleSheet(tableViewSS);
    paymentPendingTable->setStyleSheet(tableViewSS);
    topVendorsTable->setStyleSheet(tableViewSS);
    purchaseStatsTable->setStyleSheet(tableViewSS);
    lastPurchasesTable->setStyleSheet(tableViewSS);
    mostBoughtItemsTable->setStyleSheet(tableViewSS);

    pendingPurchaseCard->setDataWidget(pendingPurchaseTable);
    deliveryExpectedCard->setDataWidget(deliveryExpectedTable);
    negativeStockCard->setDataWidget(negativeStockTable);
    paymentPendingCard->setDataWidget(paymentPendingTable);
    topVendorsCard->setDataWidget(topVendorsTable);
    purchaseStatsCard->setDataWidget(purchaseStatsTable);
    totalPurchaseValueCard->setDataWidget(totalPurchaseValueTable);
    lastPurchasesCard->setDataWidget(lastPurchasesTable);
    mostBoughtItems->setDataWidget(mostBoughtItemsTable);



//    QScrollArea *scroll = new QScrollArea(this);
//    scroll->setObjectName("POSMain");
//    scroll->setStyleSheet("QWidget#POSMain{background-color:#e0f7fa}");
//    scroll->verticalScrollBar()->
//            setStyleSheet(QString::fromUtf8("QScrollBar:vertical {"
//                                            "    border: 1px solid #999999;"
//                                            "    background:white;"
//                                            "    width:20px;    "
//                                            "    margin: 0px 0px 0px 0px;"
//                                            "}"
//                                            "QScrollBar::handle:vertical {"
//                                            "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
//                                            "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
//                                            "    min-height: 0px;"
//                                            ""
//                                            "}"
//                                            "QScrollBar::add-line:vertical {"
//                                            "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
//                                            "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
//                                            "    height: 0px;"
//                                            "    subcontrol-position: bottom;"
//                                            "    subcontrol-origin: margin;"
//                                            "}"
//                                            "QScrollBar::sub-line:vertical {"
//                                            "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
//                                            "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
//                                            "    height: 0px;"
//                                            "    subcontrol-position: top;"
//                                            "    subcontrol-origin: margin;"
//                                            "}"
//                                            ""));
//    scroll->setWidget(mainWidget);
//    scroll->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
//    scroll->setAlignment(Qt::AlignCenter);
//    QVBoxLayout *vLayout = new QVBoxLayout;
//    vLayout->addWidget(scroll);



}

void PurchasesMainDash::setFilterLayout()
{
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
    filterLayout->addStretch(1);

}

void PurchasesMainDash::setValues()
{
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    negativeStockTable->setModel(invHelper->getNegativeStockList(toDateTime, 5));
    topVendorsTable->setModel(ledHelper->getTopVendors(fromDate, toDate));
    purchaseStatsTable->setModel(ledHelper->getLedgerTransValueByFrequency(GMVoucherTypes::PurchaseVoucher,
                                                                           fromDate, toDate));
    mostBoughtItemsTable->setModel(invHelper->getMostBoughtItems(fromDate, toDate));
    lastPurchasesTable->setModel(ledHelper->getLedgerwiseTransactionSummaryByVoucherType(fromDateTime, toDateTime,
                                                                                         GMVoucherTypes::PurchaseVoucher,5));

}


//1. Pending Purch. Orders
//2. Delivery Expected
//3. Avg. Purchase Value <-Monthly/Weekly/Annual
//4. Payments Pending
//5. Top Vendors
//6. Reorder Level
//7. P.O Templates
//8. Negative Stock
