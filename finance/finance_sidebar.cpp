#include "finance_sidebar.h"
#include "ui_finance_sidebar.h"
#include <QDebug>

Finance_SideBar::Finance_SideBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Finance_SideBar)
{
    ui->setupUi(this);
    ui->scrollArea->setStyleSheet("background-color:qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:0.4, stop:0 #424242, stop:1 #424242)");

    ui->verticalLayout_3->setAlignment(Qt::AlignTop);
    Section* sectionMaster      = new Section("Masters"     , 300,new QIcon(":/icons/gm.ico"), this);
    Section* sectionTransaction = new Section("Transactions", 300,new QIcon(":/icons/gm.ico"), this);
    Section* sectionBanking     = new Section("Banks"       , 300,new QIcon(":/icons/gm.ico"), this);
    Section* sectionReports     = new Section("Reports", 300,new QIcon(":/icons/gm.ico"), this);

    ui->verticalLayout_3->addWidget(sectionMaster);
    ui->verticalLayout_3->addWidget(getLine());
    ui->verticalLayout_3->addWidget(sectionTransaction);
    ui->verticalLayout_3->addWidget(getLine());
    ui->verticalLayout_3->addWidget(sectionBanking);
    ui->verticalLayout_3->addWidget(getLine());
    ui->verticalLayout_3->addWidget(sectionReports);
    ui->verticalLayout_3->addWidget(getLine());

    accountsButton = new SideBarButton("Account Groups");
    Ledger = new SideBarButton("Ledgers");
    TermsButton = new SideBarButton("Terms & Conditions");
    LedgerOpening = new SideBarButton("Ledger Opening");

    bankReconciliationButton = new SideBarButton("Bank Reconciliation");
    notification = new SideBarButton("Notifications");
    quotationButton = new SideBarButton("Quotations");
    salesInvoice = new SideBarButton("Sales Invoices");
    POSButton = new SideBarButton("DEMO POS");
    purchaseInvoice = new SideBarButton("Purchase Invoices");
    salesOrdersButton = new SideBarButton("Sales Orders");
    purchaseOrdersButton = new SideBarButton("Purchase Orders");
    payment = new SideBarButton("Payments");
    receipt = new SideBarButton("Receipts");
    journalButton = new SideBarButton("Journals");
    creditNoteButton = new SideBarButton("Credit Notes");
    debitNoteButton = new SideBarButton("Debit Notes");
    deliveryNote = new SideBarButton("Delivery Notes");
    proformaButton = new SideBarButton("Proforma");

    DatewiseSales  = new SideBarButton("Datewise Sales");
    SalesMarginButton = new SideBarButton("Sales Margins");

    templateButton = new SideBarButton("Template Vouchers");

    ContraVoucherButton = new SideBarButton("Contra Voucher");
    bankReconciliationButton = new SideBarButton("Bank Reconcile");
    notification = new SideBarButton("Notifications");

    DayBookButton = new SideBarButton("DayBook");
    LedgerReportButton = new SideBarButton("Ledger Report");
    CustomerReportButton = new SideBarButton("Customer Report");
    GroupSummaryButton = new SideBarButton("Group Summary");
    GroupVouchersButton = new SideBarButton("Group Vouchers");
    CashBankBookButton = new SideBarButton("Cash/Bank Book");
    GroupWiseOutstandingButton = new SideBarButton("Groupwise Outstanding");
    LedgerwiseOutstandingButton = new SideBarButton("Ledgerwise Outstanding");
    SalesmanwiseButton = new SideBarButton("Salesmanwise Reports");
    TrialBalanceButton = new SideBarButton("Trial Balance");
    BalanceSheetButton = new SideBarButton("Balance Sheet");
    ProfitLossButton = new SideBarButton("Profit and Loss");
    QString taxnameStr = ConfigurationSettingsDatabaseHelper::getValue(taxName,'Tax').toString();
    GSTReportButton = new SideBarButton(taxnameStr+" Report");

    DiscountButton= new SideBarButton("Discount Reports");
    CompliButton= new SideBarButton("Complimentary Reports");
    ShiftButton= new SideBarButton("Shift Reports");
    BillwiseButton = new SideBarButton("Billwise Report");

    auto* masterLayout = new QVBoxLayout();
    auto* transactionLayout = new QVBoxLayout();
    auto* bankingLayout = new QVBoxLayout();
    auto* reportsLayout  = new QVBoxLayout();

    masterLayout ->addWidget(accountsButton);
    masterLayout ->addWidget(Ledger);
    masterLayout ->addWidget(TermsButton);
    masterLayout ->addWidget(LedgerOpening);


    sectionMaster->setContentLayout(*masterLayout );

    transactionLayout->addWidget(salesInvoice);
    transactionLayout->addWidget(purchaseInvoice);
    transactionLayout->addWidget(salesOrdersButton);
    transactionLayout->addWidget(purchaseOrdersButton);
    transactionLayout->addWidget(payment);
    transactionLayout->addWidget(receipt);
    transactionLayout->addWidget(journalButton);
    transactionLayout->addWidget(creditNoteButton);
    transactionLayout->addWidget(debitNoteButton);
    transactionLayout->addWidget(deliveryNote);
    transactionLayout->addWidget(proformaButton);
    transactionLayout->addWidget(quotationButton);

    transactionLayout->addWidget(POSButton);


    transactionLayout->addWidget(notification);
    transactionLayout->addWidget(templateButton);

    reportsLayout->addWidget(DayBookButton);
    reportsLayout->addWidget(LedgerReportButton);
    reportsLayout->addWidget(CustomerReportButton);
    reportsLayout->addWidget(BillwiseButton);
    reportsLayout->addWidget(DatewiseSales);

    reportsLayout->addWidget(GroupSummaryButton);
    reportsLayout->addWidget(GroupVouchersButton);GroupVouchersButton->hide();
    reportsLayout->addWidget(CashBankBookButton);
    reportsLayout->addWidget(GroupWiseOutstandingButton);
    reportsLayout->addWidget(LedgerwiseOutstandingButton);
    reportsLayout->addWidget(SalesmanwiseButton);
    reportsLayout->addWidget(DiscountButton);
    reportsLayout->addWidget(CompliButton);
    reportsLayout->addWidget(ShiftButton);
    reportsLayout->addWidget(GSTReportButton);

    reportsLayout->addWidget(TrialBalanceButton);
    reportsLayout->addWidget(ProfitLossButton);
    reportsLayout->addWidget(BalanceSheetButton);
    reportsLayout->addWidget(DiscountButton);
    reportsLayout->addWidget(CompliButton);
    reportsLayout->addWidget(ShiftButton);



    bankingLayout->addWidget(ContraVoucherButton);
    bankingLayout->addWidget(bankReconciliationButton);

    sectionTransaction->setContentLayout(*transactionLayout);
    sectionBanking->setContentLayout(*bankingLayout);
    sectionReports->setContentLayout(*reportsLayout);

    connect(accountsButton,SIGNAL(clicked(bool)),this,SLOT(accountsGroupClicked()));
    connect(Ledger,SIGNAL(clicked(bool)),this,SLOT(LedgerClicked()));
    connect(LedgerOpening,SIGNAL(clicked(bool)),this,SLOT(OpeningClicked()));
    connect(TermsButton,SIGNAL(clicked(bool)),this,SLOT(TermsButtonClicked()));
    connect(salesInvoice,SIGNAL(clicked(bool)),this,SLOT(salesInvoiceClicked()));
    connect(POSButton,SIGNAL(clicked(bool)),this,SLOT(POSClicked()));
    connect(salesOrdersButton,SIGNAL(clicked(bool)),this,SLOT(salesOrdersClicked()));
    connect(purchaseInvoice,SIGNAL(clicked(bool)),this,SLOT(purchaseInvoiceClicked()));
    connect(purchaseOrdersButton,SIGNAL(clicked(bool)),this,SLOT(purchaseOrdersClicked()));
    connect(creditNoteButton,SIGNAL(clicked(bool)),this,SLOT(creditNoteClicked()));
    connect(debitNoteButton,SIGNAL(clicked(bool)),this,SLOT(debitNoteClicked()));
    connect(payment,SIGNAL(clicked(bool)),this,SLOT(paymentClicked()));
    connect(receipt,SIGNAL(clicked(bool)),this,SLOT(receiptClicked()));
    connect(deliveryNote,SIGNAL(clicked(bool)),this,SLOT(deliveryNoteClicked()));
    connect(notification,SIGNAL(clicked(bool)),this,SLOT(notificationClicked()));
    connect(journalButton,SIGNAL(clicked(bool)),this,SLOT(journalClicked()));
    connect(ContraVoucherButton,SIGNAL(clicked(bool)),this,SLOT(contraVoucherClicked()));
    connect(proformaButton,SIGNAL(clicked(bool)),this,SLOT(proformaInvoiceClicked()));
    connect(quotationButton,SIGNAL(clicked(bool)),this,SLOT(quotationInvoiceClicked()));
    connect(templateButton,SIGNAL(clicked(bool)),this,SLOT(TemplateButtonClicked()));
    connect(DayBookButton,SIGNAL(clicked(bool)),this,SLOT(DayBookClicked()));
    connect(LedgerReportButton,SIGNAL(clicked(bool)),this,SLOT(LedgerReportClicked()));
    connect(CustomerReportButton,SIGNAL(clicked(bool)),this,SLOT(CustomerReportClicked()));
    connect(GroupSummaryButton,SIGNAL(clicked(bool)),this,SLOT(GroupSummaryClicked()));
    connect(GroupVouchersButton,SIGNAL(clicked(bool)),this,SLOT(GroupVouchersClicked()));
    connect(CashBankBookButton,SIGNAL(clicked(bool)),this,SLOT(CashBankBookClicked()));
    connect(GroupWiseOutstandingButton,SIGNAL(clicked(bool)),this,SLOT(GroupWiseOutstandingClicked()));
    connect(DatewiseSales,SIGNAL(clicked(bool)),this,SLOT(DatewiseSalesClicked()));
    connect(LedgerwiseOutstandingButton,SIGNAL(clicked(bool)),this,SLOT(LedgerwiseOutstandingClicked()));
    connect(SalesmanwiseButton,SIGNAL(clicked(bool)),this,SLOT(SalesmanwiseClicked()));
    connect(TrialBalanceButton,SIGNAL(clicked(bool)),this,SLOT(TrialBalanceClicked()));
    connect(BalanceSheetButton,SIGNAL(clicked(bool)),this,SLOT(BalanceSheetClicked()));
    connect(ProfitLossButton,SIGNAL(clicked(bool)),this,SLOT(ProfitLossClicked()));
    connect(GSTReportButton,SIGNAL(clicked(bool)),this,SLOT(GSTReportClicked()));


    connect(DiscountButton,SIGNAL(clicked(bool)),this,SLOT(DiscountClicked()));
    connect(CompliButton,SIGNAL(clicked(bool)),this,SLOT(CompliClicked()));
    connect(ShiftButton,SIGNAL(clicked(bool)),this,SLOT(ShiftReportClicked()));
    connect(BillwiseButton,SIGNAL(clicked(bool)),this,SLOT(billwiseClicked()));

    connect(bankReconciliationButton, SIGNAL(clicked(bool)), this, SLOT(bankReconciliationClicked()));


    resetAllColors();
    dash = new DefaultDashBoard(this);
    ui->widgetLayout->addWidget(dash);

}

