#include "godownwisereport.h"
#include "ui_godownwisereport.h"

#include "datamodels/Counter/countersettingsdatamodel.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "sharetools/excel/excelreporthelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include <QFileDialog>

GodownwiseReport::GodownwiseReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GodownwiseReport)
{
    ui->setupUi(this);
    qDebug()<<"Godownwise report open";

    dbhelper = new GodownDatabaseHelper;
    invhelper = new InventoryHelper;
    godownID = dbhelper->getDefaultGodown();
    gdwid = new GMGodownLineMaterial(dbhelper,dbhelper->getGodownForSearchModel(),&godownID);
    ui->searchLay->addWidget(gdwid);
    connect(gdwid, &GMGodownLineMaterial::selected, this, [=](){
        qDebug()<<" got isgnal";
        setTable();
    });
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());
    ui->tableView->setItemDelegate(new ReportCommonDelegate());

    setTable();

}

GodownwiseReport::~GodownwiseReport()
{
    delete ui;
}

void GodownwiseReport::setTable()
{
    qDebug()<<"godown is "<<godownID;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = invhelper->getGodownwiseSalesModel(fromDate, toDate, godownID);
    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);

    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

}

void GodownwiseReport::on_OKButton_clicked()
{
    setTable();
}

void GodownwiseReport::on_excelButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();
    QString reportName = "Godownwise Sales Report";

    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }

    widthList <<60<<20<<20;
    AlignmentList<< 0<< 1<<1;

    ExcelReportHelper *ex = new ExcelReportHelper;

    QList<int> avlCols; avlCols<<1<<2<<3;
    ex->GenerateReportFromTableView(fileName,reportName,
                                    widthList,ui->tableView,
                                    fromDate.toString("yyyy-MM-dd"),
                                    toDate.toString("yyyy-MM-dd"),
                                    avlCols);
}

void GodownwiseReport::on_printButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<60<<20<<20;
    AlignmentList<< 0<< 1<<1;
    CounterPrinterHelper *printer = new CounterPrinterHelper;
    QList<int> avlCols; avlCols<<1<<2<<3;
    printer-> printReportGeneral(ui->tableView
                                 ,widthList,"GodownWise Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}

void GodownwiseReport::on_backButton_clicked()
{

}
