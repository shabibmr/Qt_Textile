#include "godownwiseitemmovement.h"
#include "ui_godownwiseitemmovement.h"


GodownwiseItemMovement::GodownwiseItemMovement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GodownwiseItemMovement)
{
    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());
    inHelper = new InventoryHelper;

    tableView = new ReportsTableView("");
    tableView->setItemDelegate(new ReportCommonDelegate());
    ui->tableLayout->addWidget(tableView);
    connect(tableView, &ReportsTableView::doubleClicked, this,
            &GodownwiseItemMovement::on_tableView_doubleClicked);

    invGrpHelper = new SalesInventoryGroupsDatabaseHelper;
    QSqlQueryModel *model = invGrpHelper->getSalesInventoryGroupDataAsModel();
    groupSearch = new GMItemSearchLite(model);
    groupSearch->setGroupFlag(true);
    groupSearch->setText("");
    QWidget *SearchWid = new QWidget(this);
    QVBoxLayout *lay = new QVBoxLayout(SearchWid);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    lay->addWidget(groupSearch);
    SearchWid->setLayout(lay);
    ui->groupLay->addWidget(SearchWid);
    QObject::connect(groupSearch,SIGNAL(SelectedGroup(QString)),
                     this,SLOT(getItemDetails(QString)));
    QObject::connect(groupSearch,SIGNAL(clearItem()),
                     this,SLOT(resetID()));

    GodownDatabaseHelper *godownHelper = new GodownDatabaseHelper;
    QSqlQueryModel* godownsModel = godownHelper->getGodownForSearchModel();
    QPointer<GMGodownSearchLite> godownSearch = new GMGodownSearchLite(godownsModel);

    QWidget *SearchWid2 = new QWidget(this);
    QVBoxLayout *lay2 = new QVBoxLayout(SearchWid2);
    lay2->setContentsMargins(0,0,0,0);
    lay2->setSpacing(0);
    lay2->addWidget(godownSearch);
    SearchWid2->setLayout(lay2);
    //    ui->itemTableWidget->setCellWidget(i,1,SearchWid);
    ui->godownLay->addWidget(SearchWid2);
    QObject::connect(godownSearch,SIGNAL(SelectedGodown(GodownDataObject)),
                     this,SLOT(getGodownDetails(GodownDataObject)));
    tableView->setSortingEnabled(true);


    itemWise = new QtMaterialIconButton(QtMaterialTheme::icon("action", "list"));
//    itemWise->setColor(QColor(255,250,0));
    itemWise->setIconSize(QSize(30,30));
    itemWise->show();

    grouped = new QtMaterialIconButton(QtMaterialTheme::icon("social", "group"));
//    grouped->setColor(QColor(255,250,0));
    grouped->setIconSize(QSize(30,30));
    grouped->show();

    ui->horizontalLayout->addWidget(itemWise);
    ui->horizontalLayout->addWidget(grouped);


    selectedGroups<<"0";

    QObject::connect(itemWise,&QtMaterialIconButton::clicked,
                     this,&GodownwiseItemMovement::setItemwiseReport);

    QObject::connect(grouped,&QtMaterialIconButton::clicked,
                     this,&GodownwiseItemMovement::setGroupedReport);

    setGroupedReport();

}

GodownwiseItemMovement::~GodownwiseItemMovement()
{
    delete ui;
}

void GodownwiseItemMovement::getGodownDetails(GodownDataObject obj)
{
    qDebug()<<" changing godown ";
    godownID = obj.GodownID;

    setTable();
}

void GodownwiseItemMovement::getItemDetails(QString item)
{
    qDebug()<<"Group Setting";

    groupID=item;

    qDebug()<<"Group Set to "<< groupID;
    setTable();
}

void GodownwiseItemMovement::setTable()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    if(groupID.length() == 0)
        groupID ="0";

    if(reportType == "Group")
    {
        model = inHelper->getGroupwiseMovementByGodown(groupID, godownID,fromDate,toDate);
    }
    else {
        model = inHelper->getItemwiseMovementByGodownAndGroup(groupID, godownID,fromDate,toDate);
    }
    tableView->setAvlcols(QList<int> {3,4,5,6});
    tableView->setModel(model);
    tableView->setModelQry(model);
    tableView->setSortingEnabled(true);
    tableView->setColumns();
    tableView->hideColumn(0);
    tableView->hideColumn(1);
    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);


}

void GodownwiseItemMovement::on_tableView_doubleClicked(const QModelIndex &index)
{
    qDebug()<<Q_FUNC_INFO;
    //    int row = sort->mapToSource(index).row();
    int row = index.row();
    QString ID = model->record(row).value(1).toString();
    QString rowType = model->record(row).value(0).toString();
    qDebug()<<"new Group : "<<ID;

    if(rowType=="G"){
        groupID = ID;
        groupSearch->setText(invGrpHelper->getGroupNameByID(ID));
        setTable();
    }
}

void GodownwiseItemMovement::setLedger(LedgerMasterDataModel ledger){
    qDebug()<<"Setting Ledger Value";
    this->godownID = ledger.LedgerID;
    setTable();
}

void GodownwiseItemMovement::setGroupedReport()
{
    reportType ="Group";
    itemWise->setColor(QColor("black"));
    grouped->setColor("#1976d2");
    setTable();

}

void GodownwiseItemMovement::setItemwiseReport()
{
    reportType ="Item";
    grouped->setColor(QColor("black"));
    itemWise->setColor("#1976d2");
    setTable();
}



void GodownwiseItemMovement::on_OKButton_clicked()
{
    setTable();
}

void GodownwiseItemMovement::on_printButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<20<<20<<20<<20<<20;
    AlignmentList<< 0<< 1 <<1<<1<<1;
    CounterPrinterHelper *printer = new CounterPrinterHelper;
    printer->setPrinter(new QPrinter(QPrinter::ScreenResolution));
    QList<int> avlCols; avlCols<<2<<3<<4<<5<<6;



    printer-> printReportGeneral(tableView
                                 ,widthList,"Godownwise Item Movement Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}

void GodownwiseItemMovement::on_excelButton_clicked()
{
    QDate fDate = ui->fromDateDateEdit->date();
    QDate tDate = ui->toDateDateEdit->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString wwaitername ="";

    QString reportName = "Godownwise Item Movement Report";

    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QStringList cols; cols<<"20"<<"20"<<"20"<<"20"<<"20";
    reportName = "Godownwise Sales Report "+wwaitername;
    QList<int> avlCols; avlCols<<2<<3<<4<<5<<6;
    ex->GenericReportFromTableView(fileName,reportName,cols,tableView,fromDate,toDate,avlCols);
}
