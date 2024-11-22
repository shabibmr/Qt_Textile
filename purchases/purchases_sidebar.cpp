#include "purchases_sidebar.h"
#include "ui_purchases_sidebar.h"


Purchases_SideBar::Purchases_SideBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Purchases_SideBar)
{
    ui->setupUi(this);
    dash = new DefaultDashBoard(this);
    ui->scrollArea->setStyleSheet("background-color:qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:0.4, stop:0 #424242, stop:1 #424242)");

    ui->verticalLayout_3->setAlignment(Qt::AlignTop);

    qDebug()<<"Purchase Side Loading";

    sectionReports = new Section("Reports", 300,new QIcon(":/icons/gm.ico"), this);
    
    
    PurchaseOrders = new QToolButton(this);
    Vendors = new QToolButton(this);
    PurchaseVouchers = new QToolButton(this);
    Payments = new QToolButton(this);
    CreditNotes = new QToolButton(this);

    PurchaseOrdersPendingButton = new QToolButton(this);
    ReorderStatusButton = new QToolButton(this);
    LedgerReportButton = new QToolButton(this);
    PurchaseRegisterButton = new QToolButton(this);
    PurchaseOrderBookButton = new QToolButton(this);
    StockReportButton = new QToolButton(this);
    PaymentOutstandingButton = new QToolButton(this);
    ReservedItemsButton = new QToolButton(this);
    CostEstimatesButton = new QToolButton(this);
    PurchaseOrders->setText("Purchase Orders");
    Vendors->setText("Vendors");
    PurchaseVouchers->setText("Purchases");
    Payments->setText("Payments");
    CreditNotes->setText("Credit Notes");
    PurchaseOrdersPendingButton->setText("Purchase Orders Pending");
    ReorderStatusButton->setText("Reorder Status");
    LedgerReportButton->setText("Ledger Report");
    PurchaseRegisterButton->setText("Purchase Register");
    PurchaseOrderBookButton->setText("Purchase Order Book");
    StockReportButton->setText("Stock Report");
    PaymentOutstandingButton->setText("Payment Outstanding");
    ReservedItemsButton->setText("Items Reserved");
    CostEstimatesButton->setText("Cost Estimates");

    QFont font;
    font.setPointSize(10);

    PurchaseOrders->setFont(font);
    Vendors->setFont(font);
    PurchaseVouchers->setFont(font);
    Payments->setFont(font);
    CreditNotes->setFont(font);

    PurchaseOrdersPendingButton->setFont(font);
    ReorderStatusButton->setFont(font);
    LedgerReportButton->setFont(font);
    PurchaseRegisterButton->setFont(font);
    PurchaseOrderBookButton->setFont(font);
    StockReportButton->setFont(font);
    PaymentOutstandingButton->setFont(font);
    ReservedItemsButton->setFont(font);
    CostEstimatesButton->setFont(font);
    PurchaseOrders->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    Vendors->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    PurchaseVouchers->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    Payments->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    CreditNotes->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    PurchaseOrdersPendingButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ReorderStatusButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    LedgerReportButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    PurchaseRegisterButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    PurchaseOrderBookButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    StockReportButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    PaymentOutstandingButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ReservedItemsButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    CostEstimatesButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->verticalLayout_3->addWidget(PurchaseOrders);
    ui->verticalLayout_3->addWidget(Vendors);
    ui->verticalLayout_3->addWidget(PurchaseVouchers);
    ui->verticalLayout_3->addWidget(Payments);
    ui->verticalLayout_3->addWidget(CreditNotes);

    ui->verticalLayout_3->addWidget(sectionReports);

    auto* reportsLayout  = new QVBoxLayout();
    reportsLayout->addWidget(PurchaseOrdersPendingButton);
    reportsLayout->addWidget(ReorderStatusButton);
    reportsLayout->addWidget(LedgerReportButton);
    reportsLayout->addWidget(PurchaseRegisterButton);
    reportsLayout->addWidget(PurchaseOrderBookButton);
    reportsLayout->addWidget(StockReportButton);
    reportsLayout->addWidget(PaymentOutstandingButton);
    reportsLayout->addWidget(ReservedItemsButton);
    reportsLayout->addWidget(CostEstimatesButton);


    sectionReports->setContentLayout(*reportsLayout);

    QObject::connect(PurchaseOrders,SIGNAL(clicked(bool)),this,SLOT(PurchaseOrdersClicked()));
    QObject::connect(Vendors,SIGNAL(clicked(bool)),this,SLOT(VendorsClicked()));
    QObject::connect(PurchaseVouchers,SIGNAL(clicked(bool)),this,SLOT(PurchaseVouchersClicked()));
    QObject::connect(Payments,SIGNAL(clicked(bool)),this,SLOT(PaymentsClicked()));
    QObject::connect(CreditNotes,SIGNAL(clicked(bool)),this,SLOT(CreditNotesClicked()));

    QObject::connect(PurchaseOrdersPendingButton,SIGNAL(clicked(bool)),this,SLOT(PurchaseOrdersPendingClicked()));
    QObject::connect(ReorderStatusButton,SIGNAL(clicked(bool)),this,SLOT(ReorderStatusClicked()));
    QObject::connect(LedgerReportButton,SIGNAL(clicked(bool)),this,SLOT(LedgerReportClicked()));
    QObject::connect(PurchaseRegisterButton,SIGNAL(clicked(bool)),this,SLOT(PurchaseRegisterClicked()));
    QObject::connect(PurchaseOrderBookButton,SIGNAL(clicked(bool)),this,SLOT(PurchaseOrderBookClicked()));
    QObject::connect(StockReportButton,SIGNAL(clicked(bool)),this,SLOT(StockReportClicked()));
    QObject::connect(PaymentOutstandingButton,SIGNAL(clicked(bool)),this,SLOT(PaymentsOutstandingClicked()));
    QObject::connect(ReservedItemsButton,SIGNAL(clicked(bool)),this,SLOT(ReservedItemsClicked()));
    QObject::connect(CostEstimatesButton,SIGNAL(clicked(bool)),this,SLOT(CostEstimatesClicked()));

    resetColors();
    ui->widgetLayout->addWidget(dash);
    qDebug()<<"Purchase Side bar created";

}

