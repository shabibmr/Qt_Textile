#include "modeofservicewisereport.h"
#include "ui_modeofservicewisereport.h"
#include "database/finance/Closing/shiftdatabasehelper.h"
#include "customwidgets/Delegates/reportcommondelegate.h"

ModeOfServiceWiseReport::ModeOfServiceWiseReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModeOfServiceWiseReport)
{
    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());

    ui->tableView->setItemDelegate(new ReportCommonDelegate());

    ShiftDatabaseHelper shift;
    dfrom = shift.getlastShiftDetailByVoucherPrefix("%").ShiftEndDateTime;

    int i=0;


    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
//    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&ModeOfServiceWiseReport::showShareOptions);

    ui->horizontalLayout->addWidget(shareButton);
    ui->excelButton->hide();
    ui->printButton->hide();

    setView();

}

ModeOfServiceWiseReport::~ModeOfServiceWiseReport()
{
    delete ui;
}

void ModeOfServiceWiseReport::on_OKButton_clicked()
{
    setView();
}

void ModeOfServiceWiseReport::on_excelButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();
    QString reportName = "Mode Of Srvice Report";

    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QList<int> avlCols; avlCols<<0<<1;
    ex->GenerateReportFromTableView(fileName,reportName,
                                    widthList,ui->tableView,
                                    fromDate.toString("yyyy-MM-dd"),
                                    toDate.toString("yyyy-MM-dd"),
                                    avlCols);
}

void ModeOfServiceWiseReport::on_printButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<60<<20;
    AlignmentList<< 0<< 1;
    CounterPrinterHelper *printer = new CounterPrinterHelper;
    QList<int> avlCols; avlCols<<0<<1;
    printer-> printReportGeneral(ui->tableView
                                 ,widthList,"Service Wise Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}

void ModeOfServiceWiseReport::setView()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    if(LoginValues::Privilege<4){
       qDebug()<<"prevl= "<<LoginValues::Privilege;
        if(dfrom < toDateTime){
            qDebug()<<"Changing date to :"<<dfrom;
            toDateTime = dfrom;
        }
    }

     model = dbHelper.getServiceBasedSalesModel(fromDateTime,toDateTime);

     ui->tableView->setModel(model);
     ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



    if(model->rowCount()==0)
        return;
    SingleSetBarchartHelper *chart = new SingleSetBarchartHelper(
                fromDate,toDate,model,0,1,"Mode of Service");

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

void ModeOfServiceWiseReport::on_backButton_clicked()
{
    this->close();
}

void ModeOfServiceWiseReport::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Servicewise Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&ModeOfServiceWiseReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&ModeOfServiceWiseReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&ModeOfServiceWiseReport::on_excelButton_clicked);
}

void ModeOfServiceWiseReport::printReport(QPrinter *printer)
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


    widthList <<60<<20;
    AlignmentList<< 0<< 1;
    QList<int> avlCols; avlCols<<0<<1;
    printerHelper-> printReportGeneral(ui->tableView
                                 ,widthList,"Service Wise Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);

}
