#include "groupsummary.h"
#include "ui_groupsummary.h"

#include "datamodels/Counter/countersettingsdatamodel.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "sharetools/excel/excelreporthelper.h"

GroupSummary::GroupSummary(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupSummary)
{

//    qDebug()<<"&&&&&&&&&&Group summary";
    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());

    accHelper = new AccountGroupMasterDatabaseHelper();
    groupsModel = accHelper->getAllAccountForSearchModel();

//    ui->tableView->setItemDelegate(new ReportCommonDelegate());

//    qDebug()<<"23"<<groupsModel->rowCount();
    GMLedgerSearchLite *accSearch = new GMLedgerSearchLite(groupsModel);
    qDebug()<<"24";
    accSearch->setText("");
    accSearch->setPlaceholderText("Search Account Group...");
    accSearch->setAccountsFlag(true);
    QWidget *SearchWid = new QWidget(this);
    QVBoxLayout *lay = new QVBoxLayout(SearchWid);
    lay->addWidget(accSearch);
    SearchWid->setLayout(lay);

    SearchWid->setFixedWidth(200);
    ui->groupLay->addWidget(SearchWid);

    QObject::connect(accSearch,SIGNAL(SelectedAccount(AccountGroupDataModel)),this,SLOT(setAccountDetails(AccountGroupDataModel)));

    tableView = new ReportsTableView();
    tableView->setItemDelegate(new ReportCommonDelegate);
    ui->tableLayout->addWidget(tableView);

    setTableView();
}

GroupSummary::~GroupSummary()
{
    delete ui;
}

void GroupSummary::on_pushButton_clicked()
{
    setTableView();
}

void GroupSummary::setTableView()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = ledHelper->getAccGroupSummaryModel(group.groupID, fromDateTime, toDateTime); //ledgers listed
    tableView->setModel(model);

    QList<int> totalCols ;
    totalCols<<1<<2;
    tableView->setAvlcols(totalCols);
    tableView->setModel(model);
    tableView->setModelQry(model);

    tableView->setColumns();
    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);



}

void GroupSummary::setAccountDetails(AccountGroupDataModel obj)
{
    group = obj;
    qDebug()<<group.groupID;
    setTableView();
}

void GroupSummary::on_OKButton_clicked()
{
    setTableView();
}

void GroupSummary::on_excelButton_clicked()
{
    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\GroupSummary_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }

    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<60<<20;
    AlignmentList<< 0<< 1<<1;
    QList<int> avlCols; avlCols<<0<<1<<2;

    ExcelReportHelper *ex = new ExcelReportHelper;

    ex->GenerateReportFromTableView(fileName,"Group Summary",
                                    widthList,tableView,
                                    fromDate.toString("yyyy-MM-dd"),
                                    toDate.toString("yyyy-MM-dd"),
                                    avlCols);
}

void GroupSummary::on_printButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<60<<20;
    AlignmentList<< 0<< 1<<1;
    CounterPrinterHelper *printer = new CounterPrinterHelper;
    QList<int> avlCols; avlCols<<0<<1<<2;
    printer-> printReportGeneral(tableView
                                 ,widthList,"Group Summary Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}
