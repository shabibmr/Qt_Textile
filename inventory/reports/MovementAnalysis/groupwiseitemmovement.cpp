#include "groupwiseitemmovement.h"
#include "ui_groupwiseitemmovement.h"

GroupwiseItemMovement::GroupwiseItemMovement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupwiseItemMovement)
{
    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());

    tableView = new ReportsTableView("");
    tableView->setItemDelegate(new ReportCommonDelegate());
    ui->tableLayout->addWidget(tableView);
    connect(tableView, &ReportsTableView::doubleClicked, this,
            &GroupwiseItemMovement::on_tableView_doubleClicked);
    inHelper = new InventoryHelper;
//    ui->tableView->setSortingEnabled(true);
    accHelper = new AccountGroupMasterDatabaseHelper();
    groupsModel = accHelper->getAllAccountForSearchModel();

    GMLedgerSearchLite* groupLineLite = new GMLedgerSearchLite(groupsModel);
    groupLineLite->setAccountsFlag(true);
    QToolButton* butt = new QToolButton;
    QVBoxLayout* lay = new QVBoxLayout;
    lay->setContentsMargins(0,0,0,0);
    butt->setContentsMargins(0,0,0,0);
    ui->groupLay->setContentsMargins(0,0,0,0);
    lay->addWidget(groupLineLite);
    butt->setLayout(lay);
    butt->setMinimumWidth(250);
    ui->groupLay->addWidget(butt);

    QObject::connect(groupLineLite,SIGNAL(SelectedAccount(AccountGroupDataModel)),this,SLOT(setAccountDetails(AccountGroupDataModel)));


    invGrpHelper = new SalesInventoryGroupsDatabaseHelper;
    QSqlQueryModel *model = invGrpHelper->getSalesInventoryGroupDataAsModel();
    groupSearch = new GMItemSearchLite(model);
    groupSearch->setGroupFlag(true);
    groupSearch->setText("");
    QWidget *SearchWid = new QWidget(this);
    QVBoxLayout *lay2 = new QVBoxLayout(SearchWid);
    lay2->setContentsMargins(0,0,0,0);
    lay2->setSpacing(0);
    lay2->addWidget(groupSearch);
    SearchWid->setLayout(lay2);
    ui->invGroupLay->addWidget(SearchWid);
    QObject::connect(groupSearch,SIGNAL(SelectedGroup(QString)),
                     this,SLOT(getItemDetails(QString)));
    QObject::connect(groupSearch,SIGNAL(clearItem()),
                     this,SLOT(resetID()));

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);

    //    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QtMaterialIconButton::clicked,
                     this,&GroupwiseItemMovement::showShareOptions);

    ui->horizontalLayout->addWidget(shareButton);

    ui->excelButton->hide();
    ui->printButton->hide();


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
                     this,&GroupwiseItemMovement::setItemwiseReport);

    QObject::connect(grouped,&QtMaterialIconButton::clicked,
                     this,&GroupwiseItemMovement::setGroupedReport);

    setGroupedReport();


}

GroupwiseItemMovement::~GroupwiseItemMovement()
{
    delete ui;
}

void GroupwiseItemMovement::setTable()
{
    if(accGroupID.length()==0){
        return;
    }
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
        model = inHelper->getAccGroupwiseItemMovement(groupID, accGroupID,fromDateTime,toDateTime);
    }
    else {
        model = inHelper->getDetailedAccGroupwiseItemMovement(groupID, accGroupID,fromDateTime,toDateTime);
    }

    tableView->setAvlcols(QList<int> {3,4,5,6});
    tableView->setModel(model);
    tableView->setModelQry(model);
    tableView->setSortingEnabled(true);
    tableView->setColumns();
    tableView->hideColumn(0);
    tableView->hideColumn(1);


    tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
}

void GroupwiseItemMovement::setAccountDetails(AccountGroupDataModel acc){
    this->accGroupID = acc.groupID;
    setTable();
}

void GroupwiseItemMovement::setGroupedReport()
{
    reportType ="Group";
    itemWise->setColor(QColor("black"));
    grouped->setColor("#1976d2");
    setTable();

}

void GroupwiseItemMovement::setItemwiseReport()
{
    reportType ="Item";
    grouped->setColor(QColor("black"));
    itemWise->setColor("#1976d2");
    setTable();
}

void GroupwiseItemMovement::showShareOptions()
{
    ShareTools *shareOptionsWidget = new ShareTools("Ledger Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);
    //    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&GroupwiseItemMovement::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&GroupwiseItemMovement::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&GroupwiseItemMovement::on_excelButton_clicked);
}

void GroupwiseItemMovement::getItemDetails(QString item)
{
    qDebug()<<"Group Setting";

    groupID=item;

    qDebug()<<"Group Set to "<< groupID;
    setTable();
}

void GroupwiseItemMovement::on_OKButton_clicked()
{
    setTable();
}

void GroupwiseItemMovement::on_excelButton_clicked()
{
    QDate fDate = ui->fromDateDateEdit->date();
    QDate tDate = ui->toDateDateEdit->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString wwaitername ="";

    QString reportName = "Account Groupwise Item Movement Report";

    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QStringList cols; cols<<"20"<<"20"<<"20"<<"20"<<"20";
    QList<int> avlCols; avlCols<<2<<3<<4<<5<<6;
    ex->GenericReportFromTableView(fileName,reportName,cols,tableView,fromDate,toDate,avlCols);
}

void GroupwiseItemMovement::printReport(QPrinter *printer)
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<20<<20<<20<<20<<20;
    AlignmentList<< 0<< 1 <<1<<1<<1;
    CounterPrinterHelper *printerHelper = new CounterPrinterHelper;
    printerHelper->setPrinter(printer);
    QList<int> avlCols; avlCols<<2<<3<<4<<5<<6;


    printerHelper-> printReportGeneral(tableView
                                              ,widthList,"Account Groupwise Item Movement Report",fromDate,toDate,AlignmentList
                                              ,extraTop,extraBottom,avlCols);
}
void GroupwiseItemMovement::on_tableView_doubleClicked(const QModelIndex &index)
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
