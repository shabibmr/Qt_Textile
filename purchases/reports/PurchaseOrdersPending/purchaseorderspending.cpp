#include "purchaseorderspending.h"
#include "ui_purchaseorderspending.h"

#include "datamodels/Counter/countersettingsdatamodel.h"

PurchaseOrdersPending::PurchaseOrdersPending(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PurchaseOrdersPending)
{
    ui->setupUi(this);

    ui->fromDateDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateDateEdit->setDate(QDate::currentDate());
    dbhelper = new PurchaseOrderDatabaseHelper();

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
//    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&PurchaseOrdersPending::showShareOptions);

    ui->horizontalLayout->addWidget(shareButton);

    setTableView();
}

PurchaseOrdersPending::~PurchaseOrdersPending()
{
    delete ui;
}

void PurchaseOrdersPending::on_OKButton_clicked()
{
    setTableView();
}

void PurchaseOrdersPending::on_excelButton_clicked()
{
    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\PendingPurchaseOrders_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }

    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<20<<20<<20<<20<<60<<20;
    AlignmentList<< 0<<0<<0<<0<<0<<1;
    QList<int> avlCols; avlCols<<0<<1<<2<<3<<4<<5;

    ExcelReportHelper *ex = new ExcelReportHelper;

    ex->GenerateReportFromTableView(fileName,"Group Summary",
                                    widthList,ui->tableView,
                                    fromDate.toString("yyyy-MM-dd"),
                                    toDate.toString("yyyy-MM-dd"),
                                    avlCols);

}

void PurchaseOrdersPending::on_backButton_clicked()
{

}

void PurchaseOrdersPending::setTableView()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = dbhelper->getPendingPurchaseOrdersByDeliveryDate(fromDateTime,toDateTime);

    ui->tableView->setModel(model);
    ui->tableView->setSortingEnabled(true);

    ui->tableView->hideColumn(1);
    ui->tableView->setColumnWidth(3,150);

    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

}

void PurchaseOrdersPending::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Pending Orders Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
    shareOptionsWidget->excelButton->hide();
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&PurchaseOrdersPending::on_printButton_clicked);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&PurchaseOrdersPending::on_printButton_clicked);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&PurchaseOrdersPending::on_excelButton_clicked);
}

void PurchaseOrdersPending::on_printButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    CounterPrinterHelper *printer = new CounterPrinterHelper;
    widthList <<20<<20<<20<<20<<60<<20;
    AlignmentList<< 0<<0<<0<<0<<0<<1;
    QList<int> avlCols; avlCols<<0<<1<<2<<3<<4<<5;
    printer-> printReportGeneral(ui->tableView
                                 ,widthList,"Pending Purchase Orders Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}