Finance_SideBar::~Finance_SideBar()
{
    delete ui;
}

QFrame *Finance_SideBar::getLine()
{
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    return line;
}

void Finance_SideBar::resetAllColors()
{
    accountsButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    Ledger->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    LedgerOpening->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    TermsButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    salesInvoice->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");;
    purchaseInvoice->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");;
    salesOrdersButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");;
    purchaseOrdersButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    payment->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    receipt->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    journalButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    creditNoteButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    debitNoteButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    deliveryNote->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    DayBookButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    LedgerReportButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    CustomerReportButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    GroupSummaryButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    DatewiseSales->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    GroupVouchersButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    CashBankBookButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    GroupWiseOutstandingButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    LedgerwiseOutstandingButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    SalesmanwiseButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    TrialBalanceButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    BalanceSheetButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    ProfitLossButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    GSTReportButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    ContraVoucherButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    notification->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    proformaButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    quotationButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    templateButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    DiscountButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    CompliButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    ShiftButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    BillwiseButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    POSButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    bankReconciliationButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");

    clearLayout();

}

void Finance_SideBar::accountsGroupClicked()
{
    resetAllColors();
    accountsButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);");
    accountGroupsWidget = new accountGroups(this);
    ui->widgetLayout->addWidget(accountGroupsWidget);
    accountGroupsWidget->setFocus();
}

