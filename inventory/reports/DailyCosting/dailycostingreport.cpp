#include "dailycostingreport.h"
#include "ui_dailycostingreport.h"

DailyCostingReport::DailyCostingReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DailyCostingReport)
{
    ui->setupUi(this);
//    ui->fromDateLabel->hide();
//    ui->fromDateDateEdit->hide();
    ui->tableView->setItemDelegate(new ReportCommonDelegate());
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());
    inHelper = new InventoryHelper;
    ui->tableView->setSortingEnabled(true);

    setTable();

}

DailyCostingReport::~DailyCostingReport()
{
    delete ui;
}

void DailyCostingReport::setTable()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));
    model = inHelper->getItemwiseDatewisePurchasePriceModel(fromDateTime, toDateTime);
    ui->tableView->setModel(model);
    ui->tableView->setSortingEnabled(true);

    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

}

void DailyCostingReport::on_OKButton_clicked()
{
    setTable();
}

void DailyCostingReport::on_excelButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();

    QString reportName = "Daily Costing Report";

    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    widthList <<60;

    AlignmentList<< 0;
    QList<int> avlCols; avlCols<<0;
    for(int i=1; i<model->columnCount(); i++){
        widthList<<20;
        AlignmentList<<1;
        avlCols<<i;

    }
    ExcelReportHelper *ex = new ExcelReportHelper;
    ex->GenerateReportFromTableView(fileName,reportName,
                                    widthList,ui->tableView,
                                    fromDate.toString("yyyy-MM-dd"),
                                    toDate.toString("yyyy-MM-dd"),
                                    avlCols);
}

void DailyCostingReport::on_printButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();

    widthList <<60;

    AlignmentList<< 0;
    for(int i=1; i<model->columnCount(); i++){
        widthList<<20;
        AlignmentList<<1;

    }
    CounterPrinterHelper *printer = new CounterPrinterHelper;
    QList<int> avlCols; avlCols<<0<<1;
    printer-> printReportGeneral(ui->tableView
                                 ,widthList,"Daily Costing Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}

void DailyCostingReport::on_backButton_clicked()
{

}
