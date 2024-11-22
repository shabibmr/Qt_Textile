#include "waiterwisesalesreport.h"
#include "ui_waiterwisesalesreport.h"
#include "customwidgets/Charts/SinglesSet/singlesetbarcharthelper.h"
#include <database/finance/Closing/shiftdatabasehelper.h>
#include "customwidgets/Delegates/reportcommondelegate.h"
WaiterWiseSalesReport::WaiterWiseSalesReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaiterWiseSalesReport)
{
    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());
    ledHelper = new LedgerHelper;

    ShiftDatabaseHelper shift;
    dfrom = shift.getlastShiftDetailByVoucherPrefix("%").ShiftEndDateTime;

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
//    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&WaiterWiseSalesReport::showShareOptions);

    ui->horizontalLayout->addWidget(shareButton);
    ui->excelButton->hide();
    ui->printButton->hide();

     if(CounterSettingsDataModel::TypeOfTrade == "ERP"){
        ui->tabWidget->removeTab(0);
     }

     ui->amountWiseTableView->setItemDelegate(new ReportCommonDelegate());
     ui->itemWiseTableView->setItemDelegate(new ReportCommonDelegate());

    setTable();
}

WaiterWiseSalesReport::~WaiterWiseSalesReport()
{
    delete ui;
}

void WaiterWiseSalesReport::on_OKButton_clicked()
{
    setTable();
}

void WaiterWiseSalesReport::on_excelButton_clicked()
{
    qDebug()<<"create Excel";

    QDate fDate = ui->fromDateDateEdit->date();
    QDate tDate = ui->toDateDateEdit->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString wwaitername ="";

    QString reportName = "Waiterwise Sales Report";

    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }

    ExcelReportHelper *ex = new ExcelReportHelper;

    QStringList cols; cols<<"60"<<"20"<<"20";
    reportName = "Waiter Wise Sales Report "+wwaitername;
    QList<int> avlCols; avlCols<<0<<1<<2;
    ex->GenericReportFromTableView(fileName,reportName,cols,
                                   ui->amountWiseTableView,fromDate,toDate,avlCols);
}

void WaiterWiseSalesReport::on_printButton_clicked()
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
    printer-> printReportGeneral(ui->amountWiseTableView
                                 ,widthList,"Waiter Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}

void WaiterWiseSalesReport::on_backButton_clicked()
{
    this->close();
}

void WaiterWiseSalesReport::setTable()
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

    model = ledHelper->getSalesManwiseReport(fromDateTime,toDateTime);
    deliveryModel = ledHelper->getSalesManwiseReportByService(fromDateTime,toDateTime,ServiceMode::Delivery);

    ui->amountWiseTableView->setModel(model);

    ui->amountWiseTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    ui->itemWiseTableView->setModel(deliveryModel);

    ui->itemWiseTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    if(model->rowCount()==0)
        return;
    SingleSetBarchartHelper *chart = new SingleSetBarchartHelper(
                fromDate,toDate,model,0,2,"Salesmanwise");

    if ( ui->layoutChart != NULL )
    {
        QLayoutItem* item;
        while ( ( item = ui->layoutChart->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
        //delete m_view->layout();
    }
    ui->layoutChart->addWidget(chart->setBarChart());

}

void WaiterWiseSalesReport::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Waiter-wise Sales Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
//    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&WaiterWiseSalesReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&WaiterWiseSalesReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&WaiterWiseSalesReport::on_excelButton_clicked);
}

void WaiterWiseSalesReport::printReport(QPrinter *printer)
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
    printerHelper-> printReportGeneral(ui->amountWiseTableView
                                 ,widthList,"Waiter Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}