void Finance_SideBar::OpeningClicked()
{
    resetAllColors();
    LedgerOpeningValuesWidget = new LedgerOpeningValues(this);
    ui->widgetLayout->addWidget(LedgerOpeningValuesWidget);
    LedgerOpeningValuesWidget ->setFocus();
}

void Finance_SideBar::TermsButtonClicked()
{
    resetAllColors();
    TermsButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);");
    TermsListWidget= new TermsList(this);
    ui->widgetLayout->addWidget(TermsListWidget);
    TermsListWidget->setFocus();
}

void Finance_SideBar::LedgerClicked()
{
    resetAllColors();
    Ledger->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0)");
    ledgerMasterWidget = new ledgerMaster(this);
    QObject::connect(this,SIGNAL(searchTextchanged(QString)),ledgerMasterWidget ,SLOT(searchTextChanged(QString)));

    ui->widgetLayout->addWidget(ledgerMasterWidget);
    ledgerMasterWidget->setFocus();
}

void Finance_SideBar::salesInvoiceClicked()
{
    resetAllColors();
    salesInvoice->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    SalesVoucherListWidget = new SalesVoucherList();
    ui->widgetLayout->addWidget(SalesVoucherListWidget);
    SalesVoucherListWidget->setFocus();
}

void Finance_SideBar::POSClicked()
{
    resetAllColors();

}

