#include "itemwisereport.h"
#include "ui_itemwisereport.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "customwidgets/Charts/SinglesSet/singlesetbarcharthelper.h"
ItemwiseReport::ItemwiseReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemwiseReport)
{
    ui->setupUi(this);

    salesHelper = new salesVoucherDatabaseHelper();

    ui->tableView->setItemDelegate(new ReportCommonDelegate());
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());
    inHelper = new InventoryHelper;
    ui->totalLineEdit->setReadOnly(true);
    ui->totalLineEdit->setAlignment(Qt::AlignRight);
    ui->tableView->setSortingEnabled(true);

    setTable();
    UserProfileDatabaseHelper userHelper;
    employees = userHelper.getAllUsers();

    if(LoginValues::Privilege<=4){
       ui->totalLineEdit->hide();
       ui->label_3->hide();
    }

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
//    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&ItemwiseReport::showShareOptions);

    showChartButton  = new QtMaterialIconButton(QtMaterialTheme::icon("editor", "insert_chart"));
    showChartButton->setParent(this);
//    shareButton->setColor(QColor(255,250,0));
    showChartButton->setIconSize(QSize(30,30));
    QObject::connect(showChartButton,&QPushButton::clicked,
                     this,&ItemwiseReport::showChart);

    ui->horizontalLayout->addWidget(shareButton);
    ui->horizontalLayout->addWidget(showChartButton);
    ui->excelButton->hide();
    ui->printButton->hide();

//    ui->salesmanCombo->addItem(" ",0);
//    QMapIterator<int, QString> i(employees);
//    while (i.hasNext()) {
//        i.next();
//        ui->salesmanCombo->addItem(i.value(),i.key());
//    }
//    setSalesmanTable();
}

ItemwiseReport::~ItemwiseReport()
{
    delete ui;
}



void ItemwiseReport::setTable()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));


    model = inHelper->getItemWiseSalesByDateModel(fromDateTime,toDateTime);
//    model = inHelper->getItemWiseSalesWithGroupByDateModel(fromDateTime, toDateTime);
    sort = new GMItemSortFilterProxyModel(this);
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);

    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);
    QSqlQueryModel *mm =  inHelper->getItemWiseSalesWithGroupByDateModel(fromDateTime,toDateTime);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    qDebug()<<"Row alpha count = "<<model->rowCount();
//    ui->tableView->setModel(model);

    float total = 0;
    for(int i=0;i<model->rowCount();i++){
        total += model->record(i).value(2).toFloat();
    }

    ui->totalLineEdit->setText(QString::number(total,'f',2));

//    if(model->rowCount()==0)
        return;




}

//void ItemwiseReport::setSalesmanTable()
//{
//    QDate fromDate = ui->fromDateDateEdit->date();
//    QDateTime fromDateTime = QDateTime(fromDate);
//    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

//    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
//    QDateTime toDateTime = QDateTime(toDate);
//    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));
//    int salesmanID=0;

//    salesmanID = ui->salesmanCombo->currentData().toInt();
//    waiterModel = inHelper->getItemWiseWaiterWise(fromDateTime,toDateTime,salesmanID);

//    ui->salesmanTable->setModel(waiterModel);

//    ui->salesmanTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

//}

void ItemwiseReport::on_OKButton_clicked()
{
    setTable();
}

void ItemwiseReport::on_excelButton_clicked()
{

    QDate fDate = ui->fromDateDateEdit->date();
    QDate tDate = ui->toDateDateEdit->date();
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

    QStringList cols; cols<<"40"<<"20"<<"20";
    reportName = "Itemwise Sales Report "+wwaitername;
    QList<int> avlCols; avlCols<<0<<1<<2;
    ex->GenericReportFromTableView(fileName,reportName,cols,ui->tableView,fromDate,toDate,avlCols);
}

void ItemwiseReport::on_printButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<60<<20<<20;
    AlignmentList<< 0<< 1 <<1;
    CounterPrinterHelper *printer = new CounterPrinterHelper;
    QList<int> avlCols; avlCols<<0<<1<<2;

    printer-> printReportGeneral(ui->tableView
                                 ,widthList,"Item Sales Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}

void ItemwiseReport::on_backButton_clicked()
{
    this->close();
}

void ItemwiseReport::on_salesmanCombo_currentIndexChanged(int index)
{
    //setSalesmanTable();
}

//void ItemwiseReport::on_excelWaiter_clicked()
//{
//    qDebug()<<"create Excel";

//    QDate fDate = ui->fromDateDateEdit->date();
//    QDate tDate = ui->toDateDateEdit->date();
//    QString fromDate = fDate.toString("yyyy-MM-dd");
//    QString toDate = tDate.toString("yyyy-MM-dd");
//    QString wwaitername =ui->salesmanCombo->currentText();

//    QString reportName = "Itemwise Sales Report"+wwaitername;

//    QDate curDate = QDate::currentDate();
//    QString dateNow = curDate.toString("dd_MMM_yyyy");
//    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
//    if (fileName.isEmpty())
//    {
//        return;
//    }


//    ExcelReportHelper *ex = new ExcelReportHelper;

//    QStringList cols; cols<<"40"<<"20"<<"20";
//    reportName = "Itemwise Sales Report "+wwaitername;
//    ex->GenericReportFromTableView(fileName,reportName,cols,ui->tableView,fromDate,toDate);
//}

void ItemwiseReport::on_ExtractButton_clicked()
{


}

void ItemwiseReport::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Itemwise Sales Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
//    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&ItemwiseReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&ItemwiseReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&ItemwiseReport::on_excelButton_clicked);
}

void ItemwiseReport::showChart()
{
    QDate fromDate = ui->fromDateDateEdit->date();

    QDate toDate = ui->toDateDateEdit->date();

    SingleSetBarchartHelper *chart = new SingleSetBarchartHelper(
                fromDate,toDate,model,0,2,"Item wise");

    QWidget *wid = new QWidget(this);
    QLayout *lay = new QVBoxLayout(wid);

    lay->addWidget(chart->setBarChart());
    wid->setWindowTitle("Itemwise Sales");
    wid->resize(800,500);
    wid->setWindowFlags(Qt::Window);
    wid->setAttribute(Qt::WA_DeleteOnClose);
    wid->show();
}

void ItemwiseReport::printReport(QPrinter *printer)
{
    printer->setPrinterName("printerName");
    printerHelper = new CounterPrinterHelper;
    printerHelper->setPrinter(printer);

    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<60<<20<<20;
    AlignmentList<< 0<< 1 <<1;
    QList<int> avlCols; avlCols<<0<<1<<2;

    printerHelper-> printReportGeneral(ui->tableView
                                 ,widthList,"Item Sales Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}
