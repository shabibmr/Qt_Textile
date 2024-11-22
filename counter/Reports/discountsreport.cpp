#include "discountsreport.h"
#include "ui_discountsreport.h"

DiscountsReport::DiscountsReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiscountsReport)
{
    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate::currentDate().addDays(-30));
    ui->toDateDateEdit->setDate(QDate::currentDate());
    dbHelper = new LedgerHelper;
    ui->tabWidget->removeTab(0);
    salesHelper = new salesVoucherDatabaseHelper;

    ui->totalLine->setAlignment(Qt::AlignRight);

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
//    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&DiscountsReport::showShareOptions);

    ui->horizontalLayout->addWidget(shareButton);
    ui->excelButton->hide();
    ui->printButton->hide();

    setTable();
}

DiscountsReport::~DiscountsReport()
{
    delete ui;
}

void DiscountsReport::setTable()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = dbHelper->getDiscountedSales(fromDateTime,toDateTime);

    ui->amountWiseTableView->setModel(model);

    ui->amountWiseTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    float total = 0;
    for(int i=0;i<model->rowCount();i++){
        total += model->record(i).value(3).toFloat();
    }

    ui->totalLine->setText(QString::number(total,'f',2));



}

void DiscountsReport::on_OKButton_clicked()
{
    setTable();
}

void DiscountsReport::on_excelButton_clicked()
{
    qDebug()<<"create Excel";

    QDate fDate = ui->fromDateDateEdit->date();
    QDate tDate = ui->toDateDateEdit->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString wwaitername ="";

    QString reportName = "Discounted Sales Report";

    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QStringList cols; cols<<"30"<<"20"<<"20"<<"20";
    reportName = "Discounted Sales Report "+wwaitername;
    QList<int> avlCols; avlCols<<0<<1<<2<<3;
    ex->GenericReportFromTableView(fileName,reportName,cols,ui->amountWiseTableView,fromDate,toDate,avlCols);
}

void DiscountsReport::on_printButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<60<<20<<20;
    AlignmentList<< 0<< 1 <<1;
    CounterPrinterHelper *printer = new CounterPrinterHelper;
    QList<int> avlCols; avlCols<<0<<1<<2<<3;
    printer-> printReportGeneral(ui->amountWiseTableView
                                 ,widthList,"Discount Sales Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}

void DiscountsReport::on_backButton_clicked()
{
    this->close();
}

void DiscountsReport::on_amountWiseTableView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();

    QString voucherNo = model->record(row).value(0).toString();
//    QString voucherType = ledgerListModel->record(row).value(3).toString();
    qDebug()<<"Double clicked";

//    if(voucherType ==  salesHelper->VoucherType){
        SalesVoucherEditor *voucherWidget = new SalesVoucherEditor(new salesVoucherDatabaseHelper());
        GeneralVoucherDataObject* voucher = salesHelper->getVoucherByVoucherNoPtr(voucherNo,LoginValues::voucherPrefix);
//        voucherWidget->showVoucher(voucher);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTable()));
//    }

}

void DiscountsReport::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Discount Sales Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&DiscountsReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&DiscountsReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&DiscountsReport::on_excelButton_clicked);
}

void DiscountsReport::printReport(QPrinter *printer)
{
    printer->setPrinterName("printerName");
    printerHelper = new CounterPrinterHelper;
    printerHelper->setPrinter(printer);

    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<60<<20<<20;
    AlignmentList<< 0<< 1 <<1;
    QList<int> avlCols; avlCols<<0<<1<<2<<3;
    printerHelper-> printReportGeneral(ui->amountWiseTableView
                                 ,widthList,"Discount Sales Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}
