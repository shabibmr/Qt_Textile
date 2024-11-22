#include "complimentarysalesreport.h"
#include "ui_complimentarysalesreport.h"

ComplimentarySalesReport::ComplimentarySalesReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComplimentarySalesReport)
{
    ui->setupUi(this);
    salesHelper = new salesVoucherDatabaseHelper();
    ledHelper = new LedgerHelper;
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());
    ui->tabWidget->removeTab(1);
    //    setItemTableHeading();
    //    setItemTable();
    //    setValueTableHeading();

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
//    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&ComplimentarySalesReport::showShareOptions);

    ui->horizontalLayout->addWidget(shareButton);
    ui->excelButton->hide();
    ui->printButton->hide();

    setItemTable();
    setValueTable();
}

ComplimentarySalesReport::~ComplimentarySalesReport()
{
    delete ui;
}

void ComplimentarySalesReport::setItemTableHeading()
{
    //    ui->itemWiseTableWidget->setColumnCount(4);

    //    int i=0;

    //    QTableWidgetItem* dateItem = new QTableWidgetItem;
    //    dateItem->setText("Date");
    //    ui->itemWiseTableWidget->setHorizontalHeaderItem(i++,dateItem);

    //    QTableWidgetItem* LedgerName = new QTableWidgetItem;
    //    LedgerName->setText("Ledger");
    //    ui->itemWiseTableWidget->setHorizontalHeaderItem(i++,LedgerName);

    //    QTableWidgetItem* billNo = new QTableWidgetItem;
    //    billNo->setText("No.");
    //    ui->itemWiseTableWidget->setHorizontalHeaderItem(i++,billNo);

    //    QTableWidgetItem* Amount = new QTableWidgetItem;
    //    Amount->setText("Amount");
    //    ui->itemWiseTableWidget->setHorizontalHeaderItem(i++,Amount);

    //    ui->itemWiseTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    //    ui->itemWiseTableWidget->setShowGrid(false);

}

void ComplimentarySalesReport::setItemTable()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = ledHelper->getComplimentarySales(fromDateTime,toDateTime);
    ui->itemWiseTableView->setModel(model);

    ui->itemWiseTableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

    //    voucherList = salesHelper->getVoucherListByDate(dateFrom,dateTo);
    //    int cnt =0;
    //    int i=0;
    //    ui->itemWiseTableWidget->setRowCount(0);
    //    for(GeneralVoucherDataObject obj:voucherList){
    //        ui->itemWiseTableWidget->insertRow(cnt);
    //        i=0;

    //        QTableWidgetItem* dateItem = new QTableWidgetItem;
    //        dateItem->setText(obj.VoucherDate.toString("dd-MM-yyyy"));
    //        dateItem->setFlags(dateItem->flags() ^ Qt::ItemIsEditable);
    //        ui->itemWiseTableWidget->setItem(cnt,i++,dateItem);

    //        QTableWidgetItem* LedgerName = new QTableWidgetItem;
    //        LedgerName->setText(obj.ledgerObject.LedgerName);
    //        LedgerName->setFlags(LedgerName->flags() ^ Qt::ItemIsEditable);
    //        ui->itemWiseTableWidget->setItem(cnt,i++,LedgerName);

    //        QTableWidgetItem* billNo = new QTableWidgetItem;
    //        billNo->setText(obj.voucherNumber);
    //        billNo->setTextAlignment(Qt::AlignRight |Qt::AlignVCenter);
    //        billNo->setFlags(billNo->flags()^ Qt::ItemIsEditable);
    //        ui->itemWiseTableWidget->setItem(cnt,i++,billNo);

    //        QTableWidgetItem* Amount = new QTableWidgetItem;
    //        Amount->setText(QString::number(obj.grandTotal,'f',2));
    //        Amount->setTextAlignment(Qt::AlignRight |Qt::AlignVCenter);
    //        Amount->setFlags(Amount->flags()^ Qt::ItemIsEditable);
    //        ui->itemWiseTableWidget->setItem(cnt,i++,Amount);

    //        cnt++;
    //    }

}

void ComplimentarySalesReport::setValueTableHeading()
{
    //    ui->amountWiseTableWidget->setColumnCount(4);

    //    int i=0;

    //    QTableWidgetItem* dateItem = new QTableWidgetItem;
    //    dateItem->setText("Date");
    //    ui->amountWiseTableWidget->setHorizontalHeaderItem(i++,dateItem);

    //    QTableWidgetItem* LedgerName = new QTableWidgetItem;
    //    LedgerName->setText("Ledger");
    //    ui->amountWiseTableWidget->setHorizontalHeaderItem(i++,LedgerName);

    //    QTableWidgetItem* billNo = new QTableWidgetItem;
    //    billNo->setText("No.");
    //    ui->amountWiseTableWidget->setHorizontalHeaderItem(i++,billNo);

    //    QTableWidgetItem* Amount = new QTableWidgetItem;
    //    Amount->setText("Amount");
    //    ui->amountWiseTableWidget->setHorizontalHeaderItem(i++,Amount);

    //    ui->amountWiseTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    //    ui->amountWiseTableWidget->setShowGrid(false);
}

