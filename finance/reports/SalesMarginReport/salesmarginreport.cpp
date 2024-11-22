#include "salesmarginreport.h"
#include "ui_salesmarginreport.h"

SalesMarginReport::SalesMarginReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SalesMarginReport)
{
    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateDateEdit->setDate(QDate::currentDate());
    ui->tableView->setItemDelegate(new ReportCommonDelegate());

    dbHelper = new InventoryHelper;
    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
//    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&SalesMarginReport::showShareOptions);

    ui->horizontalLayout->addWidget(shareButton);
    ui->excelButton->hide();
    ui->printButton->hide();
    setTable();
}

SalesMarginReport::~SalesMarginReport()
{
    delete ui;
}

void SalesMarginReport::on_OKButton_clicked()
{
    setTable();
}

void SalesMarginReport::on_excelButton_clicked()
{
    qDebug()<<"create Excel";

    QDate fDate = ui->fromDateDateEdit->date();
    QDate tDate = ui->toDateDateEdit->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString wwaitername ="";

    QString reportName = "Itemwise Sales Margin Report";

    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QStringList cols; cols<<"30"<<"20"<<"20"<<"20"<<"20"<<"20";
    reportName = "Sales Margin Report "+wwaitername;
    QList<int> avlCols; avlCols<<0<<1<<2<<3<<4<<5;
    ex->GenericReportFromTableView(fileName,reportName,cols,ui->tableView,fromDate,toDate,avlCols);
}

void SalesMarginReport::on_printButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<35<<15<<15<<15;
    AlignmentList<< 0<< 1 <<1<<1;
    CounterPrinterHelper *printer = new CounterPrinterHelper;
    QList<int> avlCols; avlCols<<0<<2<<4<<5;

    printer-> printReportGeneral(ui->tableView
                                 ,widthList,"Sales Margin Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}

void SalesMarginReport::setTable()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = dbHelper->getSalesMarginReport(fromDateTime,toDateTime);

    ui->tableView->setModel(model);

    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    float purchTotal = 0;
    float marginTotal = 0;
    float salesTotal = 0;

    for(int i=0;i<model->rowCount();i++){
        salesTotal += model->record(i).value(2).toFloat();
        purchTotal += model->record(i).value(4).toFloat();
        marginTotal += model->record(i).value(5).toFloat();
    }


    ui->salesTotal->setText(QString::number(salesTotal,'f',2));
    ui->purchaseTotal->setText(QString::number(purchTotal,'f',2));
    ui->marginTotal->setText(QString::number(marginTotal,'f',2));


}

void SalesMarginReport::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Sales Margin Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
//    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&SalesMarginReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&SalesMarginReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&SalesMarginReport::on_excelButton_clicked);
}

void SalesMarginReport::printReport(QPrinter *printer)
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


    widthList <<35<<15<<15<<15;
    AlignmentList<< 0<< 1 <<1<<1;
    QList<int> avlCols; avlCols<<0<<2<<4<<5;

    printerHelper-> printReportGeneral(ui->tableView
                                 ,widthList,"Sales Margin Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}
