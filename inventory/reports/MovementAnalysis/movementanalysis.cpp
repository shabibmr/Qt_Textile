#include "movementanalysis.h"
#include "ui_movementanalysis.h"

MovementAnalysis::MovementAnalysis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MovementAnalysis)
{
    ui->setupUi(this);
//    ui->tableView->setItemDelegate(new ReportCommonDelegate());
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());
    inHelper = new InventoryHelper;
    itemHelper = new SalesInventoryItemDatabaseHelper();
    ui->tableView->setSortingEnabled(true);

    itemsModel = itemHelper->getInventoryItemsQueryModelForSearch();

    QPointer<GMItemSearchLite> itemLineLite = new GMItemSearchLite(itemsModel);
    QWidget *SearchWid = new QWidget(this);
    QVBoxLayout *lay = new QVBoxLayout(SearchWid);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    lay->addWidget(itemLineLite);
    SearchWid->setLayout(lay);
    ui->groupLay->addWidget(SearchWid);

    QObject::connect(itemLineLite,SIGNAL(SelectedItemID(inventoryItemDataModel)),
                     this,SLOT(getItemDetails(inventoryItemDataModel)));

    setTable();

}

MovementAnalysis::~MovementAnalysis()
{
    delete ui;
}

void MovementAnalysis::setTable()
{
    if(itemID.length() == 0){
        return;
    }
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));
    model = inHelper->getItemTransaction(itemID,fromDateTime,toDateTime);
    ui->tableView->setModel(model);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->hideColumn(3);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MovementAnalysis::getItemDetails(inventoryItemDataModel item){
    this->itemID = item.ItemID;
    setTable();
}

void MovementAnalysis::on_OKButton_2_clicked()
{
    setTable();
}

void MovementAnalysis::on_excelButton_2_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();
    QString reportName = "Movement Analysis";

    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    widthList <<60<<20<<60<<20<<20;
    AlignmentList<< 0<<0<<0<<1<<1;
    ExcelReportHelper *ex = new ExcelReportHelper;
    QList<int> avlCols; avlCols<<0<<1<<2<<4<<5;
    ex->GenerateReportFromTableView(fileName,reportName,
                                    widthList,ui->tableView,
                                    fromDate.toString("yyyy-MM-dd"),
                                    toDate.toString("yyyy-MM-dd"),
                                    avlCols);
}

void MovementAnalysis::on_printButton_2_clicked()
{
    qDebug()<<"print";
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<60<<20<<60<<20<<20;
    AlignmentList<< 0<<0<<0<<1<<1;
    CounterPrinterHelper *printer = new CounterPrinterHelper;
    QList<int> avlCols; avlCols<<0<<1<<2<<4<<5;
    printer-> printReportGeneral(ui->tableView
                                 ,widthList,"Movement Analysis",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}

void MovementAnalysis::on_backButton_2_clicked()
{

}