Purchases_SideBar::~Purchases_SideBar()
{
    delete ui;
}

void Purchases_SideBar::clearLayout()
{
    if (ui->widgetLayout != NULL )
    {
        qDebug()<<"clearing Inventory Layout";

        QLayoutItem* item;
        while (( item = ui->widgetLayout->takeAt(0)) != NULL )
        {
            delete item->widget();
            delete item;
        }
    }
}

void Purchases_SideBar::resetColors()
{

    PurchaseOrders->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    Vendors->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    PurchaseVouchers->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    Payments->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    CreditNotes->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");

    PurchaseOrdersPendingButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    ReorderStatusButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    LedgerReportButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    PurchaseRegisterButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    PurchaseOrderBookButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    StockReportButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    PaymentOutstandingButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    ReservedItemsButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    CostEstimatesButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    clearLayout();
}

void Purchases_SideBar::PurchaseOrdersClicked()
{
    resetColors();
    PurchaseOrders->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    purchaseOrdersWidget = new purchaseOrders(this);
    ui->widgetLayout->addWidget(purchaseOrdersWidget);

}

void Purchases_SideBar::VendorsClicked()
{
    resetColors();
    Vendors->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    ledgerMasterWidget = new ledgerMaster(this);
    ui->widgetLayout->addWidget(ledgerMasterWidget);
}

void Purchases_SideBar::PurchaseVouchersClicked()
{
    resetColors();
    PurchaseVouchers->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    PurchaseEntryWidget = new PurchaseEntry();
    ui->widgetLayout->addWidget(PurchaseEntryWidget);
}

void Purchases_SideBar::PaymentsClicked()
{
    resetColors();
    Payments->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    PaymentWidget = new Payment(this);
    ui->widgetLayout->addWidget(PaymentWidget);
}

void Purchases_SideBar::CreditNotesClicked()
{
    resetColors();
    CreditNotes->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    creditNoteWidget = new creditNote(this);
    ui->widgetLayout->addWidget(creditNoteWidget);
}

void Purchases_SideBar::PurchaseOrdersPendingClicked()
{
    resetColors();
    PurchaseOrdersPendingButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    PurchaseOrdersPendingWidget = new PurchaseOrdersPending();
    ui->widgetLayout->addWidget(PurchaseOrdersPendingWidget);
    PurchaseOrdersPendingWidget->setFocus();
}

void Purchases_SideBar::ReorderStatusClicked()
{
    resetColors();
    ReorderStatusButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    ReorderStatusWidget = new ReOrderLevel();
    ui->widgetLayout->addWidget(ReorderStatusWidget);
    ReorderStatusWidget->setFocus();
}

void Purchases_SideBar::LedgerReportClicked()
{
    resetColors();
    LedgerReportButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    LedgerReportWidget = new LedgerReport();
    ui->widgetLayout->addWidget(LedgerReportWidget);
    LedgerReportWidget->setFocus();
}

void Purchases_SideBar::PurchaseRegisterClicked()
{
    resetColors();
    PurchaseRegisterButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    PurchaseRegisterWidget = new PurchaseRegister();
    ui->widgetLayout->addWidget(PurchaseRegisterWidget);
    PurchaseRegisterWidget->setFocus();
}

void Purchases_SideBar::PurchaseOrderBookClicked()
{
    resetColors();
    PurchaseOrderBookButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    PurchaseOrderBookWidget = new PurchaseOrderBook();
    ui->widgetLayout->addWidget(PurchaseOrderBookWidget);
    PurchaseOrderBookWidget->setFocus();
}

void Purchases_SideBar::StockReportClicked()
{
    resetColors();
    StockReportButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    StockReportWidget = new RestaurantClosingStock();
    ui->widgetLayout->addWidget(StockReportWidget);
    StockReportWidget->setFocus();
}

void Purchases_SideBar::PaymentsOutstandingClicked()
{
    resetColors();
    PaymentOutstandingButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    PaymentOutstandingWidget = new PaymentOutstanding();
    ui->widgetLayout->addWidget(PaymentOutstandingWidget);
    PaymentOutstandingWidget->setFocus();
}

void Purchases_SideBar::ReservedItemsClicked()
{
    resetColors();
    ReservedItemsButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    ItemsReservedWidget = new ItemsReserved();
    ui->widgetLayout->addWidget(ItemsReservedWidget);
    ItemsReservedWidget->setFocus();
}

void Purchases_SideBar::CostEstimatesClicked()
{
    resetColors();
    CostEstimatesButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    CostEstimatesWidget = new CostEstimates();
    ui->widgetLayout->addWidget(CostEstimatesWidget);
    CostEstimatesWidget->setFocus();
}
