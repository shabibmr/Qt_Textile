#include "datewisesalesreport.h"
#include "ui_datewisesalesreport.h"
#include "database/finance/Closing/shiftdatabasehelper.h"

DatewiseSalesReport::DatewiseSalesReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatewiseSalesReport)
{
    ui->setupUi(this);

    ledHelper = new LedgerHelper();
    ui->tableView->setItemDelegate(new LedgerReportDelegate());
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ShiftDatabaseHelper shift;
    dfrom = shift.getlastShiftDetailByVoucherPrefix("%").ShiftEndDateTime;

    ui->fromDateDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateDateEdit->setDate(QDate::currentDate());

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
//    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&DatewiseSalesReport::showShareOptions);

    ui->horizontalLayout->addWidget(shareButton);
    ui->excelButton->hide();
    ui->printButton->hide();

    setTable();

}

DatewiseSalesReport::~DatewiseSalesReport()
{
    delete ui;
}

void DatewiseSalesReport::on_OKButton_clicked()
{
    setTable();
}

void DatewiseSalesReport::on_excelButton_clicked()
{
    qDebug()<<"create Excel";

    QDate fDate = ui->fromDateDateEdit->date();
    QDate tDate = ui->toDateDateEdit->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");

    QString reportName = "Datewise_Sales_Report";

    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QStringList cols; cols<<"30"<<"30"<<"30"<<"30";
QList<int> avlCols; avlCols<<0<<1<<2<<3;
    ex->GenericReportFromTableView(fileName,reportName,cols,ui->tableView,fromDate,toDate,avlCols);
}

void DatewiseSalesReport::on_printButton_clicked()
{
    //    qDebug()<<"create Excel";

    //       QDate fDate = ui->fromDateDateEdit->date();
    //       QDate tDate = ui->toDateDateEdit->date();
    //       QString fromDate = fDate.toString("yyyy-MM-dd");
    //       QString toDate = tDate.toString("yyyy-MM-dd");
    //       QString wwaitername ="";

    //       QString reportName = "Datewise Sales Report";

    //       QDate curDate = QDate::currentDate();
    //       QString dateNow = curDate.toString("dd_MMM_yyyy");
    //       QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    //       if (fileName.isEmpty())
    //       {
    //           return;
    //       }

    float total = 0;
    float valTotal=0;
    float vatTotal =0;
    for(int i=0;i<ui->tableView->model()->rowCount();i++){
        valTotal += model->record(i).value(1).toFloat();
        vatTotal += model->record(i).value(2).toFloat();
        total += model->record(i).value(3).toFloat();
    }




    QStringList cols; cols<<"20"<<"20"<<"20"<<"20";

    //       ex->GenericReportFromTableView(fileName,reportName,cols,ui->tableView,fromDate,toDate);

    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;

    extraBottom << " Taxable Total : "+QString::number(valTotal,'f',2)
                << " Tax Total : "+QString::number(vatTotal,'f',2)
                << " Grand Total : "+QString::number(total,'f',2);

    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();

    widthList <<40<<20<<20<<20;
    AlignmentList<< 0<< 1<< 1 <<1;
    CounterPrinterHelper *printer = new CounterPrinterHelper;
    QList<int> avlCols; avlCols<<0<<1<<2<<3;

    printer-> printReportGeneral(ui->tableView
                                 ,widthList,"Sales Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);

}

void DatewiseSalesReport::setTable()
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

    model = ledHelper->getDatewiseSales(fromDateTime,toDateTime);
    ui->tableView->setModel(model);

}

void DatewiseSalesReport::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Datewise Sales Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
//    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&DatewiseSalesReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&DatewiseSalesReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&DatewiseSalesReport::on_excelButton_clicked);
}

void DatewiseSalesReport::printReport(QPrinter *printer)
{
    float total = 0;
    float valTotal=0;
    float vatTotal =0;
    for(int i=0;i<ui->tableView->model()->rowCount();i++){
        valTotal += model->record(i).value(1).toFloat();
        vatTotal += model->record(i).value(2).toFloat();
        total += model->record(i).value(3).toFloat();
    }

    QStringList cols; cols<<"20"<<"20"<<"20"<<"20";

    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;

    extraBottom << " Taxable Total : "+QString::number(valTotal,'f',2)
                << " Vat Total : "+QString::number(vatTotal,'f',2)
                << " Grand Total : "+QString::number(total,'f',2);

    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();

    widthList <<40<<20<<20<<20;
    AlignmentList<< 0<< 1<< 1 <<1;
    QList<int> avlCols; avlCols<<0<<1<<2<<3;

    printerHelper-> printReportGeneral(ui->tableView
                                 ,widthList,"Datewise Sales Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);

}
