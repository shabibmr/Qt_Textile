#include "customerreport.h"
#include "ui_customerreport.h"

CustomerReport::CustomerReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomerReport)
{
    ui->setupUi(this);

    ui->label_2->hide();
    ui->lineEdit->hide();
    ui->horizontalSpacer_3->changeSize(0,0);
    ui->tableView_2->hide();
    ui->pushButton->hide();

    ui->tabWidget->removeTab(1);

    ui->fromDateDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateDateEdit->setDate(QDate::currentDate());

    ui->openingBalanceLineEdit->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->debitTotalLineEdit->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->creditTotalLineEdit->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->balanceLineEdit->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    CustomerReportDbHelper = new LedgerHelper;


    printerHelper = new CounterPrinterHelper;

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->phoneLineEdit, &QLineEdit::textChanged, this, [=](QString val){
        phone = val;
    });

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
//    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&CustomerReport::showShareOptions);

    ui->horizontalLayout->addWidget(shareButton);
    ui->excelButton->hide();
    ui->printButton->hide();


    setTableView();
    this->setFocus();

    salesHelper = new salesVoucherDatabaseHelper;
    purchaseHelper = new PurchaseVoucherDatabaseHelper;
    paymentHelper = new PaymentVoucherDatabaseHelper;
    receiptHelper = new ReceiptVoucherDatabaseHelper;
    journalHelper = new JournalVoucherDatabaseHelper;
    creditHelper = new CreditNoteVoucherDatabaseHelper;
    debitHelper = new DebitNoteVoucherDatabaseHelper;
    salesOrderHelper = new SalesOrderDatabaseHelper;

}


CustomerReport::~CustomerReport()
{
    delete ui;
}


void CustomerReport::setTableView()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();
    model= CustomerReportDbHelper->getContactwiseLedgerReportModel(phone,fromDate,toDate);
    //  ledgerListModel = CustomerReportDbHelper->getDayBookReportModel(fromDate,toDate);

    ui->tableView->setModel(model);
    ui->tableView->setItemDelegate(new LedgerReportDelegate());

    ui->tableView->hideColumn(5);

    openingBalance = CustomerReportDbHelper->getOpeningBalanceOfContact(phone,fromDate);
    crTotal = 0;
    drTotal = 0;
    dr =0;
    cr =0;

    for(int i=0;i<model->rowCount();i++){

        dr = model->record(i).value(3).toFloat();
        cr = model->record(i).value(4).toFloat();
        crTotal += cr>0?cr:0;
        drTotal += dr>0?dr:0;

    }

    ui->debitTotalLineEdit->setText(QString::number(drTotal,'f',2));
    ui->creditTotalLineEdit->setText(QString::number(crTotal,'f',2));

    closingBalance = crTotal- drTotal + openingBalance;
    QString CrDr = "Cr";
    if(closingBalance>=0){

    }
    else{
        CrDr="Dr";
        closingBalance *= -1;
    }
    ui->balanceLineEdit->setText(QString::number(closingBalance,'f',2)+ " "+ CrDr);

    CrDr = "Cr";
    if(openingBalance>=0){

    }
    else{
        CrDr="Dr";
        openingBalance *= -1;
    }
    ui->openingBalanceLineEdit->setText(QString::number(openingBalance,'f',2)+ " "+ CrDr);

}

void CustomerReport::on_OKButton_clicked()
{
    setTableView();
}

void CustomerReport::on_excelButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();
    QString reportName = "Customer Report";

    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QList<int> avlCols; avlCols<<0<<1<<2<<3<<4<<5;
    ex->GenerateReportFromTableView(fileName,reportName,
                                    widthList,ui->tableView,
                                    fromDate.toString("yyyy-MM-dd"),
                                    toDate.toString("yyyy-MM-dd"),
                                    avlCols);
}

void CustomerReport::on_printButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<20<<15<<15<<20<<10<<10<<10;
    AlignmentList<< 0<< 0 <<0<<0<<0<<1<<1;

    //    printer-> printReportGeneral(ui->tableView,widthList,"Ledger Report",fromDate,toDate,AlignmentList
    //                                 ,extraTop,extraBottom);

    printerHelper->printLedgerSimple(phone,
                               ui->tableView,
                               fromDate,
                               toDate,
                               openingBalance,
                               closingBalance
                               );
}

