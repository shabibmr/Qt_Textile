#include "ledgerreport.h"
#include "ui_ledgerreport.h"

#include <QFileDialog>
#include "customwidgets/test/ledgerreportdelegate.h"


LedgerReport::LedgerReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LedgerReport)
{
    ui->setupUi(this);

    ui->label_2->hide();
    ui->lineEdit->hide();
    ui->horizontalSpacer_3->changeSize(0,0);
    ui->pushButton->hide();

    tableView = new ReportsTableView("");

    ui->verticalLayout_2->addWidget(tableView);

    tableView->setItemDelegate(new LedgerReportDelegate());
//    if(!ConfigurationSettingsDatabaseHelper::getValue(showBillwiseMapping,false).toBool())
//        ui->tabWidget->removeTab(1);



    ui->fromDateDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateDateEdit->setDate(QDate::currentDate());

    ui->openingBalanceLineEdit->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->debitTotalLineEdit->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->creditTotalLineEdit->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->balanceLineEdit->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    ledHelper = new LedgerMasterDatabaseHelper();
    model = ledHelper->getLedgerQueryString();
    ledgerReportDbHelper = new LedgerHelper;

    salesHelper = new salesVoucherDatabaseHelper;
    purchaseHelper = new PurchaseVoucherDatabaseHelper;
    paymentHelper = new PaymentVoucherDatabaseHelper;
    receiptHelper = new ReceiptVoucherDatabaseHelper;
    journalHelper = new JournalVoucherDatabaseHelper;
    creditHelper = new CreditNoteVoucherDatabaseHelper;
    debitHelper = new DebitNoteVoucherDatabaseHelper;


    allLedgersPtr = ledHelper->getAllLedgersAsPtr();

    printerHelper = new CounterPrinterHelper;

    connect(tableView, &QTableView::doubleClicked, this,&LedgerReport::on_tableView_doubleClicked);

//    tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

    GMLedgerSearch *GMSearch = new GMLedgerSearch(allLedgersPtr);
    QPointer<QToolButton> SearchWid = new QToolButton;
    SearchWid->setMinimumWidth(250);
    QPointer<QVBoxLayout> lay = new QVBoxLayout();
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    lay->addWidget(GMSearch);
    SearchWid->setLayout(lay);
    QObject::connect(SearchWid,SIGNAL(clicked(bool)),GMSearch,SLOT(setFocus()));
    QObject::connect(GMSearch,SIGNAL(SelectedLedger(LedgerMasterDataModel)),
                     this,SLOT(setLedgerName(LedgerMasterDataModel)));

    ui->ledNameLay->addWidget(SearchWid);

    ui->ledgerNameComboBox->setModel(model);
    ui->ledgerNameComboBox->hide();

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);

    //    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&LedgerReport::showShareOptions);

    ui->horizontalLayout->addWidget(shareButton);
    ui->excelButton->hide();
    ui->printButton->hide();




    //    ui->tabWidget->removeTab(1);

    billwiseWidget = new AddBillwiseRecord() ;
    billwiseWidget->setWindowFlags(Qt::Window);
    billwiseWidget->setAttribute(Qt::WA_DeleteOnClose);
    ui->widLay->addWidget(billwiseWidget);
    //    setTable();
    setTableView();
    setBillwiseWidget();
    this->setFocus();

}

void LedgerReport::setBillwiseWidget()
{
    billwiseWidget->setLedger(&currentLedger);
    billwiseWidget->showPendingBills();
//    billwiseWidget->hide();
}


LedgerReport::~LedgerReport()
{
    delete ui;
}


void LedgerReport::setTableView()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();
    ledgerListModel= ledgerReportDbHelper->getLedgerReportModel(currentLedger.LedgerID,fromDate,toDate);
    //  ledgerListModel = ledgerReportDbHelper->getDayBookReportModel(fromDate,toDate);

    tableView->setModel(ledgerListModel);
    tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

    tableView->hideColumn(6);

    tableView->lastRow->hide();

    openingBalance = ledgerReportDbHelper->getOpeningBalanceOfLedger(currentLedger.LedgerID,fromDate);
    crTotal = 0;
    drTotal = 0;
    dr =0;
    cr =0;

    for(int i=0;i<tableView->model()->rowCount();i++){

        dr = ledgerListModel->record(i).value(4).toFloat();
        cr = ledgerListModel->record(i).value(5).toFloat();
        crTotal += cr>0?cr:0;
        drTotal += dr>0?dr:0;
    }

    QSqlQueryModel *totalModel = ledgerReportDbHelper->getLedgerTotalsModel(currentLedger.LedgerID,fromDate,toDate);
    drTotal = totalModel->record(0).value(0).toFloat();
    crTotal = totalModel->record(0).value(1).toFloat();

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

