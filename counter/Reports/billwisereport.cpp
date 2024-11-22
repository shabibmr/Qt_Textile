#include "billwisereport.h"
#include "ui_billwisereport.h"
#include "customwidgets/appbarwidget.h"


BillwiseReport::BillwiseReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BillwiseReport)
{
    ui->setupUi(this);
    //    ui->fromDateDateEdit->setDate(QDate::currentDate());//.addDays(-30));
    //    ui->toDateDateEdit->setDate(QDate::currentDate());

    ui->label->hide();
    AppBarWidget *appBar = new  AppBarWidget("Billwise Report");
    appBar->backButton->hide();
    connect(appBar, &AppBarWidget::shareButtonClicked, this, &BillwiseReport::showShareOptions);
    ui->appBarLayout->addWidget(appBar);

    // qDebug()<<Q_FUNC_INFO<<__LINE__;
    filterWid = new FilterWidget(this);
    // qDebug()<<Q_FUNC_INFO<<__LINE__;
    filterWid->showFromDate(true);
    filterWid->showToDate(true);
    // qDebug()<<Q_FUNC_INFO<<__LINE__;

    ui->filterLayout->addWidget(filterWid);
    // qDebug()<<Q_FUNC_INFO<<__LINE__;
    connect(filterWid, &FilterWidget::refresh, this, &BillwiseReport::setValueTable);
    // qDebug()<<Q_FUNC_INFO<<__LINE__;

    //    QObject::connect(shareButton,&QPushButton::clicked,
    //                     this,&BillwiseReport::showShareOptions);
    QPalette pal = palette();
    pal.setColor(QPalette::Background,QColor("#ffffff"));
    setAutoFillBackground(true);
    setPalette(pal);
    // qDebug()<<Q_FUNC_INFO<<__LINE__;

    tableView = new ReportsTableView("Total");
    ui->tableLayout->addWidget(tableView);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(tableView, &ReportsTableView::clicked, this,&BillwiseReport::on_amountWiseTableView_clicked);
    tableView->verticalHeader()->show();
    // qDebug()<<Q_FUNC_INFO<<__LINE__;

    salesHelper = new salesVoucherDatabaseHelper();
    ledHelper = new LedgerHelper;
    ledgerHelper = new LedgerMasterDatabaseHelper();
    //    sort = new GMItemSortFilterProxyModel;
    tableView->setItemDelegate(new BillwiseReportDelegate());
    //    ui->totalLineEdit->setReadOnly(true);
    //    ui->totalLineEdit->setAlignment(Qt::AlignRight);
    // qDebug()<<Q_FUNC_INFO<<__LINE__;

    if(LoginValues::Privilege <= 4){
        ui->totalLineEdit->hide();
        ui->totalLabel->hide();
    }
    ui->printCopy->hide();
    // qDebug()<<Q_FUNC_INFO<<__LINE__;

    //    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    //    shareButton->setParent(this);
    //    //    shareButton->setColor(QColor(255,250,0));
    //    shareButton->setIconSize(QSize(30,30));

    //    ui->horizontalLayout_2->addWidget(shareButton);
    //    ui->excelButton->hide();
    //    ui->printButton->hide();

    ui->textEdit->clearFocus();
    //    setValueTable();
    // qDebug()<<Q_FUNC_INFO<<__LINE__;
}

BillwiseReport::~BillwiseReport()
{
    delete ui;
}

void BillwiseReport::setDeleted(bool value)
{
    deleted = value;
    if(deleted)
        ui->label->setText("Deleted Billwise Sales Report");

}

void BillwiseReport::setSalesHelper(VoucherDBAbstract *value)
{
    salesHelper = value;
    //    setValueTable();
}

void BillwiseReport::setValueTable()
{

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    QDate fromDate = filterWid->fromDate;
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = filterWid->toDate.addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));


    if(deleted){
        model = ledHelper->getDeletedSalesReportModelByDate(fromDateTime,toDateTime);
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        //        tableView->hideColumn(5);
    }
    else{
        model = ledHelper->getSalesReportModelByDate(fromDateTime,toDateTime);
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        //        tableView->hideColumn(4);
    }

    // qDebug()<<Q_FUNC_INFO<<__LINE__;
    //    sort = new GMItemSortFilterProxyModel(this);
    //    sort->setDynamicSortFilter(true);
    //    sort->setSourceModel(model);


    //   tableView->setModel(sort);
    filterWid->setSortModel(model, tableView, QList<int> {0,1,2,3});
    //    ui->amountWiseTableView->setSortingEnabled(true);
    if(deleted){
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        tableView->hideColumn(5);
    }
    else{
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        tableView->hideColumn(4);
    }

    bool show = ConfigurationSettingsDatabaseHelper::getValue(POSshowLedger,true).toBool();
    if(!show){
        tableView->hideColumn(2);
    }

    // qDebug()<<Q_FUNC_INFO<<__LINE__;

    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    float total = 0;
    for(int i=0;i<model->rowCount();i++){
        //        if(deleted)
        //            total += model->record(i).value(4).toFloat();
        //        else {
        total += model->record(i).value(3).toFloat();
        //        }
    }

    ui->totalLineEdit->setText(QString::number(total,'f',2));

    qDebug()<<Q_FUNC_INFO<<__LINE__;

}

void BillwiseReport::on_OKButton_clicked()
{
    setValueTable();
}

