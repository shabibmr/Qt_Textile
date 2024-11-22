    #include "godownwisestockreport.h"
#include "ui_godownwisestockreport.h"

GodownwiseStockReport::GodownwiseStockReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GodownwiseStockReport)
{
    ui->setupUi(this);

    qDebug()<<"GG";

    ui->fromDateDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateDateEdit->setDate(QDate::currentDate());

    godownHelper = new GodownDatabaseHelper;
    QSqlQueryModel* godownsModel = godownHelper->getGodownForSearchModel();
    QPointer<GMGodownSearchLite> godownSearch = new GMGodownSearchLite(godownsModel);

    QWidget *SearchWid = new QWidget(this);
    QVBoxLayout *lay = new QVBoxLayout(SearchWid);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    lay->addWidget(godownSearch);
    SearchWid->setLayout(lay);
    //    ui->itemTableWidget->setCellWidget(i,1,SearchWid);
    ui->godownLayout->addWidget(SearchWid);
    QObject::connect(godownSearch,SIGNAL(SelectedGodown(GodownDataObject)),
                     this,SLOT(getGodownDetails(GodownDataObject)));

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
//    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&GodownwiseStockReport::showShareOptions);

    ui->horizontalLayout->addWidget(shareButton);
    ui->excelButton->hide();
    ui->printButton->hide();


//    setTable();


}

GodownwiseStockReport::~GodownwiseStockReport()
{
    delete ui;
}

void GodownwiseStockReport::setTable()
{
    QDate fromDate = ui->fromDateDateEdit->date();
//    QDateTime fromDateTime = QDateTime(fromDate);
//    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date();//.addDays(CounterSettingsDataModel::EndDay);
//    QDateTime toDateTime = QDateTime(toDate);
//    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));


//    model = ReportsHelper->getGroupwiseClosingStockByGodown(fromDate, toDate, godownId, "0");

    qDebug()<<"model"<<model->rowCount();

//    sort->setDynamicSortFilter(true);
//    sort->setSourceModel(model);
    ui->tableView->setModel(model);

}


void GodownwiseStockReport::getGodownDetails(GodownDataObject obj)
{
    godownId = obj.GodownID;
    setTable();
}

void GodownwiseStockReport::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Godownwise Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
//    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);
    shareOptionsWidget->excelButton->hide();
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&GodownwiseStockReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&GodownwiseStockReport::printReport);
//    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
//                     this,&GodownwiseStockReport::on_excelButton_clicked);
}

void GodownwiseStockReport::printReport(QPrinter *printer)
{
    printer->setPrinterName("printerName");
    printerHelper = new CounterPrinterHelper;
    printerHelper->setPrinter(printer);

}
