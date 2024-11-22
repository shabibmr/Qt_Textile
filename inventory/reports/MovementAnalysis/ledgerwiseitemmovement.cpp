#include "ledgerwiseitemmovement.h"
#include "ui_ledgerwiseitemmovement.h"


LedgerwiseItemMovement::LedgerwiseItemMovement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LedgerwiseItemMovement)
{
    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());
    inHelper = new InventoryHelper;
    ledHelper = new LedgerMasterDatabaseHelper();
    tableView = new ReportsTableView("");
    tableView->setItemDelegate(new ReportCommonDelegate());
    ui->tableLayout->addWidget(tableView);
    connect(tableView, &ReportsTableView::doubleClicked, this,
            &LedgerwiseItemMovement::on_tableView_doubleClicked);

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

    ledgerModel = ledHelper->getLedgerQueryString();
    GMLedgerSearchLite* ledgerLineLite = new GMLedgerSearchLite(ledgerModel);
    QToolButton* butt = new QToolButton;
    QVBoxLayout* lay2 = new QVBoxLayout;
    lay2->setContentsMargins(0,0,0,0);
    butt->setContentsMargins(0,0,0,0);
    ui->ledgerLay->setContentsMargins(0,0,0,0);
    lay2->addWidget(ledgerLineLite);
    butt->setLayout(lay2);
    butt->setMinimumWidth(250);
    ui->ledgerLay->addWidget(butt);

    QObject::connect(ledgerLineLite,SIGNAL(selectedLedger(LedgerMasterDataModel)),this,SLOT(setLedger(LedgerMasterDataModel)));



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
                     this,&LedgerwiseItemMovement::setItemwiseReport);

    QObject::connect(grouped,&QtMaterialIconButton::clicked,
                     this,&LedgerwiseItemMovement::setGroupedReport);

    setGroupedReport();

}

LedgerwiseItemMovement::~LedgerwiseItemMovement()
{
    delete ui;
}

void LedgerwiseItemMovement::setTable()
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
        model = inHelper->getGroupwiseMovementByLedger(groupID, ledgerID,fromDate,toDate);
    }
    else {
       model = inHelper->getItemwiseMovementByLedgerAndItemGroup(groupID, ledgerID,fromDate,toDate);
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

void LedgerwiseItemMovement::setLedger(LedgerMasterDataModel ledger){
    qDebug()<<"Setting Ledger Value";
    this->ledgerID = ledger.LedgerID;
    setTable();
}

void LedgerwiseItemMovement::setGroupedReport()
{

    reportType ="Group";
    itemWise->setColor(QColor("black"));
    grouped->setColor("#1976d2");
    setTable();

}

void LedgerwiseItemMovement::setItemwiseReport()
{
    reportType ="Item";
    grouped->setColor(QColor("black"));
    itemWise->setColor("#1976d2");
    setTable();

}



void LedgerwiseItemMovement::on_OKButton_clicked()
{
    setTable();
}

void LedgerwiseItemMovement::on_printButton_clicked()
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
                                              ,widthList,"Ledgerwise Item Movement Report",fromDate,toDate,AlignmentList
                                              ,extraTop,extraBottom,avlCols);
}

void LedgerwiseItemMovement::on_excelButton_clicked()
{
    QDate fDate = ui->fromDateDateEdit->date();
    QDate tDate = ui->toDateDateEdit->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString wwaitername ="";

    QString reportName = "Ledgerwise Item Movement Report";

    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QStringList cols; cols<<"20"<<"20"<<"20"<<"20"<<"20";
    reportName = "Itemwise Sales Report "+wwaitername;
    QList<int> avlCols; avlCols<<2<<3<<4<<5<<6;
    ex->GenericReportFromTableView(fileName,reportName,cols,tableView,fromDate,toDate,avlCols);
}

void LedgerwiseItemMovement::on_tableView_doubleClicked(const QModelIndex &index)
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


void LedgerwiseItemMovement::getItemDetails(QString item)
{
    qDebug()<<"Group Setting";

    groupID=item;

    qDebug()<<"Group Set to "<< groupID;
    setTable();
}
