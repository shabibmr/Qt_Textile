#include "waiterwiseservicewisereport.h"
#include "database/finance/Closing/shiftdatabasehelper.h"
#include "customwidgets/Delegates/reportcommondelegate.h"


WaiterwiseServicewiseReport::WaiterwiseServicewiseReport(QWidget *parent) :
    QWidget(parent)
{

    QPalette pal = palette();
    this->setStyleSheet("QWidget#report{background-color:#ffffff}");

    pal.setColor(QPalette::Background,QColor("#ffffff"));
    setAutoFillBackground(true);
    setPalette(pal);

    appBar = new  AppBarWidget("Salesmanwise Servicewise Report");
    appBar->backButton->hide();
    connect(appBar, &AppBarWidget::shareButtonClicked, this, &WaiterwiseServicewiseReport::showShareOptions);

    filterWid = new FilterWidget(this);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    filterWid->showFromDate(true);
    filterWid->showToDate(true);
    connect(filterWid, &FilterWidget::refresh, this, &WaiterwiseServicewiseReport::setView);

    ShiftDatabaseHelper shift;
    dfrom = shift.getlastShiftDetailByVoucherPrefix("%").ShiftEndDateTime;

    QVBoxLayout *vLay = new QVBoxLayout(this);
    this->setLayout(vLay);

    tableView = new ReportsTableView("Total");
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    vLay->addWidget(appBar);
    vLay->addWidget(filterWid);
    vLay->addWidget(tableView);


   setView();

}

WaiterwiseServicewiseReport::~WaiterwiseServicewiseReport()
{
}

void WaiterwiseServicewiseReport::on_OKButton_clicked()
{
    setView();
}

void WaiterwiseServicewiseReport::on_excelButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = filterWid->fromDate;
    QDate toDate = filterWid->toDate;

    widthList <<20;
    AlignmentList<< 0;
    QList<int> avlCols; avlCols<<0;
    for(int i=1; i<model->columnCount(); i++){
        avlCols<<i;
        widthList <<20;
        AlignmentList<< 1;
    }

    QString reportName = "Servicewise Report";

    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

//    QList<int> avlCols; avlCols<<0<<1;
    ex->GenerateReportFromTableView(fileName,reportName,
                                    widthList,tableView,
                                    fromDate.toString("yyyy-MM-dd"),
                                    toDate.toString("yyyy-MM-dd"),
                                    avlCols);
}

void WaiterwiseServicewiseReport::on_printButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = filterWid->fromDate;
    QDate toDate = filterWid->toDate;


    widthList <<60<<20;
    AlignmentList<< 0<< 1;
    CounterPrinterHelper *printer = new CounterPrinterHelper;
    QList<int> avlCols; avlCols<<0<<1;
    printer-> printReportGeneral(tableView
                                 ,widthList,"Service Wise Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}

void WaiterwiseServicewiseReport::setView()
{
    QDate fromDate = filterWid->fromDate;
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = filterWid->toDate.addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

//    if(LoginValues::Privilege<4){
//       qDebug()<<"prevl= "<<LoginValues::Privilege;
//        if(dfrom < toDateTime){
//            qDebug()<<"Changing date to :"<<dfrom;
//            toDateTime = dfrom;
//        }
//    }

    qDebug()<<Q_FUNC_INFO<<"from"<<fromDateTime<<toDateTime;

     model = dbHelper.getSalesManwiseServicewiseReport(fromDateTime,toDateTime);

     QList<int> avlcols;
    for(int i=1; i<model->columnCount(); i++){
        avlcols<<i;
    }

     tableView->setModel(model);
     tableView->setItemDelegate(new ReportCommonDelegate);
     tableView->setModelQry(model);
     tableView->setAvlcols(avlcols);
     tableView->setColumns();
     tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



    if(model->rowCount()==0)
        return;
}

void WaiterwiseServicewiseReport::on_backButton_clicked()
{
    this->close();
}

void WaiterwiseServicewiseReport::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Servicewise Report", appBar->shareButton);
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&WaiterwiseServicewiseReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&WaiterwiseServicewiseReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&WaiterwiseServicewiseReport::on_excelButton_clicked);
}

void WaiterwiseServicewiseReport::printReport(QPrinter *printer)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    printer->setPrinterName("printerName");
    printerHelper = new CounterPrinterHelper;
    printerHelper->setPrinter(printer);
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = filterWid->fromDate;
    QDate toDate = filterWid->toDate;


    widthList <<20;
    AlignmentList<< 0;
    QList<int> avlCols; avlCols<<0;
    float wid = 80/(model->columnCount()-1);
    for(int i=1; i<model->columnCount(); i++){
        avlCols<<i;
        widthList <<wid;
        AlignmentList<< 1;
    }

    printerHelper-> printReportGeneral(tableView
                                 ,widthList,"Service Wise Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);

}