void LedgerReport::setLedgerName(LedgerMasterDataModel ledger)
{
    currentLedger = ledHelper->getLedgerObjectByID(ledger.LedgerID);
    LedgerMasterDataModel* led1 = new LedgerMasterDataModel();
    led1=ledHelper->getLedgerPtrById(ledger.LedgerID);
    qDebug()<<"Ledger*******"<<led1->LedgerID;
    billwiseWidget->setLedger(led1);
    billwiseWidget->showPendingBills(); //ledgerID
    billwiseWidget->show();
    setTableView();
    setBillwiseWidget();
    QObject::connect(billwiseWidget,SIGNAL(refresh(LedgerMasterDataModel)),this,SLOT(setLedgerName(LedgerMasterDataModel)));
}

void LedgerReport::on_OKButton_clicked()
{
    setTableView();
}

void LedgerReport::on_excelButton_clicked()
{
    qDebug()<<"create Excel";

    QDate fDate = ui->fromDateDateEdit->date();
    QDate tDate = ui->toDateDateEdit->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString wwaitername ="";

    QString reportName = "Ledger Report";

    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+currentLedger.LedgerName+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QStringList cols; cols<<"30"<<"20"<<"20"<<"20"<<"20"<<"20";
    //    reportName = "Time Based Sales Report "+wwaitername;
    QList<int> avlCols; avlCols<<0<<1<<2<<3<<4<<5;
    ex->GenericReportFromTableView(fileName,reportName,cols,tableView,fromDate,toDate,avlCols);
}

void LedgerReport::on_printButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<20<<15<<15<<20<<10<<10<<10;
    AlignmentList<< 0<< 0 <<0<<0<<0<<1<<1;

    //    printer-> printReportGeneral(tableView,widthList,"Ledger Report",fromDate,toDate,AlignmentList
    //                                 ,extraTop,extraBottom);

    QPrinter *qPrinter = new QPrinter(QPrinter::ScreenResolution);
    qPrinter->setPrinterName("printerName");
    printerHelper->setPrinter(qPrinter);
    printerHelper->printLedgerSimple(currentLedger.LedgerName,
                                     tableView,
                                     fromDate,
                                     toDate,
                                     openingBalance,
                                     closingBalance
                                     );
}

void LedgerReport::on_backButton_clicked()
{
    this->close();
}

void LedgerReport::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();

    QString voucherNo = ledgerListModel->record(row).value(2).toString();
    QString voucherType = ledgerListModel->record(row).value(3).toString();
    QString voucherPrefix = ledgerListModel->record(row).value(6).toString();
    qDebug()<<"Dowuble clicked";
    qDebug()<<voucherNo<<voucherType;

    VoucherEditorSelector *vEditor = new VoucherEditorSelector;
    vEditor->openEditor(voucherNo, voucherPrefix, voucherType);

}

void LedgerReport::on_tabWidget_tabBarClicked(int index)
{
    qDebug()<<"open tab"<<index;
}

void LedgerReport::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Ledger Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);
    //    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&LedgerReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&LedgerReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&LedgerReport::on_excelButton_clicked);

}

void LedgerReport::printReport(QPrinter *printer)
{
    qDebug()<<"printing report";
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<20<<15<<15<<20<<10<<10<<10;
    AlignmentList<< 0<< 0 <<0<<0<<0<<1<<1;

    //    printer-> printReportGeneral(tableView,widthList,"Ledger Report",fromDate,toDate,AlignmentList
    //                                 ,extraTop,extraBottom);

    printer->setPrinterName("printerName");
    printerHelper->setPrinter(printer);
    printerHelper->printLedgerSimple(currentLedger.LedgerName,
                                     tableView,
                                     fromDate,
                                     toDate,
                                     openingBalance,
                                     closingBalance
                                     );

}