void Finance_SideBar::purchaseInvoiceClicked()
{
    resetAllColors();
    purchaseInvoice->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    PurchaseEntryWidget = new PurchaseEntry();
    ui->widgetLayout->addWidget(PurchaseEntryWidget);
    PurchaseEntryWidget->setFocus();
}

void Finance_SideBar::salesOrdersClicked()
{
    resetAllColors();
    salesOrdersButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    salesOrdersWidget = new salesOrders();
    ui->widgetLayout->addWidget(salesOrdersWidget);
    salesOrdersWidget->setFocus();
}

void Finance_SideBar::purchaseOrdersClicked()
{
    resetAllColors();
    purchaseOrdersButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    purchaseOrdersWidget = new purchaseOrders();
    ui->widgetLayout->addWidget(purchaseOrdersWidget);
    purchaseOrdersWidget->setFocus();
}

void Finance_SideBar::paymentClicked()
{
    resetAllColors();
    payment->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    PaymentWidget = new Payment();
    ui->widgetLayout->addWidget(PaymentWidget);
    PaymentWidget->setFocus();
}

void Finance_SideBar::receiptClicked()
{
    resetAllColors();
    receipt->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    ReceiptEntryWidget = new ReceiptEntry();
    ui->widgetLayout->addWidget(ReceiptEntryWidget);
    ReceiptEntryWidget->setFocus();
}

void Finance_SideBar::journalClicked()
{
    resetAllColors();
    journalButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    journalWidget = new journal();
    ui->widgetLayout->addWidget(journalWidget);
    journalWidget->setFocus();
}

void Finance_SideBar::creditNoteClicked()
{
    resetAllColors();
    creditNoteButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    creditNoteWidget = new creditNote();
    ui->widgetLayout->addWidget(creditNoteWidget);
    creditNoteWidget->setFocus();
}

void Finance_SideBar::debitNoteClicked()
{
    resetAllColors();
    debitNoteButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    debitNoteWidget = new debitNote();
    ui->widgetLayout->addWidget(debitNoteWidget);
    debitNoteWidget->setFocus();
}

