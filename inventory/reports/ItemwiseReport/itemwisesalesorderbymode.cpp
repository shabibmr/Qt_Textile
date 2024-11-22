#include "itemwisesalesorderbymode.h"
#include "ui_itemwisesalesorderbymode.h"

#include "customwidgets/Delegates/Orders/salesorderstimebasedreportdelegate.h"
#include "counter/PrinterHelper/counterprinterhelper.h"

ItemwiseSalesOrderByMode::ItemwiseSalesOrderByMode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemwiseSalesOrderByMode)
{
    ui->setupUi(this);
    setWindowTitle("Sales Orders Itemwise");
    initDBHelpers();
    initGlobalObjects();
    initWidgets();
    setWidgets();
    refreshMainTable();
}

ItemwiseSalesOrderByMode::~ItemwiseSalesOrderByMode()
{
    delete ui;
}

void ItemwiseSalesOrderByMode::initDBHelpers()
{
    dbHelper = new LedgerHelperMT();
    invHelper = new InventoryHelper();
}

void ItemwiseSalesOrderByMode::initGlobalObjects()
{
    fromDate = QDate::currentDate();
    toDate   = QDate::currentDate();
}

void ItemwiseSalesOrderByMode::initWidgets()
{
    tableView = new QTableView();
    tableView->setItemDelegate(new SalesOrdersTimeBasedReportDelegate());
    tableView->setSelectionBehavior(QTableView::SelectRows);
    //    tableView = new QTableView;

    connect(tableView,&QTableView::doubleClicked,this,[=](const QModelIndex index){
        int row = sort->mapToSource(index).row();
        QString itemID = model->record(row).value(0).toString();
        // Open window to show transaction List
    });

    sort= new GMItemSortFilterProxyModel();

    dateEditFrom = new GMDateEdit(&fromDate);
    dateEditFrom->setParent(this);
    dateEditFrom->setVariable(&fromDate);
    dateEditFrom->setMinimumWidth(140);

    dateEditTo = new GMDateEdit(&toDate);
    dateEditTo->setVariable(&toDate);
    dateEditTo->setMinimumWidth(140);

    searchLine = new LineEditIcon(QIcon(":/icons/search.ico"));
    QObject::connect(searchLine,&QLineEdit::textChanged,
                     [=](QString str){
        QRegExp regExp(str,Qt::CaseInsensitive);
        sort->setFilterRegExp(regExp);
    }
    );

    shareOptionsWidget = new ShareTools("",this);

//    filter = new QComboBox(this);
//    filter->addItem("All");
//    filter->addItem("Lunch");
//    filter->addItem("Dinner");
//    connect(filter,SIGNAL(currentIndexChanged(int)),this,SLOT(filterChanged(int)));
}

void ItemwiseSalesOrderByMode::setWidgets()
{
    QPointer<QWidget> Widget = new QWidget(this);
    QPointer<QWidget> tablesWidget = new QWidget(this);

    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout();
    Widget->setLayout(mainLayout);
    QPointer<QHBoxLayout> filterLay = new QHBoxLayout;
    TableLay = new QVBoxLayout;

    tablesWidget->setLayout(TableLay);
    mainLayout->addLayout(filterLay);
    mainLayout->addWidget(tablesWidget);
    QToolButton *button = new QToolButton(this);
    button->setText("GO");
    button->setIcon(QtMaterialTheme::icon("av", "play_arrow"));
    button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    button->setIconSize(QSize(30,30));

    connect(button,&QToolButton::clicked,this,[=](){
        refreshMainTable();
    });

    QToolButton *printButton = new QToolButton(this);
    printButton->setText("PRINT");
    printButton->setIcon(QIcon(":/images/printer.ico"));
    printButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    printButton->setIconSize(QSize(30,30));

    connect(printButton,&QToolButton::clicked,this,[=](){
        showShareOptions();
    });

    QString styls= "QToolButton {";
    styls += "color: blue;";
    styls += "border-radius: 15px;";
    styls += "}";

    searchLine->hide();
    filterLay->addWidget(new QLabel("From"));
    filterLay->addWidget(dateEditFrom);
    filterLay->setSpacing(7);
    filterLay->addWidget(new QLabel("To"));
    filterLay->addWidget(dateEditTo);
    filterLay->setSpacing(7);
    filterLay->addSpacerItem(new QSpacerItem(20,20));
    filterLay->addWidget(button);
    filterLay->addStretch(1);
    filterLay->addWidget(printButton);
    filterLay->addSpacerItem(new QSpacerItem(20,20));
    TableLay->addWidget(tableView);
    TableLay->addSpacerItem(new QSpacerItem(20,20));

    stack = new QStackedWidget(this);
    tabBar = new QtMaterialTabs(this);

    ui->verticalLayout_2->addWidget(tabBar);
    ui->verticalLayout_2->addWidget(stack);

    stack->addWidget(Widget);

}

void ItemwiseSalesOrderByMode::refreshMainTable()
{

    model = invHelper->getSalesOrderByTime(fromDate,toDate);

    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);
    sort->sort(1);
    tableView->setModel(sort);
    tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tableView->hideColumn(0);
}

void ItemwiseSalesOrderByMode::setSecondTable()
{

}

void ItemwiseSalesOrderByMode::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Itemwise Sales Report", this);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
    //    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&ItemwiseSalesOrderByMode::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&ItemwiseSalesOrderByMode::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&ItemwiseSalesOrderByMode::on_excelButton_clicked);
}

void ItemwiseSalesOrderByMode::on_excelButton_clicked()
{

    QDate fDate = fromDate;
    QDate tDate = toDate;
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString wwaitername ="";

    QString reportName = "Itemwise Sales Report";

    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }

    ExcelReportHelper *ex = new ExcelReportHelper;

    QStringList cols;
    cols<<"40"<<"20"<<"20";
    reportName = "Itemwise Sales Report "+wwaitername;

    QList<int> avlCols;
    avlCols<<1<<2<<3;
    ex->GenericReportFromTableView(fileName,reportName,cols,tableView,fromDate,toDate,avlCols);
}

void ItemwiseSalesOrderByMode::printReport(QPrinter *printer)
{
    CounterPrinterHelper printerHelper;
    printerHelper.setPrinter(printer);
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QString total ;
    QDate fromDate =this->fromDate;
    QDate toDate = this->toDate;
    //    extraBottom<< "Total: " <<total;
    QList<int> avlCols;
    avlCols <<1<<2<<3;
    widthList <<20<<40<<40;
    AlignmentList << 0<<0<< 0;

    printerHelper.printReportGeneralwithModel(model, widthList,"Sales Order List",fromDate,toDate,AlignmentList
                                              ,extraTop,extraBottom,avlCols);
}

void ItemwiseSalesOrderByMode::filterChanged(int index)
{

}