void BillwiseReport::on_excelButton_clicked()
{
    qDebug()<<"create Excel";

    QDate fDate =filterWid->fromDate;
    QDate tDate = filterWid->toDate;
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString wwaitername ="";

    QString reportName = "Billwise Sales Report";

    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QStringList cols; cols<<"30"<<"20"<<"20"<<"20";
    reportName = "Billwise Sales Report "+wwaitername;
    QList<int> avlCols; avlCols<<0<<1<<2<<3;
    ex->GenericReportFromTableView(fileName,reportName,cols,tableView,fromDate,toDate,avlCols);
}

void BillwiseReport::on_printButton_clicked()
{
    qDebug()<<"PRint";
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QString total = ui->totalLineEdit->text();
    QDate fromDate = filterWid->fromDate;
    QDate toDate = filterWid->toDate;

    extraBottom<< "Total: "<<""<<""<<total;
    QList<int> avlCols;
    avlCols<<0<<1<<2<<3;
    widthList <<20<<20<<40<<20;
    AlignmentList<< 0<< 1 << 0 <<1;


    // CounterPrinterHelper *printer = new CounterPrinterHelper;

    // printer-> printReportGeneral(tableView
    //                              ,widthList,"Sales Report",fromDate,toDate,AlignmentList
    //                              ,extraTop,extraBottom,avlCols);


    // int val = sender()->property("PrintType").toInt();
    // qDebug()<<Q_FUNC_INFO<<__LINE__<<val;
    // printer->setPrinterName("printerName");
    // printerHelper = new CounterPrinterHelper;
    // printerHelper->setPrinter(printer);



    // QStringList extraTop;
    // QStringList extraBottom;

    qDebug()<<Q_FUNC_INFO<<extraBottom<<__LINE__;


    printerHelper-> printReportGeneral(tableView
                                      ,widthList,"Billwise Report",
                                      fromDate,toDate,AlignmentList
                                      ,extraTop,extraBottom,avlCols);
    qDebug()<<__LINE__;
}

void BillwiseReport::on_backButton_clicked()
{
    this->close();
}

void BillwiseReport::on_amountWiseTableView_clicked(const QModelIndex &index)
{
    qDebug()<<Q_FUNC_INFO;
    // int row = index.row();// sort->mapToSource(index).row();
    int row = filterWid->getRow(index);
    QString voucherNo = model->record(row).value(1).toString();
    QString pref = model->record(row).value(4).toString();
    if(deleted)
        pref = model->record(row).value(5).toString();
    ui->printCopy->show();

    qDebug()<<"171"<<voucherNo<<pref;
    voucher = salesHelper->getVoucherByVoucherNoPtr(voucherNo, pref);

    calcTotals();
    setBillPreview();
}

void BillwiseReport::calcTotals()
{
    TransactionCalculator::VoucherCalculateSales(voucher);


}

void BillwiseReport::disableDelete()
{
    ui->deleteButton->hide();

}
void BillwiseReport::setBillPreview()
{
    CounterPrinterHelper pHelper;
    qDebug()<<"setting printer"<<voucher->voucherNumber<<voucher->InventoryItems.size();
    QString html = pHelper.getHtml(voucher);
    ui->textEdit->setHtml(html);
    qDebug()<<"Document Created";

}

void BillwiseReport::on_printCopy_clicked()
{
    CounterPrinterHelper pHelper;
    qDebug()<<"setting printer";
    pHelper.printSalesVoucher(voucher);
}

void BillwiseReport::on_modePay_clicked()
{
    ModeOfPay* ModeOfPayWidget = new ModeOfPay(voucher,ledgerHelper,new SalesOrderDatabaseHelper(),true);

    //ModeOfPayWidget->hideDiscounts();
    ModeOfPayWidget->show();
    QObject::connect(ModeOfPayWidget,SIGNAL(modeOfPaySignal()),this,SLOT(switchModeOfPay()));
}

void BillwiseReport::switchModeOfPay(){

    calcTotals();
    salesHelper->updateVoucher(voucher,voucher->voucherNumber);
    CounterPrinterHelper pHelper;
    pHelper.printSalesVoucher(voucher);
    setValueTable();
}

void BillwiseReport::on_deleteButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Delete","Delete Voucher : "+ voucher->voucherNumber,
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        //        if(LoginValues::Privilege >= 6){
        salesHelper->deleteVoucher(voucher);
        setValueTable();
        //        }
        //        else{
        //            QMessageBox box; box.setText("Sorry! You Do not have Privileges for the operation!!!");
        //            box.exec();
        //        }
    }
    else{
        qDebug()<<"Not Printing";
    }
}

void BillwiseReport::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Sales Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&BillwiseReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&BillwiseReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&BillwiseReport::on_excelButton_clicked);
}

void BillwiseReport::printReport(QPrinter *printer)
{
    printer->setPrinterName("printerName");
    printerHelper = new CounterPrinterHelper;
    printerHelper->setPrinter(printer);


    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QString total = ui->totalLineEdit->text();
    QDate fromDate = filterWid->fromDate;
    QDate toDate = filterWid->toDate;
    extraBottom<< "Total: "<<""<<"" <<total;
    QList<int> avlCols; avlCols<<0<<1<<2<<3;
    widthList <<20<<20<<40<<20;
    AlignmentList<< 0<< 1 << 0 <<1;
    qDebug()<<extraBottom;
    printerHelper-> printReportGeneral(tableView
                                      ,widthList,"Sales Report",fromDate,toDate,AlignmentList
                                      ,extraTop,extraBottom,avlCols);

}

void BillwiseReport::on_amountWiseTableView_doubleClicked(const QModelIndex &index)
{

}
