#include "itemtransactionsummaryreportgenerator.h"
#include "ui_itemtransactionsummaryreportgenerator.h"

#include "QFileDialog"
#include "sharetools/excel/excelreporthelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"


ItemTransactionSummaryReportGenerator::ItemTransactionSummaryReportGenerator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemTransactionSummaryReportGenerator)
{
    ui->setupUi(this);
    invHelper = new InventoryHelper();
    ui->fromDateDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateDateEdit->setDate(QDate::currentDate());
}

ItemTransactionSummaryReportGenerator::~ItemTransactionSummaryReportGenerator()
{
    delete ui;
}

void ItemTransactionSummaryReportGenerator::setReportName(const QString &value)
{
    ReportName = value;
    ui->label->setText(ReportName);

}

void ItemTransactionSummaryReportGenerator::setTableView()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    if( voucherType == GMVoucherTypes::SalesVoucher ||
            voucherType == GMVoucherTypes::PurchaseVoucher||
            voucherType == GMVoucherTypes::StockEntry||
            voucherType == GMVoucherTypes::DefectiveVoucher||
            voucherType == GMVoucherTypes::CreditNote||
            voucherType == GMVoucherTypes::DebitNote||
            voucherType == GMVoucherTypes::Proforma)
        model = invHelper->getItemwiseTransactionSummaryByVoucherType(fromDateTime,toDateTime,voucherType);
    else if( voucherType == GMVoucherTypes::ReceiptNote)
        model = invHelper->getItemwiseReceiptNoteModel(fromDateTime,toDateTime);

    ui->tableView->setModel(model);

    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);

    qDebug()<<"Type : "<<voucherType<<((voucherType == GMVoucherTypes::SalesVoucher)?"yes":"no");

    if(voucherType == GMVoucherTypes::SalesVoucher ||
            voucherType == GMVoucherTypes::DebitNote ||
            voucherType == GMVoucherTypes::Proforma ||
            voucherType == GMVoucherTypes::DefectiveVoucher ){
        qDebug()<<"Hide Column 4";
        ui->tableView->hideColumn(4);
        ui->tableView->hideColumn(6);
    }
    else if(     voucherType == GMVoucherTypes::PurchaseVoucher ||
                 voucherType == GMVoucherTypes::CreditNote ||
                 voucherType == GMVoucherTypes::Proforma){
        qDebug()<<"hide column 3" ;
        ui->tableView->hideColumn(3);
        ui->tableView->hideColumn(5);
    }

    ui->tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);

}

void ItemTransactionSummaryReportGenerator::sendEmail()
{

}

void ItemTransactionSummaryReportGenerator::printReport()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<60<<20;
    AlignmentList<< 0<< 1;
    CounterPrinterHelper *printer = new CounterPrinterHelper;
    QList<int> avlCols;
    if(voucherType == GMVoucherTypes::SalesVoucher ||
            voucherType == GMVoucherTypes::DebitNote ||
            voucherType == GMVoucherTypes::Proforma ||
            voucherType == GMVoucherTypes::DefectiveVoucher ){

    }{
        avlCols<<2<<3<<5;
    }
    if(     voucherType == GMVoucherTypes::PurchaseVoucher ||
            voucherType == GMVoucherTypes::CreditNote ||
            voucherType == GMVoucherTypes::Proforma){

    }{
        avlCols<<2<<3<<4;
    }

    if(voucherType == GMVoucherTypes::DeliveryNote || voucherType == GMVoucherTypes::ReceiptNote)
        avlCols<<2<<3<<4;

    widthList<<40<<20<<20;
    printer-> printReportGeneral(ui->tableView
                                 ,widthList,"Service Wise Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}

void ItemTransactionSummaryReportGenerator::exportToExcel()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();
    QString reportName = "Itemwise_Summary_" + GMVoucherTypes::getVoucherString(voucherType);

    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;
    QList<int> avlCols;
    if(voucherType == GMVoucherTypes::SalesVoucher ||
            voucherType == GMVoucherTypes::DebitNote ||
            voucherType == GMVoucherTypes::Proforma ||
            voucherType == GMVoucherTypes::DefectiveVoucher ){
        avlCols<<2<<3<<5;
    }
    if(     voucherType == GMVoucherTypes::PurchaseVoucher ||
            voucherType == GMVoucherTypes::CreditNote ||
            voucherType == GMVoucherTypes::Proforma){
        avlCols<<2<<4<<6;
    }
    if(voucherType == GMVoucherTypes::DeliveryNote || voucherType == GMVoucherTypes::ReceiptNote)
        avlCols<<2<<3<<4;

    ex->GenerateReportFromTableView(fileName,reportName,
                                    widthList,ui->tableView,
                                    fromDate.toString("yyyy-MM-dd"),
                                    toDate.toString("yyyy-MM-dd"),
                                    avlCols);
}

void ItemTransactionSummaryReportGenerator::setVoucherType(int value)
{
    voucherType = value;
}

void ItemTransactionSummaryReportGenerator::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    QString itemId = model->record(row).value(0).toString();

}

void ItemTransactionSummaryReportGenerator::on_excelButton_clicked()
{
    exportToExcel();
}

void ItemTransactionSummaryReportGenerator::on_printButton_clicked()
{
    printReport();
}

void ItemTransactionSummaryReportGenerator::on_OKButton_clicked()
{
    setTableView();
}