void Finance_SideBar::deliveryNoteClicked()
{
    resetAllColors();
    deliveryNote->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    deliveryNotesWidget = new deliveryNotes();
    ui->widgetLayout->addWidget(deliveryNotesWidget);
    deliveryNotesWidget->setFocus();
}


void Finance_SideBar::contraVoucherClicked()
{
    resetAllColors();
    ContraVoucherButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    contraVoucherListWidget = new ContraVoucherList();
    ui->widgetLayout->addWidget(contraVoucherListWidget);
    contraVoucherListWidget->setFocus();
}

void Finance_SideBar::proformaInvoiceClicked()
{
    resetAllColors();
    proformaButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    ProformaInvoiceListWidget = new ProformaInvoiceList();
    ui->widgetLayout->addWidget(ProformaInvoiceListWidget);
    ProformaInvoiceListWidget->setFocus();
}

void Finance_SideBar::quotationInvoiceClicked()
{
    resetAllColors();
    quotationButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    QuotationWidget = new QuotaitonsList();
    ui->widgetLayout->addWidget(QuotationWidget);
    QuotationWidget->setFocus();
}

void Finance_SideBar::margin()
{
    resetAllColors();
    quotationButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    QuotationWidget = new QuotaitonsList();
    ui->widgetLayout->addWidget(QuotationWidget);
    QuotationWidget->setFocus();
}


void Finance_SideBar::notificationClicked()
{
    resetAllColors();
    notification->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    notificationWidget = new NotificationsList();
    ui->widgetLayout->addWidget(notificationWidget);
    notificationWidget->setFocus();
}

void Finance_SideBar::bankReconciliationClicked()
{
    resetAllColors();
    bankReconciliationButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    bankReconWid = new BankReconciliation();
    ui->widgetLayout->addWidget(bankReconWid);
    bankReconWid->setFocus();
}

void Finance_SideBar::TemplateButtonClicked()
{
    resetAllColors();
    templateButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    TemplateList *tempList = new TemplateList();
    ui->widgetLayout->addWidget(tempList);
    tempList->setFocus();
}


void Finance_SideBar::DayBookClicked()
{
    resetAllColors();
    DayBookButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    DayBookWidget = new Daybook();
    ui->widgetLayout->addWidget(DayBookWidget);
    DayBookWidget->setFocus();
}

void Finance_SideBar::LedgerReportClicked()
{
    resetAllColors();
    LedgerReportButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    LedgerReportWidget = new LedgerReport;
    ui->widgetLayout->addWidget(LedgerReportWidget);
    LedgerReportWidget->setFocus();
}

void Finance_SideBar::CustomerReportClicked()
{
    resetAllColors();
    CustomerReportButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    CustomerReportWidget = new CustomerReport;
    ui->widgetLayout->addWidget(CustomerReportWidget);
    CustomerReportWidget->setFocus();
}

void Finance_SideBar::GroupSummaryClicked()
{
    resetAllColors();
    GroupSummaryButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    GroupSummaryWidget = new GroupSummary();
    ui->widgetLayout->addWidget(GroupSummaryWidget);
    GroupSummaryWidget->setFocus();
}

void Finance_SideBar::GroupVouchersClicked()
{
    resetAllColors();
    GroupVouchersButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    GroupVouchersWidget = new GroupVouchers;
    ui->widgetLayout->addWidget(GroupVouchersWidget);
    GroupVouchersWidget->setFocus();
}

void Finance_SideBar::CashBankBookClicked()
{
    resetAllColors();
    CashBankBookButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    CashBankBookWidget = new CashBankBook;
    ui->widgetLayout->addWidget(CashBankBookWidget);
    CashBankBookWidget->setFocus();
}

void Finance_SideBar::GroupWiseOutstandingClicked()
{
    resetAllColors();
    GroupWiseOutstandingButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    GroupwiseOutstandingWidget = new GroupwiseOutstanding;
    ui->widgetLayout->addWidget(GroupwiseOutstandingWidget);
    GroupwiseOutstandingWidget->setFocus();
}

