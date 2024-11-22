#include "productionratereport.h"
#include "ui_productionratereport.h"

#include <QFileDialog>
#include "sharetools/excel/excelreporthelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"

ProductionRateReport::ProductionRateReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductionRateReport)
{
    ui->setupUi(this);

    ui->fromDateDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateDateEdit->setDate(QDate::currentDate());
    inventoryReportsHelper = new InventoryHelper;

    model = new QSqlQueryModel;
    sort = new GMItemSortFilterProxyModel();

//    ui->excelButton->hide();
//    ui->printButton->hide();


setTable();

}

ProductionRateReport::~ProductionRateReport()
{
    delete ui;
}

void ProductionRateReport::setTable()
{
    QDate dateTo = ui->toDateDateEdit->date();
    QDate dateFrom = ui->fromDateDateEdit->date();
    model = inventoryReportsHelper->getProductionRateModel(dateFrom, dateTo);

    // remove

//    QStringList vlist;
//    vlist.append("26604");
//    vlist.append("26605");
//    vlist.append("26606");
//    vlist.append("26607");
//    vlist.append("26608");

//    model = inventoryReportsHelper->getIngredientsByWorkOrders(vlist);

    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);
    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(1,Qt::AscendingOrder);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);

    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView->reset();

}

void ProductionRateReport::on_OKButton_clicked()
{
    setTable();
}

void ProductionRateReport::on_excelButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();
    QString reportName = "Production Rate Report";

    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    widthList <<60<<20<<20;
    AlignmentList<< 0<< 1<<1;

    ExcelReportHelper *ex = new ExcelReportHelper;

    QList<int> avlCols; avlCols<<2<<3<<4;
    ex->GenerateReportFromTableView(fileName,reportName,
                                    widthList,ui->tableView,
                                    fromDate.toString("yyyy-MM-dd"),
                                    toDate.toString("yyyy-MM-dd"),
                                    avlCols);

}

void ProductionRateReport::on_printButton_clicked()
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
    QList<int> avlCols; avlCols<<2<<3<<4;
    printer-> printReportGeneral(ui->tableView
                                 ,widthList,"Production Rate Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);

}

void ProductionRateReport::on_backButton_clicked()
{

}
