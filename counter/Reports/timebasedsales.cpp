#include "timebasedsales.h"
#include "ui_timebasedsales.h"
#include "customwidgets/Delegates/reportcommondelegate.h"


TimeBasedSales::TimeBasedSales(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeBasedSales)
{
    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());
    ledHelper = new LedgerHelper;

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
//    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&TimeBasedSales::showShareOptions);

    ui->horizontalLayout->addWidget(shareButton);
    ui->excelButton->hide();
    ui->printButton->hide();

    ui->itemWiseTableView->setItemDelegate(new ReportCommonDelegate());

    setTable();
}

TimeBasedSales::~TimeBasedSales()
{
    delete ui;
}

void TimeBasedSales::on_OKButton_clicked()
{
    setTable();
}

void TimeBasedSales::on_excelButton_clicked()
{
    qDebug()<<"create Excel";

    QDate fDate = ui->fromDateDateEdit->date();
    QDate tDate = ui->toDateDateEdit->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString wwaitername ="";

    QString reportName = "Time Sales Report";

    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QStringList cols; cols<<"30"<<"20"<<"20";
    reportName = "Time Based Sales Report "+wwaitername;
    QList<int> avlCols; avlCols<<0<<1<<2;
    ex->GenericReportFromTableView(fileName,reportName,cols,ui->itemWiseTableView,fromDate,toDate,avlCols);
}

void TimeBasedSales::on_printButton_clicked()
{

    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<60<<20<<20;
    AlignmentList<< 0<< 1 <<1;
    QList<int> avlCols; avlCols<<0<<1<<2;
    CounterPrinterHelper *printer = new CounterPrinterHelper;
    printer-> printReportGeneral(ui->itemWiseTableView
                                 ,widthList,"Time Wise Sales Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}

void TimeBasedSales::on_backButton_clicked()
{
    this->close();
}

void TimeBasedSales::setTable()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = ledHelper->getTimeBasedSales(fromDateTime,toDateTime);

    ui->itemWiseTableView->setModel(model);
    ui->itemWiseTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);


    if(model->rowCount()==0)
        return;
    SingleSetBarchartHelper *chart = new SingleSetBarchartHelper(
                fromDate,toDate,model,0,1,"Time Based Report");

    if ( ui->chartLay != NULL )
    {
        QLayoutItem* item;
        while ( ( item = ui->chartLay->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
        //delete m_view->layout();
    }
    ui->chartLay->addWidget(chart->setBarChart());
}

void TimeBasedSales::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Time-wise Sales Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&TimeBasedSales::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&TimeBasedSales::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&TimeBasedSales::on_excelButton_clicked);
}

void TimeBasedSales::printReport(QPrinter *printer)
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
    printerHelper-> printReportGeneral(ui->itemWiseTableView
                                 ,widthList,"Time Wise Sales Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}