void Finance_SideBar::DatewiseSalesClicked()
{
    resetAllColors();
    DatewiseSales->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    DatewiseSalesReport* BillwiseReportWidget = new DatewiseSalesReport(this);
    ui->widgetLayout->addWidget(BillwiseReportWidget);
    BillwiseReportWidget->setFocus();


}

void Finance_SideBar::LedgerwiseOutstandingClicked()
{
    resetAllColors();
    LedgerwiseOutstandingButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
//    LedgerwiseOutstandingWidget = new LedgerwiseOutstanding;
//    ui->widgetLayout->addWidget(LedgerwiseOutstandingWidget);
//    LedgerwiseOutstandingWidget->setFocus();

//    UiController *uiCtrl = new UiController;
//    uiCtrl->setUiData(15);
//    ui->widgetLayout->addWidget(uiCtrl->reportWid);

}

void Finance_SideBar::SalesmanwiseClicked()
{
    resetAllColors();
    SalesmanwiseButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    waiterwiseWidget = new WaiterWiseSalesReport();
    ui->widgetLayout->addWidget(waiterwiseWidget);
    waiterwiseWidget->setFocus();

    return;

    SalesmanwiseReportWidget = new SalesmanwiseReport;
    ui->widgetLayout->addWidget(SalesmanwiseReportWidget);
    SalesmanwiseReportWidget->setFocus();
}

void Finance_SideBar::TrialBalanceClicked()
{
    resetAllColors();
    TrialBalanceButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    TrialBalanceWidget = new TrialBalance;
    ui->widgetLayout->addWidget(TrialBalanceWidget);
    TrialBalanceWidget->setFocus();
}

void Finance_SideBar::BalanceSheetClicked()
{
    resetAllColors();
    BalanceSheetButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    BalanceSheetWidget = new BalanceSheet;
    ui->widgetLayout->addWidget(BalanceSheetWidget);
    BalanceSheetWidget->setFocus();
}

void Finance_SideBar::ProfitLossClicked()
{
    resetAllColors();
    ProfitLossButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    ProfitAndLossReportWidget = new ProfitAndLossReport;
    ui->widgetLayout->addWidget(ProfitAndLossReportWidget);
    ProfitAndLossReportWidget->setFocus();
}

void Finance_SideBar::GSTReportClicked()
{
    resetAllColors();
    qDebug()<<" GST clicked";
    GSTReportButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    GSTReportWidget = new GSTReport;
    qDebug()<<"show";
    ui->widgetLayout->addWidget(GSTReportWidget);
    GSTReportWidget->setFocus();
}

void Finance_SideBar::DiscountClicked()
{
    resetAllColors();
    DiscountButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    DiscReportWidget = new DiscountsReport;
    ui->widgetLayout->addWidget(DiscReportWidget);
    DiscReportWidget->setFocus();
}

void Finance_SideBar::CompliClicked()
{
    resetAllColors();
    CompliButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    compliWidget = new ComplimentarySalesReport;
    ui->widgetLayout->addWidget(compliWidget);
    compliWidget->setFocus();
}

void Finance_SideBar::ShiftReportClicked()
{
    resetAllColors();
    ShiftButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    shiftReport = new ShiftWiseReport;
    ui->widgetLayout->addWidget(shiftReport);
    shiftReport->setFocus();
}

void Finance_SideBar::billwiseClicked()
{
    resetAllColors();
    BillwiseButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);SideBarButton {border: none;}");
    BillwiseReport *report = new BillwiseReport;
    ui->widgetLayout->addWidget(report);
    report->setFocus();
}

void Finance_SideBar::searchText(QString text)
{
    emit searchTextchanged(text);
}

void Finance_SideBar::clearLayout()
{
    if (ui->widgetLayout != NULL )
    {
        qDebug()<<"clearing Finance Layout";

        QLayoutItem* item;
        while (( item = ui->widgetLayout->takeAt(0)) != NULL )
        {
            delete item->widget();
            delete item;
        }

    }
    qDebug()<<"Finance Layout cleared";

}

