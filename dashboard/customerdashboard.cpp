#include "customerdashboard.h"
#include "ui_customerdashboard.h"
#include "QFormLayout"

CustomerDashboard::CustomerDashboard(LedgerHelper *ledgerHelper,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomerDashboard)
{
    ui->setupUi(this);

    dbHelper= ledgerHelper;
    this->setObjectName("dashboard");
    this->setStyleSheet("QWidget#dashboard{background-color:" + bgcolor + "}");

    QPalette pal = palette();
    pal.setColor(QPalette::Background,QColor(bgcolor));
    setAutoFillBackground(true);
    setPalette(pal);

    initDbHelpers();
    setWidgets();
    setAllCustomersLayout();
    setFilterLayout();
    setLedgerLayout();

}

CustomerDashboard::~CustomerDashboard()
{
    delete ui;
}

void CustomerDashboard::initDbHelpers()
{
    ledger = new LedgerMasterDataModel();
    ledgerHelper = new LedgerMasterDatabaseHelper;
    accHelper = new AccountGroupMasterDatabaseHelper;
}

void CustomerDashboard::setWidgets()
{
    QWidget *mainWidget = new QWidget;
    allCustLayout = new QHBoxLayout;
    QWidget *allCustWidget = new QWidget;
    allCustWidget->setLayout(allCustLayout);


    filterLayout = new QHBoxLayout;
    ledgerLayout = new QVBoxLayout;

    ledgerDetails = new QWidget;
    ledgerDetails->setLayout(ledgerLayout);
//    ledgerDetails->hide();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainWidget->setLayout(mainLayout);

    mainLayout->addWidget(allCustWidget);
    mainLayout->addLayout(filterLayout);
    mainLayout->addWidget(ledgerDetails);
    mainLayout->addStretch(1);

    ui->verticalLayout->addWidget(mainWidget);

    mainWidget->setObjectName("dashboard");
    mainWidget->setStyleSheet("QWidget#dashboard{background-color:" + bgcolor + "}");

}

void CustomerDashboard::setFilterLayout()
{
    QStringList salesGroups;
    salesGroups.append(accHelper->getGroupIDByName("Sundry Debtors"));
    for(AccountGroupDataModel acc: accHelper->getAccountsUnderParentGroupRecursive(accHelper->getGroupIDByName("Sundry Debtors"))){
        salesGroups.append(acc.groupID);
    }

    GMLedgerLineMaterial *searchWid = new GMLedgerLineMaterial(ledgerHelper,
                                                               ledgerHelper->getLedgerQueryString(salesGroups),
                                                               ledger);
    searchWid->setLabel("Customer Name");
    searchWid->setParent(this);
    searchWid->setStyleSheet("GMLedgerLineMaterial{background-color:#ffffff; color:" + textColor +"}");
    searchWid->setFixedWidth(200);

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
    filterLayout->addWidget(searchWid);
    filterLayout->addStretch(1);

    QObject::connect(searchWid,&GMLedgerLineMaterial::itemSelectedwithItem,
                this,[=](LedgerMasterDataModel led){
        setValues();
    });
}

void CustomerDashboard::setLedgerLayout()
{

    rankCard = new GMCardWidget("Rank","By Sales", "",false);
    totalLedSalesCard = new GMCardWidget("Total Sales");
    outstandingCard = new GMCardWidget("Outstanding Balance");
    outstandingDateCard = new GMCardWidget("Outstanding From");
    avgCreditPeriodCard = new GMCardWidget("Avg Credit Period", "days");

    avgsTable = new QTableView;
    topItems = new QTableView;
    last10 = new QTableView;

    avgsTable->setStyleSheet(tableViewSS);
    topItems->setStyleSheet(tableViewSS);
    last10->setStyleSheet(tableViewSS);

    avgsTable->verticalHeader()->hide();
    topItems->verticalHeader()->hide();
    last10->verticalHeader()->hide();

    QWidget *tableWid1 = new QWidget;
    QVBoxLayout *tableLay1 = new QVBoxLayout;
    tableWid1->setLayout(tableLay1);
    QLabel *table1Label = new QLabel("Sales Statistics");
    table1Label->setStyleSheet("QLabel{color: #006064}");
    tableLay1->addWidget(table1Label);
    tableLay1->addWidget(avgsTable);

    QWidget *tableWid2 = new QWidget;
    QVBoxLayout *tableLay2 = new QVBoxLayout;
    tableWid2->setLayout(tableLay2);
    QLabel *table2Label = new QLabel("Top Items Bought");
    table2Label->setStyleSheet("QLabel{color: #006064}");
    tableLay2->addWidget(table2Label);
    tableLay2->addWidget(topItems);

    QWidget *tableWid3 = new QWidget;
    QVBoxLayout *tableLay3 = new QVBoxLayout;
    tableWid3->setLayout(tableLay3);
    QLabel *table3Label = new QLabel("Last Transactions");
    table3Label->setStyleSheet("QLabel{color: #006064}");
    tableLay3->addWidget(table3Label);
    tableLay3->addWidget(last10);

    QHBoxLayout *row1Layout = new QHBoxLayout;
    row1Layout->addWidget(rankCard);
    row1Layout->addWidget(totalLedSalesCard);
    row1Layout->addWidget(outstandingCard);
    row1Layout->addWidget(outstandingDateCard);
    row1Layout->addWidget(avgCreditPeriodCard);


    QHBoxLayout *row2Layout = new QHBoxLayout;
    row2Layout->addWidget(tableWid1);
    row2Layout->addWidget(tableWid2);
    row2Layout->addWidget(tableWid3,1);

    ledgerLayout->addLayout(row1Layout);
    ledgerLayout->addLayout(row2Layout);
//    ledgerLayout->addStretch(1);



}