void ComplimentarySalesReport::setValueTable()
{

    qDebug()<<Q_FUNC_INFO;

    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    qDebug()<<"get model"<<toDate<<toDateTime;
    model = ledHelper->getComplimentarySales(fromDateTime,toDateTime);
    ui->amountWiseTableView->setModel(model);

    ui->amountWiseTableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

    //    int cnt =0;
    //    int i=0;
    //    ui->amountWiseTableWidget->setRowCount(0);
    //    for(GeneralVoucherDataObject obj:voucherList){
    //        ui->amountWiseTableWidget->insertRow(cnt);
    //        i=0;

    //        QTableWidgetItem* dateItem = new QTableWidgetItem;
    //        dateItem->setText(obj.VoucherDate.toString("dd-MM-yyyy"));
    //        dateItem->setFlags(dateItem->flags() ^ Qt::ItemIsEditable);
    //        ui->amountWiseTableWidget->setItem(cnt,i++,dateItem);

    //        QTableWidgetItem* LedgerName = new QTableWidgetItem;
    //        LedgerName->setText(obj.ledgerObject.LedgerName);
    //        LedgerName->setFlags(LedgerName->flags() ^ Qt::ItemIsEditable);
    //        ui->amountWiseTableWidget->setItem(cnt,i++,LedgerName);

    //        QTableWidgetItem* billNo = new QTableWidgetItem;
    //        billNo->setText(obj.voucherNumber);
    //        billNo->setTextAlignment(Qt::AlignRight |Qt::AlignVCenter);
    //        billNo->setFlags(billNo->flags()^ Qt::ItemIsEditable);
    //        ui->amountWiseTableWidget->setItem(cnt,i++,billNo);

    //        QTableWidgetItem* Amount = new QTableWidgetItem;
    //        Amount->setText(QString::number(obj.grandTotal,'f',2));
    //        Amount->setTextAlignment(Qt::AlignRight |Qt::AlignVCenter);
    //        Amount->setFlags(Amount->flags()^ Qt::ItemIsEditable);
    //        ui->amountWiseTableWidget->setItem(cnt,i++,Amount);

    //        cnt++;
    //    }
}

void ComplimentarySalesReport::on_OKButton_clicked()
{
    setItemTable();
    //    setValueTable();
}

void ComplimentarySalesReport::on_excelButton_clicked()
{
    qDebug()<<"create Excel";

    QDate fDate = ui->fromDateDateEdit->date();
    QDate tDate = ui->toDateDateEdit->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString wwaitername ="";

    QString reportName = "Complimentary Sales Report";

    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QStringList cols; cols<<"30"<<"20"<<"20";
    reportName = "Complimentary Sales Report "+wwaitername;
    QList<int> avlCols; avlCols<<0<<1<<2;
    ex->GenericReportFromTableView(fileName,reportName,cols,ui->itemWiseTableView,fromDate,toDate,avlCols);
}

void ComplimentarySalesReport::on_printButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();

    QList<int> avlCols; avlCols<<0<<1<<2;
    widthList <<60<<20<<20;
    AlignmentList<< 0<< 1 <<1;
    CounterPrinterHelper *printer = new CounterPrinterHelper;
    printer-> printReportGeneral(ui->amountWiseTableView
                                 ,widthList,"Complimentary Sales Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}

void ComplimentarySalesReport::on_backButton_clicked()
{
    this->close();
}

void ComplimentarySalesReport::on_amountWiseTableView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();

    QString voucherNo = model->record(row).value(0).toString();
    //    QString voucherType = ledgerListModel->record(row).value(3).toString();
    qDebug()<<"Double clicked";

    //    if(voucherType ==  salesHelper->VoucherType){
    SalesVoucherEditor *voucherWidget = new SalesVoucherEditor(new salesVoucherDatabaseHelper());
    GeneralVoucherDataObject* voucher = salesHelper->getVoucherByVoucherNoPtr(voucherNo,LoginValues::voucherPrefix);
    voucherWidget->setVoucher(voucher);
    voucherWidget->showVoucher();
    voucherWidget->showMaximized();
    QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTable()));
    //    }
}

void ComplimentarySalesReport::on_itemWiseTableView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();

    QString voucherNo = model->record(row).value(0).toString();
    //    QString voucherType = ledgerListModel->record(row).value(3).toString();
    qDebug()<<"Double clicked";

    //    if(voucherType ==  salesHelper->VoucherType){
    SalesVoucherEditor *voucherWidget = new SalesVoucherEditor(new salesVoucherDatabaseHelper());
    GeneralVoucherDataObject* voucher = salesHelper->getVoucherByVoucherNoPtr(voucherNo,LoginValues::voucherPrefix);
    voucherWidget->setVoucher(voucher);
    voucherWidget->showVoucher();
    voucherWidget->showMaximized();
    QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTable()));
    //    }
}

void ComplimentarySalesReport::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Complimentary Sales Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&ComplimentarySalesReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&ComplimentarySalesReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&ComplimentarySalesReport::on_excelButton_clicked);
}

void ComplimentarySalesReport::printReport(QPrinter *printer)
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

    QList<int> avlCols; avlCols<<0<<1<<2;
    widthList <<60<<20<<20;
    AlignmentList<< 0<< 1 <<1;
    printerHelper->printReportGeneral(ui->amountWiseTableView
                                 ,widthList,"Complimentary Sales Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}