void CustomerReport::on_backButton_clicked()
{
    this->close();
}

void CustomerReport::on_tableView_doubleClicked(const QModelIndex &index)
{


    int row = index.row();

    QString voucherNo = model->record(row).value(1).toString();
    QString voucherType = model->record(row).value(2).toString();
    QString voucherPrefix = model->record(row).value(5).toString();
    qDebug()<<"Dowuble clicked";
    qDebug()<<voucherNo<<voucherType;

    if(voucherType == purchaseHelper->VoucherType ){
        PurchaseVoucherEditor* voucherWidget = new PurchaseVoucherEditor;
        GeneralVoucherDataObject *voucher = purchaseHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget->showVoucher(voucher);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTable()));
    }
    if(voucherType == salesOrderHelper->VoucherType ){
        SalesOrderVoucherEditor* voucherWidget = new SalesOrderVoucherEditor;
        GeneralVoucherDataObject *voucher = salesOrderHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget->showVoucher(voucher);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTable()));
    }
    else if(voucherType ==  salesHelper->VoucherType){
        SalesVoucherEditor *voucherWidget = new SalesVoucherEditor(new salesVoucherDatabaseHelper());
        GeneralVoucherDataObject* voucher = salesHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget->showVoucher(voucher);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTable()));
    }
    else if(voucherType ==  paymentHelper->VoucherType ){
        PaymentVoucherEditor* voucherWidget = new PaymentVoucherEditor;
        GeneralVoucherDataObject* voucher = paymentHelper->getVoucherByVoucherNoAsPtr(voucherNo, voucherPrefix);
        voucherWidget->showVoucher(voucher);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTable()));
    }
    else if(voucherType ==  receiptHelper->VoucherType ){
        ReceiptVoucherEditor* voucherWidget = new ReceiptVoucherEditor;
        GeneralVoucherDataObject* voucher = receiptHelper->getVoucherByVoucherNoAsPtr(voucherNo, voucherPrefix);
        voucherWidget->showVoucher(voucher);
        voucherWidget->showMaximized();
    }
    else if(voucherType ==  journalHelper->VoucherType ){
        JournalVoucherEditor* voucherWidget = new JournalVoucherEditor;
        GeneralVoucherDataObject* voucher = journalHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget->showVoucher(voucher);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTable()));
    }

    else if(voucherType ==  creditHelper->VoucherType ){
        CreditNoteVoucherEditor* voucherWidget = new CreditNoteVoucherEditor;
        GeneralVoucherDataObject* voucher = creditHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget->showVoucher(voucher);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTable()));
    }
    else if(voucherType ==  debitHelper->VoucherType ){
        DebitNoteVoucherEditor* voucherWidget= new DebitNoteVoucherEditor;
        GeneralVoucherDataObject* voucher = debitHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget->showVoucher(voucher);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTable()));
    }
}

void CustomerReport::on_tabWidget_tabBarClicked(int index)
{
    qDebug()<<"open tab"<<index;
}

void CustomerReport::on_phoneLineEdit_editingFinished()
{
    setTableView();
}

void CustomerReport::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Customer Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
//    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&CustomerReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&CustomerReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&CustomerReport::on_excelButton_clicked);
}

void CustomerReport::printReport(QPrinter *printer)
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<20<<15<<15<<20<<10<<10<<10;
    AlignmentList<< 0<< 0 <<0<<0<<0<<1<<1;

    //    printer-> printReportGeneral(ui->tableView,widthList,"Ledger Report",fromDate,toDate,AlignmentList
    //                                 ,extraTop,extraBottom);

    printer->setPrinterName("printerName");
    printerHelper->setPrinter(printer);
    printerHelper->printLedgerSimple(phone,
                               ui->tableView,
                               fromDate,
                               toDate,
                               openingBalance,
                               closingBalance
                               );

}
