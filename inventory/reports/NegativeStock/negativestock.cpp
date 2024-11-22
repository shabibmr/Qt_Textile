#include "negativestock.h"
#include "ui_negativestock.h"

NegativeStock::NegativeStock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NegativeStock)
{
    ui->setupUi(this);
    ui->fromDateLabel->hide();
    ui->fromDateDateEdit->hide();
    ui->tableView->setItemDelegate(new ReportCommonDelegate());
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());
    inHelper = new InventoryHelper;
    ui->tableView->setSortingEnabled(true);

    setTable();

}

NegativeStock::~NegativeStock()
{
    delete ui;
}

void NegativeStock::setTable()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));
    model = inHelper->getNegativeStockList(toDateTime);
    ui->tableView->setModel(model);
    ui->tableView->setSortingEnabled(true);

    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

}

void NegativeStock::on_OKButton_clicked()
{
    setTable();
}

void NegativeStock::on_excelButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();

    QString reportName = "Negative Stock Report";

    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    widthList <<60<<20;
    AlignmentList<< 0<<1;
    ExcelReportHelper *ex = new ExcelReportHelper;
    QList<int> avlCols; avlCols<<0<<1;
    ex->GenerateReportFromTableView(fileName,reportName,
                                    widthList,ui->tableView,
                                    fromDate.toString("yyyy-MM-dd"),
                                    toDate.toString("yyyy-MM-dd"),
                                    avlCols);
}

void NegativeStock::on_printButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<60<<20;
    AlignmentList<< 0<<1;
    CounterPrinterHelper *printer = new CounterPrinterHelper;
    QList<int> avlCols; avlCols<<0<<1;
    printer-> printReportGeneral(ui->tableView
                                 ,widthList,"Negative Stock Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}

void NegativeStock::on_backButton_clicked()
{

}