void CustomerDashboard::setAllCustomersLayout()
{
     QHBoxLayout *row1Layout = new QHBoxLayout;
     allCustLayout->addLayout(row1Layout);

     GMCardWidget *numActiveCustomer = new GMCardWidget("Customers Count",
                                                        "(Last 3 months)"
                                                        );
     numActiveCustomer->setDataWidget(new QLabel(QString::number(dbHelper->getCustomerCount
                                                                 (QDate::currentDate().addMonths(-3),
                                                                  QDate::currentDate()))));
     GMCardWidget *totalOuts = new GMCardWidget("Total Outstanding");
     totalOuts->setDataWidget(new QLabel(dbHelper->getTotalOutstandingBalance
                                         (QDate::currentDate().addMonths(-6),
                                          QDate::currentDate())));
     totalOuts->setFixedWidth(200);

     QTableView *topSales = new QTableView;
     topSales->setStyleSheet(tableViewSS);
     QTableView *topDebtors = new QTableView;
     topDebtors->setStyleSheet(tableViewSS);

     topSales->setModel(dbHelper->getTopCustomers(QDate::currentDate().addMonths(-3),
                                                  QDate::currentDate()));
     topSales->setItemDelegate(new ReportCommonDelegate());
     topSales->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
     topSales->verticalHeader()->hide();

     topDebtors->setModel( dbHelper->getTopDebtors(fromDate, toDate,5));
     topDebtors->setItemDelegate(new ReportCommonDelegate);
     topDebtors->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
     topDebtors->verticalHeader()->hide();

     row1Layout->addWidget(numActiveCustomer);
     row1Layout->addWidget(totalOuts);
     row1Layout->addWidget(topSales);
     row1Layout->addWidget(topDebtors);
//     row1Layout->addWidget(totalOverDu);


}

void CustomerDashboard::setLedgerValues()
{
    QLabel *phone = new  QLabel(ledger->phoneNumber);
    QLabel *mob = new  QLabel(ledger->mobileNumber);
    QLabel *grpName = new  QLabel(ledger->LedgerGroupName);
    QLabel *cperiod = new  QLabel(QString::number(ledger->Credit_Period));
    QLabel *climit = new  QLabel(QString::number(ledger->Credit_Limit,'f',2));
//    QLabel *grpName = new  QLabel(ledger->listid);
    QLabel *currBalance = new  QLabel(QString::number(ledger->closingBalance,'f',2));

    QFormLayout *form = new QFormLayout;

    form->addRow(new QLabel("Group"),grpName);
    form->addRow(new QLabel("Mobile"),mob);
    form->addRow(new QLabel("Phone"),phone);
    form->addRow(new QLabel("Credit Limit"),climit);
    form->addRow(new QLabel("Credit Period"),cperiod);
    form->addRow(new QLabel("Current Balance"),currBalance);

}

void CustomerDashboard::setValues()
{
    avgsTable->setModel(dbHelper->getLedgerTransValueByFrequency
                        (GMVoucherTypes::SalesVoucher, fromDate, toDate, ledger->LedgerID));
    avgsTable->setItemDelegate(new ReportCommonDelegate());

    topItems->setModel( dbHelper->getTopItemsByCustomer(ledger->LedgerID, fromDate, toDate,5));
    topItems->setItemDelegate(new ReportCommonDelegate);

    last10->setModel(dbHelper->getLedgerReportModel(ledger->LedgerID, fromDate, toDate, 5));
    last10->setItemDelegate(new ReportCommonDelegate);

    avgsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    topItems->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    last10->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QSqlQueryModel *rankModel = dbHelper->getRankBySalesValue(ledger->LedgerID, fromDate, toDate);
    rankCard->setDataWidget(new QLabel(rankModel->record(0).value(0).toString()));
    totalLedSalesCard->setDataWidget(new QLabel(rankModel->record(0).value(1).toString()));

    QSqlQueryModel *creditModel = dbHelper->getCreditInfoByLedger(ledger->LedgerID, fromDate, toDate)    ;
    outstandingCard->setDataWidget(new QLabel(creditModel->record(0).value(0).toString()));
    outstandingDateCard->setDataWidget(new QLabel(creditModel->record(0).value(1).toDate().toString("dd-MM-yyyy")));

    QSqlQueryModel *averageCreditModel = dbHelper->getAverageCreditPeriodByCustomer(ledger->LedgerID, fromDate, toDate);
    avgCreditPeriodCard->setDataWidget(new QLabel(averageCreditModel->record(0).value(0).toString()));
}


//1. Avg. Monthly/Weekly/Daily/Annual  Sales/Receipts/Returns -- getLedgerTransValueByFrequency -- table
//2. Avg Credit Period -- getAverageCreditPeriodByCustomer
//3. Most Sold 10 items by Value -- getTopItemsByCustomer -- table
//4. Earliest Outstanding date -- getCreditInfoByLedger
//5. Outstanding Bill Amounts and dates -- getOutstandingReportByLedger -- table
//5. Outstanding Total :
//6. Credit Period
//7. Credit Limit
//8. Send Ledger Report By Mail
//9. Automated Ledger Mailing
//10. Margins by date
//11. Rank By Sales Value
//12. Active Customers Count

// item --> most sold, least sold
